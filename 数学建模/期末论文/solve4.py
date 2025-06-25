import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# 参数设置
C_0 = 1000  # 非容器容量
C_1 = 800   # 容器容量
COST_OWN = 500
COST_OUT = 800
delta_list = [-0.3, -0.275, -0.25, -0.225, -0.2, -0.175, -0.15, -0.125, -0.1, -0.075, -0.05, -0.025, 0, 0.025, 0.05, 0.075, 0.1, 0.125, 0.15, 0.175, 0.2, 0.225, 0.25, 0.275, 0.3]

# 读取数据
df_plan = pd.read_excel('D:\\Chino_edu\\数学建模\\期末论文\\processedData\\结果表4.xlsx')     # 问题三输出
df_pred = pd.read_excel('D:\\Chino_edu\\数学建模\\期末论文\\processedData\\结果表1_预测结果.xlsx')  # 原预测值
df_plan['是否使用容器'] = df_plan['是否使用容器'].map(lambda x: 1 if x == '是' else 0)
df_pred['线路编码'] = df_pred['线路编码'].str.strip()
df_plan['线路编码'] = df_plan['线路编码'].str.strip()

# 原始调度统计
vehicle_map = df_plan['发运车辆'].str[:2]  # "自有" or "外部"
origin_stats = {
    'own_count': (vehicle_map == '自有').sum(),
    'out_count': (vehicle_map == '外部').sum(),
    'total_cost': ((vehicle_map == '自有') * COST_OWN + (vehicle_map == '外部') * COST_OUT).sum(),
    'avg_load': None  # 稍后算
}

# 添加预测货量
df_plan = df_plan.merge(df_pred[['线路编码', '日期', '货量']], how='left', on=['线路编码', '日期'])
df_plan.rename(columns={'货量': '原预测货量'}, inplace=True)

# 添加容量
df_plan['单车容量'] = df_plan['是否使用容器'].map(lambda x: C_1 if x == 1 else C_0)

# 计算平均装载率（原始）
df_plan['装载率'] = df_plan['原预测货量'] / df_plan['单车容量']
origin_stats['avg_load'] = df_plan['装载率'].mean()


# 鲁棒性模拟函数
def simulate_under_delta(df_plan, delta):
    df_sim = df_plan.copy()
    df_sim['扰动后货量'] = df_sim['原预测货量'] * (1 + delta)

    # 判断是否发运失败（车装不下）
    df_sim['是否失败'] = df_sim['扰动后货量'] > df_sim['单车容量']

    # 成本重新计算（不变）
    df_sim['车类型'] = df_sim['发运车辆'].str[:2]
    df_sim['成本'] = df_sim['车类型'].map({'自有': COST_OWN, '外部': COST_OUT})

    # 实际装载率
    df_sim['扰动装载率'] = np.minimum(df_sim['扰动后货量'], df_sim['单车容量']) / df_sim['单车容量']

    # 统计指标
    total = len(df_sim)
    fail = df_sim['是否失败'].sum()
    own_now = (df_sim['车类型'] == '自有').sum()
    out_now = (df_sim['车类型'] == '外部').sum()
    cost_now = df_sim['成本'].sum()
    avg_load = df_sim['扰动装载率'].mean()

    # 指标计算
    R1 = fail / total
    R2 = (own_now - origin_stats['own_count']) / origin_stats['own_count']
    R3 = (out_now - origin_stats['out_count']) / origin_stats['out_count']
    R4 = avg_load - origin_stats['avg_load']
    R5 = (cost_now - origin_stats['total_cost']) / origin_stats['total_cost']

    return {
        '扰动系数': f"{int(delta*100)}%",
        '任务失败率 R1': round(R1, 4),
        '自有车使用变化率 R2': round(R2, 4),
        '外部车调用变化率 R3': round(R3, 4),
        '平均装载率变化 R4': round(R4, 4),
        '总成本变化率 R5': round(R5, 4)
    }

# 主程序
def main():
    result_rows = []
    for d in delta_list:
        result = simulate_under_delta(df_plan, d)
        result_rows.append(result)

    df_result = pd.DataFrame(result_rows)
    df_result.to_excel('D:\\Chino_edu\\数学建模\\期末论文\\processedData\\问题四_鲁棒性指标.xlsx', index=False)
    print("问题四评估完成，结果已保存到：问题四_鲁棒性指标.xlsx")
    print(df_result)

    # 可选可视化
    plt.rcParams['font.sans-serif'] = ['Microsoft YaHei']  # 或者 'SimHei'、'FangSong'
    plt.rcParams['axes.unicode_minus'] = False  # 正确显示负号
    plt.figure(figsize=(10, 6))
    for col in df_result.columns[1:]:
        plt.plot(df_result['扰动系数'], df_result[col], label=col, marker='o')
    plt.xlabel('扰动系数')
    plt.ylabel('指标变化值')
    plt.title('调度方案鲁棒性评估指标趋势')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig('D:\\Chino_edu\\数学建模\\期末论文\\processedData\\问题四_鲁棒性趋势图.jpg', dpi=500)
    print("趋势图已生成：问题四_鲁棒性趋势图.jpg")

main()
