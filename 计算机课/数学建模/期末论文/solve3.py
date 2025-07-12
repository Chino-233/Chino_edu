import pandas as pd
import numpy as np
from math import ceil
from datetime import datetime, timedelta

# 读取数据
df_lineinfo = pd.read_excel('D:\\Chino_edu\\数学建模\\期末论文\\rawData\\附件1.xlsx')
df_ownercount = pd.read_excel('D:\\Chino_edu\\数学建模\\期末论文\\rawData\\附件5.xlsx')
df_route = pd.read_excel('D:\\Chino_edu\\数学建模\\期末论文\\rawData\\附件4.xlsx')
df_pred = pd.read_excel('D:\\Chino_edu\\数学建模\\期末论文\\processedData\\结果表1_预测结果.xlsx')

# 预处理
df_lineinfo['发运时间'] = pd.to_datetime(df_lineinfo['发运节点'], format='%H:%M:%S')
df_lineinfo['线路日期'] = '2024/12/16'
df_pred['日期'] = pd.to_datetime(df_pred['日期']).dt.strftime('%Y/%m/%d')
df_lineinfo['线路编码'] = df_lineinfo['线路编码'].str.strip()
df_pred['线路编码'] = df_pred['线路编码'].str.strip()

# 添加预测货量
def get_predicted_volume(row):
    match = df_pred[(df_pred['线路编码'] == row['线路编码']) & (df_pred['日期'] == row['线路日期'])]
    return match['货量'].values[0] if not match.empty else 0
df_lineinfo['预测货量'] = df_lineinfo.apply(get_predicted_volume, axis=1)

# 可串点
route_pairs = set()
for _, row in df_route.iterrows():
    route_pairs.add((row['站点编号1'], row['站点编号2']))
    route_pairs.add((row['站点编号2'], row['站点编号1']))

# 容器决策
C_0 = 1000  # 非容器容量
C_1 = 800   # 容器容量

def container_decision(row):
    load = row['预测货量']
    time = row['发运时间'].hour
    if load <= C_1:
        return 1
    if row['车队编码'].startswith("Z") or time <= 6:
        return 1
    return 0

df_lineinfo['使用容器'] = df_lineinfo.apply(container_decision, axis=1)
df_lineinfo['单车容量'] = df_lineinfo['使用容器'].map(lambda x: C_1 if x == 1 else C_0)
df_lineinfo['车辆需求'] = df_lineinfo.apply(lambda r: ceil(r['预测货量'] / r['单车容量']), axis=1)

# 串点逻辑
def can_chain(row_i, row_j):
    if row_i['起始场地'] != row_j['起始场地']:
        return False
    if row_i['车队编码'] != row_j['车队编码']:
        return False
    if abs((row_i['发运时间'] - row_j['发运时间']).total_seconds()) > 1800:
        return False
    if row_i['目的场地'] != row_j['目的场地'] and (row_i['目的场地'], row_j['目的场地']) not in route_pairs:
        return False
    if row_i['使用容器'] != row_j['使用容器']:
        return False
    return True

def greedy_chain(df):
    used = set()
    chains = []
    df_sorted = df.sort_values(by='发运时间')

    for i, row_i in df_sorted.iterrows():
        if row_i['线路编码'] in used:
            continue
        group = [row_i]
        total_volume = row_i['预测货量']
        capacity = row_i['单车容量']
        used.add(row_i['线路编码'])
        for j, row_j in df_sorted.iterrows():
            if row_j['线路编码'] in used or row_j['线路编码'] == row_i['线路编码']:
                continue
            if can_chain(row_i, row_j):
                if total_volume + row_j['预测货量'] <= capacity:
                    group.append(row_j)
                    total_volume += row_j['预测货量']
                    used.add(row_j['线路编码'])
        chains.append(group)
    return chains

# 分配车辆
def assign_vehicles(chain_list, df_ownercount):
    vehicle_id = 0
    result = []

    owner_pool = {k: v for k, v in zip(df_ownercount['车队编码'], df_ownercount['自有车数量'])}
    chain_list.sort(key=lambda x: -len(x))

    for chain in chain_list:
        team = chain[0]['车队编码']
        container_flag = chain[0]['使用容器']
        total_volume = sum(r['预测货量'] for r in chain)
        capacity = chain[0]['单车容量']
        load_rate = total_volume / capacity

        if owner_pool.get(team, 0) > 0:
            vehicle_type = '自有'
            owner_pool[team] -= 1
        else:
            vehicle_type = '外部'

        vehicle_id += 1
        for route in chain:
            result.append({
                '线路编码': route['线路编码'],
                '日期': route['线路日期'],
                '预计发运时间': route['发运时间'].strftime('%H:%M:%S'),
                '是否使用容器': '是' if container_flag else '否',
                '发运车辆': f'{vehicle_type}-V{vehicle_id}',
                '车辆类型': vehicle_type,
                '装载率': round(load_rate, 3)
            })
    return pd.DataFrame(result)

# 改进的目标函数评估（包含详细成本分解）
def evaluate(df_result, df_lineinfo, df_ownercount):
    # 合并成本信息
    merged = df_result.merge(df_lineinfo[['线路编码', '自有变动成本', '外部承运商成本']], on='线路编码')
    
    # 计算变动成本
    own_variable_cost = merged[merged['车辆类型'] == '自有']['自有变动成本'].sum()
    external_cost = merged[merged['车辆类型'] == '外部']['外部承运商成本'].sum()
    total_variable_cost = own_variable_cost + external_cost
    
    # 计算固定成本（自有车日固定成本）
    total_own_vehicles = df_ownercount['自有车数量'].sum()
    fixed_cost = total_own_vehicles * 100  # 假设每辆自有车日固定成本100
    
    # 总成本
    total_cost = fixed_cost + total_variable_cost
    
    # 其他指标
    own_vehicles_used = df_result[df_result['车辆类型'] == '自有']['发运车辆'].nunique()
    external_vehicles_used = df_result[df_result['车辆类型'] == '外部']['发运车辆'].nunique()
    avg_load_rate = df_result['装载率'].mean()
    
    # 容器使用统计
    container_tasks = len(df_result[df_result['是否使用容器'] == '是'])
    total_tasks = len(df_result)
    container_usage_rate = container_tasks / total_tasks if total_tasks > 0 else 0
    
    # 自有车周转率
    own_turnover_rate = own_vehicles_used / total_own_vehicles if total_own_vehicles > 0 else 0
    
    # 权重计算综合指标
    w1, w2, w3 = 0.6, 0.3, 0.1
    Z = w1 * total_cost - w2 * own_vehicles_used - w3 * avg_load_rate

    return {
        'Z_综合指标': Z,
        'Z1_总成本': total_cost,
        'Z1a_固定成本': fixed_cost,
        'Z1b_自有变动成本': own_variable_cost,
        'Z1c_外部承运成本': external_cost,
        'Z2_自有车使用数': own_vehicles_used,
        'Z2a_外部车使用数': external_vehicles_used,
        'Z2b_自有车总数': total_own_vehicles,
        'Z2c_自有车周转率': round(own_turnover_rate, 3),
        'Z3_平均装载率': round(avg_load_rate, 3),
        'Z4_总任务数': total_tasks,
        'Z5_容器使用任务数': container_tasks,
        'Z5a_容器使用率': round(container_usage_rate, 3)
    }

# 主程序
def main():
    print("=== 问题三：容器技术优化调度 ===")
    print("开始执行调度算法...")
    
    chains = greedy_chain(df_lineinfo)
    df_result = assign_vehicles(chains, df_ownercount)
    
    # 计算评估指标
    metrics = evaluate(df_result, df_lineinfo, df_ownercount)
    
    # 保存调度结果（保持原路径）
    df_result.to_excel('D:\\Chino_edu\\数学建模\\期末论文\\processedData\\结果表4.xlsx', index=False)
    print("问题三调度结果已写入：结果表4.xlsx")
    
    print("\n=== 调度结果统计 ===")
    for key, value in metrics.items():
        print(f"{key}: {value}")
    
    # 成本分解展示
    print(f"\n=== 成本详细分解 ===")
    print(f"固定成本: {metrics['Z1a_固定成本']} (自有车 {metrics['Z2b_自有车总数']} 辆 × 100)")
    print(f"自有车变动成本: {metrics['Z1b_自有变动成本']}")
    print(f"外部承运成本: {metrics['Z1c_外部承运成本']}")
    print(f"总成本: {metrics['Z1_总成本']}")
    
    print(f"\n=== 车辆使用情况 ===")
    print(f"自有车使用: {metrics['Z2_自有车使用数']} / {metrics['Z2b_自有车总数']} (周转率: {metrics['Z2c_自有车周转率']:.1%})")
    print(f"外部车使用: {metrics['Z2a_外部车使用数']} 辆")
    print(f"平均装载率: {metrics['Z3_平均装载率']:.1%}")
    
    print(f"\n=== 容器技术使用情况 ===")
    print(f"使用容器任务: {metrics['Z5_容器使用任务数']} / {metrics['Z4_总任务数']} (使用率: {metrics['Z5a_容器使用率']:.1%})")

    # 输出指定线路（保持原格式）
    targets = ['场地3 - 站点83 - 0600', '场地3 - 站点83 - 1400']
    df_specific = df_result[df_result['线路编码'].isin(targets)]
    print("\n=== 指定线路调度结果 ===")
    if not df_specific.empty:
        print(df_specific[['线路编码', '日期', '预计发运时间', '是否使用容器', '发运车辆', '车辆类型', '装载率']].to_string(index=False))
    else:
        print("未找到指定线路的调度结果")
    
    # 保存详细指标到Excel
    metrics_df = pd.DataFrame([metrics])
    metrics_df.to_excel('D:\\Chino_edu\\数学建模\\期末论文\\processedData\\问题三_调度指标统计.xlsx', index=False)
    
    print(f"\n结果文件已保存:")
    print(f"  - 调度结果: 结果表4.xlsx")
    print(f"  - 指标统计: 问题三_调度指标统计.xlsx")

main()
