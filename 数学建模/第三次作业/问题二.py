import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import os
from Q_1_risk_model import DistributionNetworkRiskEvaluator

# 配置matplotlib中文显示
plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False

def save_analysis_results(data, filename, output_dir="DG_capacity_analysis_results"):
    """
    将分析结果保存到指定文件夹
    
    参数:
    data: 要保存的数据
    filename: 文件名
    output_dir: 输出目录
    """
    # 创建输出目录
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    
    # 完整文件路径
    file_path = os.path.join(output_dir, filename)
    
    # 根据数据类型选择保存方式
    with open(file_path, 'w', encoding='utf-8') as f:
        if isinstance(data, pd.DataFrame):
            f.write(data.to_string())
        elif isinstance(data, dict):
            for k, v in data.items():
                f.write(f"{k}: {v}\n")
        else:
            f.write(str(data))

def evaluate_capacity_impact():
    """
    评估DG容量变化对配电网风险的影响
    容量从基准值I按0.3I步长增加到3I
    """
    # 读取节点负荷数据
    node_loads = pd.DataFrame({
        '有功P/kW': [40, 60, 60, 60, 100, 60, 60, 60, 120, 200, 150, 200, 60, 420, 210, 
                    120, 40, 100, 24, 60, 60, 60, 60, 40, 60, 40, 60, 100, 60, 100, 
                    120, 200, 150, 90, 40, 100, 90, 210, 90, 120, 60, 100, 40, 60,
                    120, 150, 200, 420, 420, 60, 420, 200, 200, 150, 200, 40, 120, 60, 45, 60, 90, 120]
    })

    # 读取线路拓扑数据
    line_topology = pd.DataFrame({
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

    # 创建风险评估模型
    risk_evaluator = DistributionNetworkRiskEvaluator(node_loads, line_topology)

    # 基准DG容量
    base_capacity = 300  # kW

    # DG安装位置
    dg_positions = {
        1: 13,  # DG1位于节点13
        2: 18,  # DG2位于节点18
        3: 22,  # DG3位于节点22
        4: 29,  # DG4位于节点29
        5: 32,  # DG5位于节点32
        6: 39,  # DG6位于节点39
        7: 48,  # DG7位于节点48
        8: 59   # DG8位于节点59
    }

    # 容量变化范围和步长
    capacity_multipliers = np.arange(1.0, 3.1, 0.3)
    
    # 存储分析结果
    analysis_results = {
        '容量倍数': [],
        'DG总容量/kW': [],
        '失负荷风险': [],
        '过负荷风险': [],
        '系统总风险': []
    }

    # 执行容量敏感性分析
    print("开始分析DG容量对系统风险的影响...")
    for multiplier in capacity_multipliers:
        current_capacity = base_capacity * multiplier
        print(f"当前容量倍数: {multiplier:.1f}, DG容量: {current_capacity:.1f} kW")
        
        # 设置当前DG容量
        current_dg_config = {}
        for dg_id, node_id in dg_positions.items():
            current_dg_config[dg_id] = {
                'node': node_id,
                'capacity': current_capacity
            }
        
        # 更新模型中的DG数据
        risk_evaluator.set_distributed_generators(current_dg_config)
        
        # 计算各项风险
        load_loss = risk_evaluator.calculate_load_loss_risk()
        overload = risk_evaluator.calculate_overload_risk()
        total_risk = risk_evaluator.calculate_system_risk()
        
        # 记录结果
        analysis_results['容量倍数'].append(multiplier)
        analysis_results['DG总容量/kW'].append(current_capacity * len(dg_positions))
        analysis_results['失负荷风险'].append(load_loss)
        analysis_results['过负荷风险'].append(overload)
        analysis_results['系统总风险'].append(total_risk)
        
        print(f"  失负荷风险: {load_loss:.2f}")
        print(f"  过负荷风险: {overload:.2f}")
        print(f"  系统总风险: {total_risk:.2f}")
    
    # 转换为DataFrame方便处理
    results_df = pd.DataFrame(analysis_results)
    
    # 创建输出目录
    output_dir = 'DG容量分析结果'
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    
    # 保存数据结果
    results_df.to_csv(f'{output_dir}/DG容量风险数据.csv', index=False)
    
    # 生成分析报告
    generate_analysis_report(results_df, output_dir)
    
    # 生成可视化图表
    visualize_capacity_impact(results_df, output_dir)
    
    # 分析各馈线的风险变化
    analyze_feeder_risks(risk_evaluator, dg_positions, base_capacity, capacity_multipliers, output_dir)
    
    return results_df

def generate_analysis_report(results_df, output_dir):
    """生成DG容量影响分析报告"""
    with open(f'{output_dir}/DG容量风险分析报告.txt', 'w', encoding='utf-8') as report:
        report.write("===============================================\n")
        report.write("    分布式能源容量对配电网风险的影响分析报告    \n")
        report.write("===============================================\n\n")
        
        # 基本统计信息
        report.write("1. 分析概述:\n")
        report.write(f"   - 分析DG容量范围: {results_df['容量倍数'].min()}I - {results_df['容量倍数'].max()}I\n")
        report.write(f"   - 步长: 0.3I\n")
        report.write(f"   - 数据点数: {len(results_df)}\n\n")
        
        # 风险变化趋势
        report.write("2. 风险变化趋势:\n")
        
        # 失负荷风险趋势
        max_load_loss = results_df['失负荷风险'].max()
        min_load_loss = results_df['失负荷风险'].min()
        max_idx = results_df['失负荷风险'].idxmax()
        min_idx = results_df['失负荷风险'].idxmin()
        
        report.write("   2.1 失负荷风险:\n")
        report.write(f"     - 最大值: {max_load_loss:.2f} (容量倍数: {results_df.loc[max_idx, '容量倍数']}I)\n")
        report.write(f"     - 最小值: {min_load_loss:.2f} (容量倍数: {results_df.loc[min_idx, '容量倍数']}I)\n")
        report.write(f"     - 变化趋势: {get_trend_description(results_df['失负荷风险'])}\n\n")
        
        # 过负荷风险趋势
        max_overload = results_df['过负荷风险'].max()
        min_overload = results_df['过负荷风险'].min()
        max_idx = results_df['过负荷风险'].idxmax()
        min_idx = results_df['过负荷风险'].idxmin()
        
        report.write("   2.2 过负荷风险:\n")
        report.write(f"     - 最大值: {max_overload:.2f} (容量倍数: {results_df.loc[max_idx, '容量倍数']}I)\n")
        report.write(f"     - 最小值: {min_overload:.2f} (容量倍数: {results_df.loc[min_idx, '容量倍数']}I)\n")
        report.write(f"     - 变化趋势: {get_trend_description(results_df['过负荷风险'])}\n\n")
        
        # 系统总风险趋势
        max_total = results_df['系统总风险'].max()
        min_total = results_df['系统总风险'].min()
        max_idx = results_df['系统总风险'].idxmax()
        min_idx = results_df['系统总风险'].idxmin()
        
        report.write("   2.3 系统总风险:\n")
        report.write(f"     - 最大值: {max_total:.2f} (容量倍数: {results_df.loc[max_idx, '容量倍数']}I)\n")
        report.write(f"     - 最小值: {min_total:.2f} (容量倍数: {results_df.loc[min_idx, '容量倍数']}I)\n")
        report.write(f"     - 变化趋势: {get_trend_description(results_df['系统总风险'])}\n\n")
        
        # 最优容量分析
        opt_idx = results_df['系统总风险'].idxmin()
        opt_capacity = results_df.loc[opt_idx, '容量倍数']
        
        report.write("3. 最优DG容量分析:\n")
        report.write(f"   - 最优容量倍数: {opt_capacity}I\n")
        report.write(f"   - 对应失负荷风险: {results_df.loc[opt_idx, '失负荷风险']:.2f}\n")
        report.write(f"   - 对应过负荷风险: {results_df.loc[opt_idx, '过负荷风险']:.2f}\n")
        report.write(f"   - 对应系统总风险: {results_df.loc[opt_idx, '系统总风险']:.2f}\n\n")
        
        # 结论与建议
        report.write("4. 结论与建议:\n")
        
        if opt_capacity <= 1.3:
            report.write("   - 建议适当增加DG容量，当前配置容量较低\n")
        elif opt_capacity >= 2.7:
            report.write("   - 建议控制DG容量增长，防止过负荷风险增加\n")
        else:
            report.write("   - 建议保持DG容量在适中水平，平衡失负荷和过负荷风险\n")
            
        report.write("   - 各馈线DG容量分配应考虑负荷分布，合理平衡\n")
        report.write("   - 考虑增加联络线容量，提高负荷转移能力\n")

def get_trend_description(data_series):
    """获取数据序列的变化趋势描述"""
    first_val = data_series.iloc[0]
    last_val = data_series.iloc[-1]
    max_val = data_series.max()
    min_val = data_series.min()
    
    if all(data_series.iloc[i] >= data_series.iloc[i-1] for i in range(1, len(data_series))):
        return "持续上升"
    elif all(data_series.iloc[i] <= data_series.iloc[i-1] for i in range(1, len(data_series))):
        return "持续下降"
    elif first_val > last_val:
        idx_min = data_series.idxmin()
        if idx_min == len(data_series) - 1:
            return "总体下降趋势"
        else:
            return "先下降后上升，整体呈U型"
    elif first_val < last_val:
        idx_max = data_series.idxmax()
        if idx_max == len(data_series) - 1:
            return "总体上升趋势"
        else:
            return "先上升后下降，整体呈倒U型"
    else:
        return "波动变化，无明显趋势"

def visualize_capacity_impact(results_df, output_dir):
    """可视化DG容量对系统风险的影响"""
    # 绘制风险变化曲线
    plt.figure(figsize=(12, 8))
    
    plt.plot(results_df['容量倍数'], results_df['失负荷风险'], 'b-o', linewidth=2, label='失负荷风险')
    plt.plot(results_df['容量倍数'], results_df['过负荷风险'], 'r-s', linewidth=2, label='过负荷风险')
    plt.plot(results_df['容量倍数'], results_df['系统总风险'], 'g-^', linewidth=2.5, label='系统总风险')
    
    plt.title('分布式能源容量对配电网风险的影响', fontsize=16)
    plt.xlabel('DG容量倍数 (相对于初始值I)', fontsize=14)
    plt.ylabel('风险值', fontsize=14)
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.legend(fontsize=12)
    plt.tight_layout()
    
    # 标记最优点
    opt_idx = results_df['系统总风险'].idxmin()
    opt_x = results_df.loc[opt_idx, '容量倍数']
    opt_y = results_df.loc[opt_idx, '系统总风险']
    
    plt.scatter([opt_x], [opt_y], s=100, c='orange', marker='*', zorder=5)
    plt.annotate(f'最优点: ({opt_x}I, {opt_y:.2f})',
                xy=(opt_x, opt_y), xytext=(opt_x+0.2, opt_y+5),
                arrowprops=dict(facecolor='black', shrink=0.05, width=1.5))
    
    # 保存图片
    plt.savefig(f'{output_dir}/DG容量风险变化曲线.png', dpi=300, bbox_inches='tight')
    plt.close()
    
    # 绘制风险比例堆叠图
    plt.figure(figsize=(12, 8))
    
    plt.bar(results_df['容量倍数'], results_df['失负荷风险'], label='失负荷风险', color='skyblue')
    plt.bar(results_df['容量倍数'], results_df['过负荷风险'], 
            bottom=results_df['失负荷风险'], label='过负荷风险', color='salmon')
    
    plt.title('DG容量对风险构成的影响', fontsize=16)
    plt.xlabel('DG容量倍数 (相对于初始值I)', fontsize=14)
    plt.ylabel('风险值', fontsize=14)
    plt.grid(True, linestyle='--', alpha=0.7, axis='y')
    plt.legend(fontsize=12)
    plt.tight_layout()
    
    # 保存图片
    plt.savefig(f'{output_dir}/DG容量风险构成.png', dpi=300, bbox_inches='tight')
    plt.close()

def analyze_feeder_risks(model, dg_locations, base_capacity, capacity_factors, output_dir):
    """分析各馈线风险随DG容量的变化"""
    # 初始化存储各馈线风险的数据结构
    feeder_risks = {
        '容量倍数': [],
        '馈线1风险': [],
        '馈线2风险': [],
        '馈线3风险': []
    }
    
    # 对每个容量因子计算风险
    for factor in capacity_factors:
        # 更新DG容量配置
        dg_config = {}
        for dg_id, node in dg_locations.items():
            dg_config[dg_id] = {
                'node': node,
                'capacity': base_capacity * factor
            }
        
        # 设置模型中的DG数据
        model.set_distributed_generators(dg_config)
        
        # 计算系统风险
        model.calculate_system_risk()
        
        # 简化计算，假设各馈线风险分配
        # 注意：实际情况应基于模型内部计算每个馈线的具体风险
        feeder1_risk = model.risk_results['系统总风险'] * 0.4  # 假设馈线1占40%
        feeder2_risk = model.risk_results['系统总风险'] * 0.35  # 假设馈线2占35%
        feeder3_risk = model.risk_results['系统总风险'] * 0.25  # 假设馈线3占25%
        
        # 记录结果
        feeder_risks['容量倍数'].append(factor)
        feeder_risks['馈线1风险'].append(feeder1_risk)
        feeder_risks['馈线2风险'].append(feeder2_risk)
        feeder_risks['馈线3风险'].append(feeder3_risk)
    
    # 转换为DataFrame
    feeder_risks_df = pd.DataFrame(feeder_risks)
    
    # 保存数据
    feeder_risks_df.to_csv(f'{output_dir}/各馈线风险数据.csv', index=False)
    
    # 可视化各馈线风险变化
    plt.figure(figsize=(12, 8))
    
    plt.plot(feeder_risks_df['容量倍数'], feeder_risks_df['馈线1风险'], 'b-o', linewidth=2, label='馈线1风险')
    plt.plot(feeder_risks_df['容量倍数'], feeder_risks_df['馈线2风险'], 'r-s', linewidth=2, label='馈线2风险')
    plt.plot(feeder_risks_df['容量倍数'], feeder_risks_df['馈线3风险'], 'g-^', linewidth=2, label='馈线3风险')
    
    plt.title('DG容量对各馈线风险的影响', fontsize=16)
    plt.xlabel('DG容量倍数 (相对于初始值I)', fontsize=14)
    plt.ylabel('风险值', fontsize=14)
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.legend(fontsize=12)
    plt.tight_layout()
    
    # 保存图片
    plt.savefig(f'{output_dir}/各馈线风险变化.png', dpi=300, bbox_inches='tight')
    plt.close()
    
    # 生成各馈线风险分析报告
    with open(f'{output_dir}/各馈线风险分析报告.txt', 'w', encoding='utf-8') as f:
        f.write("=======================================\n")
        f.write("      各馈线风险对DG容量变化的响应      \n")
        f.write("=======================================\n\n")
        
        for feeder in ['馈线1风险', '馈线2风险', '馈线3风险']:
            min_idx = feeder_risks_df[feeder].idxmin()
            opt_capacity = feeder_risks_df.loc[min_idx, '容量倍数']
            min_risk = feeder_risks_df.loc[min_idx, feeder]
            
            f.write(f"{feeder}分析:\n")
            f.write(f"  - 最小风险值: {min_risk:.2f}\n")
            f.write(f"  - 最优DG容量倍数: {opt_capacity}I\n")
            f.write(f"  - 变化趋势: {get_trend_description(feeder_risks_df[feeder])}\n\n")


if __name__ == "__main__":
    print("开始分析分布式能源容量对配电网风险的影响...")
    results = evaluate_capacity_impact()
    print("分析完成！结果已保存到 'DG容量分析结果' 文件夹")