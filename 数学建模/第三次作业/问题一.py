import numpy as np
import pandas as pd
import networkx as nx
import matplotlib.pyplot as plt
import os
from scipy.stats import norm

# 配置中文显示
plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False

def save_result_file(data, filename, folder="risk_analysis_results"):
    """将计算结果保存到指定文件夹"""
    # 确保文件夹存在
    if not os.path.exists(folder):
        os.makedirs(folder)
    
    filepath = os.path.join(folder, filename)
    
    # 根据数据类型选择保存方式
    with open(filepath, 'w', encoding='utf-8') as f:
        if isinstance(data, pd.DataFrame):
            f.write(data.to_string())
        elif isinstance(data, dict):
            for k, v in data.items():
                f.write(f"{k}: {v}\n")
        else:
            f.write(str(data))

class DistributionNetworkRiskEvaluator:
    """配电网风险评估模型"""
    
    def __init__(self, node_data, line_data):
        """
        初始化风险评估模型
        
        参数:
        node_data: 包含节点负荷信息的DataFrame
        line_data: 包含线路拓扑信息的DataFrame
        """
        self.node_data = node_data
        self.line_data = line_data
        
        # 记录节点负荷信息
        self.node_load_dict = {i+1: row['有功P/kW'] for i, row in node_data.iterrows()}
        
        # 客户类型定义和损失系数
        self.customer_types = {
            'residential': {'name': '居民', 'loss_factor': 1.0},
            'commercial': {'name': '商业', 'loss_factor': 3.0},
            'government': {'name': '政府机构', 'loss_factor': 5.0},
            'office': {'name': '办公建筑', 'loss_factor': 2.5}
        }
        
        # 节点客户类型映射
        self.node_customer_type = self._initialize_customer_types()
        
        # 分布式能源数据，初始为空
        self.distributed_generators = {}
        
        # 系统参数设置
        self.feeder_rated_capacity = 2200  # 馈线额定容量(kW)
        self.feeder_rated_current = 220    # 馈线额定电流(A)
        self.system_voltage = 10           # 电压等级(kV)
        
        # 故障率参数
        self.dg_failure_rate = 0.005       # 分布式能源故障率
        self.load_failure_rate = 0.005     # 用户负荷故障率
        self.switch_failure_rate = 0.002   # 开关故障率
        self.line_failure_rate_km = 0.002  # 线路故障率(每公里)
        
        # 联络开关信息
        self.tie_switches = {
            "TS1-2": (13, 23),  # 连接馈线1和馈线2
            "TS2-3": (29, 43),  # 连接馈线2和馈线3
            "TS3-1": (62, 1)    # 连接馈线3和馈线1
        }
        
        # 变电站出线开关
        self.main_circuit_breakers = {
            "CB1": 1,   # 馈线1连接节点
            "CB2": 23,  # 馈线2连接节点
            "CB3": 43   # 馈线3连接节点
        }
        
        # 馈线区域定义
        self.feeders = {
            "F1": list(range(1, 23)),
            "F2": list(range(23, 43)),
            "F3": list(range(43, 63))
        }
        
        # 构建网络拓扑
        self.network = self._create_network_topology()
        
    def _initialize_customer_types(self):
        """初始化节点客户类型"""
        node_customer_type = {}
        
        # 设置居民节点
        residential_nodes = [1, 2, 3, 4, 5, 6, 8, 10, 13, 15, 17, 19, 20, 
                           22, 23, 24, 25, 26, 28, 30, 35, 37, 39, 41, 43, 44, 
                           46, 48, 50, 51, 52, 54, 55, 56, 59, 60, 62]
        for node in residential_nodes:
            node_customer_type[node] = 'residential'
        
        # 设置商业节点
        commercial_nodes = [11, 16, 27, 31, 33, 34, 38, 42, 49, 53, 56]
        for node in commercial_nodes:
            node_customer_type[node] = 'commercial'
        
        # 设置政府和机构节点
        government_nodes = [9, 21, 29, 36, 45, 58, 61]
        for node in government_nodes:
            node_customer_type[node] = 'government'
        
        # 设置办公和建筑节点
        office_nodes = [7, 12, 14, 18, 32, 40, 47]
        for node in office_nodes:
            node_customer_type[node] = 'office'
        
        return node_customer_type
    
    def _calculate_node_damage(self, node, outage_duration=1.0):
        """
        计算节点故障的危害度

        参数:
        node: 节点编号
        outage_duration: 停电持续时间(小时)，默认为1小时

        返回:
        危害度值
        """
        # 获取节点负荷和客户类型
        load = self.node_load_dict.get(node, 0)
        customer_type = self.node_customer_type.get(node, 'residential')  # 默认为居民
        loss_factor = self.customer_types[customer_type]['loss_factor']

        # 基础危害度 = 负荷 × 停电时间 × 客户类型损失系数
        base_damage = load * outage_duration * loss_factor

        # 非线性参数（仅在此函数内使用）
        damage_params = {
            'residential': {'threshold_hr': 4, 'extra_rate': 0.10},
            'commercial': {
                'phase1': {'hour': 1, 'coeff': 0.50},
                'phase2': {'hour': 4, 'coeff': 0.30},
                'phase3': {'coeff': 0.10},
            },
            'government': {
                'phase1': {'hour': 2, 'init_coeff': 1.50, 'slope': 0.40},
                'phase2': {'init_coeff': 2.30, 'slope': 0.20},
            },
            'office': {'phase1': {'hour': 2, 'slope1': 0.30, 'slope2': 0.10}},
        }

        # 根据客户类型添加非线性危害度计算
        if customer_type == 'residential':
        # 居民用户：前 threshold_hr 小时线性，之后每小时额外加成
            params = damage_params['residential']
            threshold = params['threshold_hr']
            extra_rate = params['extra_rate']
            if outage_duration <= threshold:
                damage = base_damage
            else:
                extra_hours = outage_duration - threshold
                extra_damage = load * threshold * loss_factor * extra_rate * extra_hours
                damage = base_damage + extra_damage

        elif customer_type == 'commercial':
            # 商业用户：分三阶段增长
            params = damage_params['commercial']
            h1, c1 = params['phase1']['hour'], params['phase1']['coeff']
            h2, c2 = params['phase2']['hour'], params['phase2']['coeff']
            c3 = params['phase3']['coeff']
            if outage_duration <= h1:
                factor = 1.0 + c1 * outage_duration
            elif outage_duration <= h2:
                factor = (1.0 + c1 * h1) + c2 * (outage_duration - h1)
            else:
                factor = (1.0 + c1 * h1) + c2 * (h2 - h1) + c3 * (outage_duration - h2)
            damage = base_damage * factor

        elif customer_type == 'government':
            # 政府机构：前 phase1 阶段和后 phase2 阶段不同斜率
            params = damage_params['government']
            h1 = params['phase1']['hour']
            init1 = params['phase1']['init_coeff']
            slope1 = params['phase1']['slope']
            init2 = params['phase2']['init_coeff']
            slope2 = params['phase2']['slope']
            if outage_duration <= h1:
                factor = init1 + slope1 * outage_duration
            else:
                factor = init2 + slope2 * (outage_duration - h1)
            damage = base_damage * factor

        elif customer_type == 'office':
            # 办公建筑：统一按“工作时段”模型计算（无时间维度区分）
            params = damage_params['office']['phase1']
            h1 = params['hour']
            slope1 = params['slope1']
            slope2 = params['slope2']
            if outage_duration <= h1:
                factor = 1.0 + slope1 * outage_duration
            else:
                factor = (1.0 + slope1 * h1) + slope2 * (outage_duration - h1)
            damage = base_damage * factor

        else:
            # 其他类型：仅线性
            damage = base_damage

        return damage

        
    def _create_network_topology(self):
        """构建配电网络拓扑结构"""
        G = nx.Graph()
        
        # 添加节点和负荷信息
        for i in range(1, len(self.node_data) + 1):
            customer_type = self.node_customer_type.get(i, 'residential')
            G.add_node(i, 
                      load=self.node_data.loc[i-1, '有功P/kW'],
                      customer_type=customer_type)
        
        # 添加线路及其属性
        for _, line in self.line_data.iterrows():
            start = int(line['起点'])
            end = int(line['终点'])
            length = float(line['长度/km'])
            r = float(line['电阻/Ω'])
            x = float(line['电抗/Ω'])
            failure_rate = length * self.line_failure_rate_km
            
            G.add_edge(start, end, length=length, resistance=r, 
                      reactance=x, failure_rate=failure_rate)
        
        return G
    
    def set_distributed_generators(self, dg_data):
        """
        设置分布式能源信息
        
        参数:
        dg_data: 字典，格式为 {id: {'node': 节点编号, 'capacity': 容量(kW)}}
        """
        self.distributed_generators = dg_data
    
    def calculate_load_loss_risk(self):
        """
        计算系统失负荷风险
        
        考虑节点故障、线路故障和开关故障三种情况
        同时考虑通过联络线实现部分负荷转移的能力
        
        返回:
        失负荷风险值
        """
        total_risk = 0
        
        # 1. 节点故障导致的失负荷风险
        for node in self.network.nodes():
            # 确定节点故障率
            if node in [dg_info['node'] for dg_info in self.distributed_generators.values()]:
                failure_prob = self.dg_failure_rate
            else:
                failure_prob = self.load_failure_rate
                
            # 计算节点故障造成的负荷损失和危害
            load_loss, damage = self._node_failure_impact(node)
            
            # 累加风险
            node_risk = failure_prob * damage
            total_risk += node_risk
        
        # 2. 线路故障导致的失负荷风险
        for u, v, data in self.network.edges(data=True):
            failure_prob = data['failure_rate']
            load_loss, damage = self._line_failure_impact(u, v)
            
            line_risk = failure_prob * damage
            total_risk += line_risk
        
        # 3. 开关故障导致的失负荷风险
        for switch_name, nodes in self.tie_switches.items():
            failure_prob = self.switch_failure_rate
            load_loss, damage = self._switch_failure_impact(nodes)
            
            switch_risk = failure_prob * damage
            total_risk += switch_risk
            
        return total_risk
    
    def _node_failure_impact(self, node):
        """计算节点故障导致的负荷损失和危害"""
        # 确定节点所属馈线
        feeder_name = None
        for name, nodes in self.feeders.items():
            if node in nodes:
                feeder_name = name
                break
                
        if feeder_name is None:
            return 0, 0
            
        # 如果是DG节点，计算DG容量损失
        for dg_info in self.distributed_generators.values():
            if dg_info['node'] == node:
                # DG损失按其容量计算，但危害度需考虑对周围负荷的影响
                affected_nodes = self._get_nodes_dependent_on_dg(node)
                damage = sum(self._calculate_node_damage(n) for n in affected_nodes)
                return dg_info['capacity'], damage
                
        # 负荷节点，计算负荷损失并考虑负荷转移
        load = self.node_load_dict.get(node, 0)
        
        # 检查其他馈线的剩余容量，确定可转移负荷
        remaining_capacity = self._calculate_feeder_remaining_capacity(feeder_name)
        transferable_load = min(load, remaining_capacity)
        
        # 实际负荷损失 = 总负荷 - 可转移负荷
        actual_loss = load - transferable_load
        
        # 计算危害程度
        damage = self._calculate_node_damage(node) * (actual_loss / load if load > 0 else 0)
        
        return actual_loss, damage
    
    def _get_nodes_dependent_on_dg(self, dg_node):
        """获取依赖某个DG的节点"""
        # 简化实现，假设节点所在馈线的所有节点都依赖该DG
        dependent_nodes = []
        for feeder_name, nodes in self.feeders.items():
            if dg_node in nodes:
                dependent_nodes = nodes
                break
        return dependent_nodes
    
    def _line_failure_impact(self, u, v):
        """计算线路故障导致的负荷损失和危害"""
        # 模拟线路故障
        temp_network = self.network.copy()
        temp_network.remove_edge(u, v)
        
        # 识别受影响节点
        affected_nodes = set()
        for feeder_name, nodes in self.feeders.items():
            # 获取该馈线的变电站连接点
            substation_node = next(node for cb, node in self.main_circuit_breakers.items() 
                                if node in nodes)
            
            # 检查哪些节点无法从变电站获取电力
            for node in nodes:
                if node != substation_node and not nx.has_path(temp_network, substation_node, node):
                    affected_nodes.add(node)
        
        # 计算总负荷损失
        total_loss = sum(self.node_load_dict.get(node, 0) for node in affected_nodes)
        
        # 考虑DG的供电能力
        for dg_info in self.distributed_generators.values():
            if dg_info['node'] in affected_nodes:
                # DG可以为受影响区域提供部分供电
                recoverable = min(dg_info['capacity'], total_loss)
                total_loss -= recoverable
                # 从受影响节点中移除可恢复的部分
                self._update_affected_nodes_after_recovery(affected_nodes, recoverable)
        
        # 考虑联络线的负荷转移能力
        for (node1, node2) in self.tie_switches.values():
            # 检查联络线是否可以帮助恢复供电
            if node1 in affected_nodes and node2 not in affected_nodes:
                # 找到node2所在的馈线
                for feeder_name, nodes in self.feeders.items():
                    if node2 in nodes:
                        remaining = self._calculate_feeder_remaining_capacity(feeder_name)
                        transferable = min(total_loss, remaining)
                        total_loss -= transferable
                        # 更新受影响节点
                        self._update_affected_nodes_after_recovery(affected_nodes, transferable)
                        break
            elif node2 in affected_nodes and node1 not in affected_nodes:
                # 找到node1所在的馈线
                for feeder_name, nodes in self.feeders.items():
                    if node1 in nodes:
                        remaining = self._calculate_feeder_remaining_capacity(feeder_name)
                        transferable = min(total_loss, remaining)
                        total_loss -= transferable
                        # 更新受影响节点
                        self._update_affected_nodes_after_recovery(affected_nodes, transferable)
                        break
        
        # 计算总危害度
        total_damage = sum(self._calculate_node_damage(node) for node in affected_nodes)
        
        return total_loss, total_damage
    
    def _update_affected_nodes_after_recovery(self, affected_nodes, recovered_load):
        """更新故障恢复后的受影响节点集合"""
        # 按照一定优先级恢复负荷
        # 优先恢复政府机构 > 商业 > 办公建筑 > 居民
        priority_order = ['government', 'commercial', 'office', 'residential']
        
        remaining_recovery = recovered_load
        nodes_to_remove = set()
        
        # 按优先级逐类恢复
        for customer_type in priority_order:
            if remaining_recovery <= 0:
                break
                
            # 找出该类型的节点
            nodes_of_type = [n for n in affected_nodes 
                           if n in self.node_customer_type and 
                           self.node_customer_type[n] == customer_type]
            
            # 按负荷大小排序
            nodes_of_type.sort(key=lambda n: self.node_load_dict.get(n, 0), reverse=True)
            
            # 逐个恢复节点
            for node in nodes_of_type:
                node_load = self.node_load_dict.get(node, 0)
                if remaining_recovery >= node_load:
                    remaining_recovery -= node_load
                    nodes_to_remove.add(node)
                    if remaining_recovery <= 0:
                        break
        
        # 从受影响节点集合中移除已恢复的节点
        for node in nodes_to_remove:
            affected_nodes.discard(node)
    
    def _switch_failure_impact(self, nodes):
        """计算开关故障导致的负荷损失和危害"""
        # 在此实现中，开关故障主要影响联络线的负荷转移能力
        # 假设开关故障发生在重负荷时段，会导致一定比例的负荷无法转移
        node1, node2 = nodes
        
        # 获取两端节点所在馈线
        feeder1, feeder2 = None, None
        for name, feeder_nodes in self.feeders.items():
            if node1 in feeder_nodes:
                feeder1 = name
            if node2 in feeder_nodes:
                feeder2 = name
        
        if not feeder1 or not feeder2 or feeder1 == feeder2:
            return 0, 0
            
        # 计算假设性故障情况下无法转移的负荷
        feeder1_nodes = self.feeders[feeder1]
        feeder2_nodes = self.feeders[feeder2]
        
        # 简化计算：假设10%的负荷在故障情况下无法通过联络线转移
        untransferable_ratio = 0.1
        
        # 计算两个馈线的总负荷
        feeder1_load = sum(self.node_load_dict.get(n, 0) for n in feeder1_nodes)
        feeder2_load = sum(self.node_load_dict.get(n, 0) for n in feeder2_nodes)
        
        # 无法转移的负荷
        untransferable_load = (feeder1_load + feeder2_load) * untransferable_ratio
        
        # 计算危害度：选择影响较大的几个节点计算危害
        # 优先选择政府、商业节点
        critical_nodes = []
        for node_type in ['government', 'commercial']:
            critical_nodes.extend([n for n in feeder1_nodes if self.node_customer_type.get(n, '') == node_type])
            critical_nodes.extend([n for n in feeder2_nodes if self.node_customer_type.get(n, '') == node_type])
        
        # 如果关键节点不足，添加一些大负荷节点
        if len(critical_nodes) < 5:
            all_nodes = feeder1_nodes + feeder2_nodes
            # 按负荷降序排列
            sorted_nodes = sorted(all_nodes, 
                                key=lambda n: self.node_load_dict.get(n, 0), 
                                reverse=True)
            critical_nodes.extend(sorted_nodes[:5-len(critical_nodes)])
        
        # 计算这些关键节点的总危害度
        damage = sum(self._calculate_node_damage(n) * untransferable_ratio 
                   for n in critical_nodes)
        
        return untransferable_load, damage
    
    def _calculate_feeder_remaining_capacity(self, feeder_name):
        """计算馈线的剩余容量"""
        nodes = self.feeders[feeder_name]
        
        # 馈线总负荷
        total_load = sum(self.node_load_dict.get(node, 0) for node in nodes)
        
        # DG总容量
        dg_capacity = 0
        for dg_info in self.distributed_generators.values():
            if dg_info['node'] in nodes:
                dg_capacity += dg_info['capacity']
        
        # 考虑DG不向上级电网倒送功率
        net_load = max(0, total_load - dg_capacity)
        
        # 剩余容量 = 额定容量 - 净负荷
        return self.feeder_rated_capacity - net_load
    
    def calculate_overload_risk(self):
        """
        计算过负荷风险
        
        考虑分布式能源不得向上级电网倒送功率的情况
        需分析每条馈线的负荷情况和DG出力
        
        返回:
        过负荷风险值
        """
        total_risk = 0
        base_risk = 0.1  # 基础风险值
        
        # 分析每条馈线的负载情况
        for feeder_name, nodes in self.feeders.items():
            # 计算馈线总负荷
            total_load = sum(self.node_load_dict.get(node, 0) for node in nodes)
            
            # 计算馈线上DG总容量
            dg_capacity = 0
            for dg_info in self.distributed_generators.values():
                if dg_info['node'] in nodes:
                    dg_capacity += dg_info['capacity']
            
            # 计算净负荷（考虑DG不向上级电网倒送功率）
            net_load = max(0, total_load - dg_capacity)
            excess_power = 0
            
            # 计算馈线电流
            if net_load > 0:
                current = net_load / (self.system_voltage * 1000 * np.sqrt(3))
            else:
                # DG容量超过负荷，产生多余功率
                current = 0
                excess_power = dg_capacity - total_load
                
                # 检查是否可以通过联络线向其他馈线转移多余功率
                for node1, node2 in self.tie_switches.values():
                    # 寻找相邻馈线
                    if node1 in nodes or node2 in nodes:
                        adjacent_node = node2 if node1 in nodes else node1
                        
                        # 确定相邻馈线
                        adjacent_feeder = None
                        for fname, fnodes in self.feeders.items():
                            if adjacent_node in fnodes:
                                adjacent_feeder = fname
                                break
                        
                        if adjacent_feeder and adjacent_feeder != feeder_name:
                            # 计算相邻馈线的负载余量
                            adj_total_load = sum(self.node_load_dict.get(n, 0) 
                                              for n in self.feeders[adjacent_feeder])
                            adj_dg_capacity = 0
                            for dg_info in self.distributed_generators.values():
                                if dg_info['node'] in self.feeders[adjacent_feeder]:
                                    adj_dg_capacity += dg_info['capacity']
                                    
                            adj_net_load = max(0, adj_total_load - adj_dg_capacity)
                            adj_remaining = self.feeder_rated_capacity - adj_net_load
                            
                            # 计算可转移的功率
                            transferable = min(excess_power, adj_remaining)
                            excess_power -= transferable
            
            # 如果仍有多余功率，需要限制DG出力，增加过负荷风险
            if excess_power > 0:
                overload_prob = min(1.0, excess_power / self.feeder_rated_capacity)
                
                # 计算危害度时考虑客户类型
                customer_impact = 0
                for node in nodes:
                    if self.node_customer_type.get(node) == 'government':
                        customer_impact += 2.0  # 政府机构受到DG限制影响更大
                    elif self.node_customer_type.get(node) == 'commercial':
                        customer_impact += 1.5  # 商业用户
                    else:
                        customer_impact += 1.0  # 其他用户
                
                overload_impact = excess_power * (customer_impact / len(nodes))
                total_risk += overload_prob * overload_impact
            
            # 如果电流超过额定值110%，增加过负荷风险
            if current > self.feeder_rated_current * 1.1:
                overload_ratio = current / (self.feeder_rated_current * 1.1)
                overload_prob = min(1.0, (overload_ratio - 1) * 2)
                
                # 过负荷风险与客户类型相关
                customer_composition = {
                    'residential': 0,
                    'commercial': 0,
                    'government': 0,
                    'office': 0
                }
                
                # 统计各类客户比例
                for node in nodes:
                    customer_type = self.node_customer_type.get(node, 'residential')
                    customer_composition[customer_type] += self.node_load_dict.get(node, 0)
                
                # 计算总负荷
                total_feeder_load = sum(customer_composition.values())
                
                # 计算危害系数
                if total_feeder_load > 0:
                    hazard_factor = (
                        5.0 * customer_composition['government'] / total_feeder_load +
                        3.0 * customer_composition['commercial'] / total_feeder_load +
                        2.5 * customer_composition['office'] / total_feeder_load +
                        1.0 * customer_composition['residential'] / total_feeder_load
                    )
                else:
                    hazard_factor = 1.0
                
                overload_impact = (current - self.feeder_rated_current * 1.1) * \
                                self.feeder_rated_capacity / (self.feeder_rated_current * 1.1) * \
                                hazard_factor
                                
                total_risk += overload_prob * overload_impact
            else:
                # 即使未超过额定值，也考虑潜在风险
                load_ratio = current / self.feeder_rated_current
                potential_risk = base_risk * (1 + load_ratio)
                total_risk += potential_risk
        
        return max(total_risk, base_risk)  # 确保风险不为零
    
    def calculate_system_risk(self):
        """
        计算系统总风险
        
        系统总风险 = 失负荷风险 + 过负荷风险
        """
        # 计算两种风险
        load_loss_risk = self.calculate_load_loss_risk()
        overload_risk = self.calculate_overload_risk()
        
        # 计算总风险
        total_risk = load_loss_risk + overload_risk
        
        # 保存详细结果
        self.risk_results = {
            '失负荷风险': load_loss_risk,
            '过负荷风险': overload_risk,
            '系统总风险': total_risk
        }
        
        # 计算按客户类型的风险分布
        self.customer_type_risks = self._calculate_risk_by_customer_type()
        
        self._save_risk_analysis()
        
        return total_risk
    
    def _calculate_risk_by_customer_type(self):
        """计算不同客户类型的风险分布"""
        # 初始化各类客户风险
        customer_risks = {ctype: 0.0 for ctype in self.customer_types.keys()}
        
        # 计算各类客户总负荷
        customer_loads = {ctype: 0.0 for ctype in self.customer_types.keys()}
        for node, load in self.node_load_dict.items():
            ctype = self.node_customer_type.get(node, 'residential')
            customer_loads[ctype] += load
            
        # 计算总负荷
        total_load = sum(customer_loads.values())
        
        # 按负荷比例和危害度系数分配风险
        if total_load > 0:
            for ctype, load in customer_loads.items():
                load_ratio = load / total_load
                hazard_factor = self.customer_types[ctype]['loss_factor']
                # 考虑负荷比例和危害度系数
                weighted_factor = load_ratio * hazard_factor
                # 归一化权重
                total_weight = sum(customer_loads[t] / total_load * self.customer_types[t]['loss_factor'] 
                                 for t in self.customer_types.keys())
                if total_weight > 0:
                    normalized_weight = weighted_factor / total_weight
                    # 分配风险
                    customer_risks[ctype] = self.risk_results['系统总风险'] * normalized_weight
        
        return customer_risks
    
    def _save_risk_analysis(self):
        """保存风险分析结果"""
        # 确保结果文件夹存在
        results_dir = '配电网风险分析结果'
        if not os.path.exists(results_dir):
            os.makedirs(results_dir)
        
        # 保存分析结果
        with open(f'{results_dir}/系统风险分析报告.txt', 'w', encoding='utf-8') as f:
            f.write("===== 配电网风险分析报告 =====\n\n")
            for key, value in self.risk_results.items():
                f.write(f"{key}: {value:.4f}\n")
            
            load_loss_pct = self.risk_results['失负荷风险'] / self.risk_results['系统总风险'] * 100
            overload_pct = self.risk_results['过负荷风险'] / self.risk_results['系统总风险'] * 100
            
            f.write("\n风险构成分析:\n")
            f.write(f"- 失负荷风险占比: {load_loss_pct:.2f}%\n")
            f.write(f"- 过负荷风险占比: {overload_pct:.2f}%\n\n")
            
            f.write("不同客户类型风险分布:\n")
            for ctype, risk in self.customer_type_risks.items():
                name = self.customer_types[ctype]['name']
                pct = risk / self.risk_results['系统总风险'] * 100
                f.write(f"- {name}客户风险: {risk:.2f} ({pct:.2f}%)\n")
            
            risk_level = '高' if self.risk_results['系统总风险'] > 30 else \
                       '中' if self.risk_results['系统总风险'] > 15 else '低'
            f.write(f"\n系统风险等级: {risk_level}\n")
    
    def visualize_network(self):
        """可视化配电网络拓扑"""
        plt.figure(figsize=(15, 10))
        
        # 使用spring布局算法绘制网络
        pos = nx.spring_layout(self.network, seed=42)
        
        # 设置不同客户类型的节点颜色
        color_map = {
            'residential': 'skyblue',
            'commercial': 'green',
            'government': 'red',
            'office': 'orange'
        }
        
        # 创建颜色列表
        node_colors = []
        for node in self.network.nodes():
            if node in [dg_info['node'] for dg_info in self.distributed_generators.values()]:
                node_colors.append('gray')  # DG节点为灰色
            else:
                ctype = self.node_customer_type.get(node, 'residential')
                node_colors.append(color_map[ctype])
        
        # 绘制基本网络结构
        nx.draw(self.network, pos, with_labels=True, 
                node_color=node_colors, node_size=600, 
                font_weight='bold', font_size=10)
        
        # 添加节点负荷和类型标签
        node_labels = {}
        for n in self.network.nodes():
            load = self.node_load_dict[n]
            ctype = self.node_customer_type.get(n, 'residential')
            ctype_short = self.customer_types[ctype]['name'][0]
            node_labels[n] = f"{n}\n{load}kW\n({ctype_short})"
            
        nx.draw_networkx_labels(self.network, pos, labels=node_labels, font_size=8)
        
        # 添加馈线区域标注
        for feeder_name, nodes in self.feeders.items():
            center_x = sum(pos[n][0] for n in nodes) / len(nodes)
            center_y = sum(pos[n][1] for n in nodes) / len(nodes)
            plt.text(center_x, center_y, feeder_name, fontsize=20, 
                   color='red', ha='center', va='center', alpha=0.5)
        
        # 添加图例
        legend_handles = []
        for ctype, color in color_map.items():
            legend_handles.append(plt.Line2D([0], [0], marker='o', color='w', 
                                          markerfacecolor=color, markersize=10, 
                                          label=self.customer_types[ctype]['name']))
        legend_handles.append(plt.Line2D([0], [0], marker='o', color='w', 
                                      markerfacecolor='green', markersize=10, 
                                      label='分布式能源'))
        
        plt.legend(handles=legend_handles, loc='upper right')
        plt.title('配电网拓扑结构与客户类型分布')
        plt.axis('off')
        
        # 保存图像
        plt.savefig('配电网风险分析结果/网络拓扑图_客户分类.png', dpi=300)
        plt.close()
        
        # 绘制客户类型风险饼图
        plt.figure(figsize=(10, 8))
        labels = [self.customer_types[ctype]['name'] for ctype in self.customer_type_risks.keys()]
        sizes = [risk for risk in self.customer_type_risks.values()]
        colors = [color_map[ctype] for ctype in self.customer_type_risks.keys()]
        
        plt.pie(sizes, labels=labels, colors=colors, autopct='%1.1f%%', startangle=90)
        plt.title('不同客户类型风险分布')
        plt.axis('equal')
        
        # 保存图像
        plt.savefig('配电网风险分析结果/客户类型风险分布.png', dpi=300)
        plt.close()

# 示例使用代码
if __name__ == "__main__":
    # 加载节点数据
    nodes_data = pd.DataFrame({
        '有功P/kW': [40, 60, 60, 60, 100, 60, 60, 60, 120, 200, 150, 200, 60, 420, 210, 
                    120, 40, 100, 24, 60, 60, 60, 60, 40, 60, 40, 60, 100, 60, 100, 
                    120, 200, 150, 90, 40, 100, 90, 210, 90, 120, 60, 100, 40, 60,
                    120, 150, 200, 420, 420, 60, 420, 200, 200, 150, 200, 40, 120, 60, 45, 60, 90, 120]
    })

    # 加载线路数据
    lines_data = pd.DataFrame({
        '编号': list(range(1, 60)),
        '起点': [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 3, 20, 21, 4, 17, 18, 5, 14, 15, 
               23, 24, 25, 26, 27, 28, 25, 30, 31, 26, 33, 34, 24, 40, 41, 24, 36, 37, 38, 
               43, 44, 45, 46, 47, 44, 49, 50, 51, 45, 53, 54, 43, 56, 57, 58, 57, 60, 61],
        '终点': [2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 20, 21, 22, 17, 18, 19, 14, 15, 16, 
               24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 40, 41, 42, 36, 37, 38, 39, 
               44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62],
        '长度/km': [0.025, 0.05, 0.1, 0.1, 1.21, 1.7, 4.25, 1.21, 3.04, 3.04, 0.76, 1.84, 
                   0.76, 1.21, 0.55, 0.45, 0.24, 0.19, 0.57, 0.39, 0.39, 0.025, 0.05, 0.93, 
                   0.14, 0.22, 0.29, 1.45, 0.16, 0.21, 0.81, 0.28, 0.3, 0.48, 0.35, 0.43, 
                   0.05, 0.55, 0.4, 0.3, 0.05, 0.05, 1.57, 0.46, 0.24, 1.4, 0.28, 0.16, 
                   0.16, 0.9, 0.32, 0.17, 0.05, 2.35, 3.05, 1.45, 2.62, 1.65, 0.1],
        '电阻/Ω': [0.0031, 0.0063, 0.0125, 0.0125, 0.203, 0.2842, 0.3105, 0.203, 0.5075, 
                  0.5075, 0.1966, 0.164, 0.1966, 0.203, 0.0922, 0.0563, 0.03, 0.0238, 
                  0.0713, 0.0488, 0.0488, 0.0031, 0.0063, 0.1163, 0.0175, 0.0275, 0.0363, 
                  0.1813, 0.02, 0.0263, 0.1013, 0.035, 0.0375, 0.06, 0.0438, 0.0538, 
                  0.0063, 0.0688, 0.05, 0.0375, 0.0063, 0.0063, 0.1963, 0.0575, 0.03, 
                  0.175, 0.035, 0.02, 0.02, 0.1125, 0.04, 0.0213, 0.0063, 0.2938, 0.3813, 
                  0.1813, 0.3275, 0.2063, 0.0125],
        '电抗/Ω': [0.0021, 0.0042, 0.0085, 0.0085, 0.1034, 0.1447, 0.3619, 0.1034, 0.2585, 
                  0.2585, 0.065, 0.1565, 0.065, 0.1034, 0.047, 0.0383, 0.0204, 0.0161, 
                  0.0485, 0.0332, 0.0332, 0.0021, 0.0043, 0.0792, 0.0119, 0.0187, 0.0247, 
                  0.1234, 0.0136, 0.0179, 0.0689, 0.0238, 0.0255, 0.0409, 0.0298, 0.0366, 
                  0.0043, 0.0468, 0.034, 0.0255, 0.0043, 0.0043, 0.1337, 0.0391, 0.0204, 
                  0.1191, 0.0238, 0.0136, 0.0136, 0.0766, 0.0272, 0.0145, 0.0043, 0.2, 
                  0.2597, 0.1234, 0.223, 0.1405, 0.0085]
    })

    # 初始化风险评估模型
    risk_model = DistributionNetworkRiskEvaluator(nodes_data, lines_data)

    # 设置分布式能源数据
    dg_data = {
        1: {'node': 16, 'capacity': 300},
        2: {'node': 22, 'capacity': 300},
        3: {'node': 32, 'capacity': 300},
        4: {'node': 35, 'capacity': 300},
        5: {'node': 39, 'capacity': 300},
        6: {'node': 48, 'capacity': 300},
        7: {'node': 52, 'capacity': 300},
        8: {'node': 55, 'capacity': 300}
    }
    risk_model.set_distributed_generators(dg_data)

    # 计算系统总风险
    system_risk = risk_model.calculate_system_risk()
    print(f"系统总风险计算结果: {system_risk:.4f}")

    # 可视化网络拓扑
    risk_model.visualize_network()
    print("网络拓扑图已保存至'配电网风险分析结果/网络拓扑图_客户分类.png'")
    print("客户类型风险分布图已保存至'配电网风险分析结果/客户类型风险分布.png'")