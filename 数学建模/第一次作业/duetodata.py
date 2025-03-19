import pandas as pd
import numpy as np
from scipy import interpolate
import matplotlib.pyplot as plt
import os
import matplotlib as mpl

# 设置中文字体支持
mpl.rcParams['font.sans-serif'] = ['SimHei']  # 指定默认字体为黑体
mpl.rcParams['axes.unicode_minus'] = False  # 解决保存图像时负号'-'显示为方块的问题

# 读取CSV数据
file_path = r"d:\chino_edu\数学建模\第一次作业\梭子蟹_归一化.csv"
data = pd.read_csv(file_path)

# 提取需要的列作为自变量和因变量
x_columns = ['AvgTN', '盐度1', 'pH1', '溶解氧', '导电率']
y_columns = ['蜕壳数', '死亡数']

# 创建保存图片的目录
output_dir = r"d:\chino_edu\数学建模\第一次作业\插值结果"
os.makedirs(output_dir, exist_ok=True)

# 对每个x列和y列的组合进行三次样条插值并绘图
for x_col in x_columns:
    for y_col in y_columns:
        # 筛选有效数据点（两列都有值的行）
        valid_data = data[[x_col, y_col]].dropna()
        
        # 检查有效数据点是否足够进行三次样条插值
        if len(valid_data) < 4:  # 三次样条插值至少需要4个点
            print(f"列 {x_col} 和 {y_col} 的有效数据点不足，无法进行插值")
            continue
            
        x = valid_data[x_col].values
        y = valid_data[y_col].values
        
        # 对数据点按x值排序
        sort_indices = np.argsort(x)
        x_sorted = x[sort_indices]
        y_sorted = y[sort_indices]
        
        # 进行三次样条插值
        tck = interpolate.splrep(x_sorted, y_sorted, s=0)
        
        # 生成更密集的点用于绘制平滑曲线
        x_new = np.linspace(min(x), max(x), 1000)
        y_new = interpolate.splev(x_new, tck, der=0)
        
        # 绘制原始数据点和插值曲线
        plt.figure(figsize=(10, 6))
        plt.scatter(x, y, c='r', marker='o', s=30, label='原始数据')
        plt.plot(x_new, y_new, 'b-', linewidth=2, label='三次样条插值')
        plt.xlabel(x_col, fontsize=12)
        plt.ylabel(y_col, fontsize=12)
        plt.title(f"{x_col} 与 {y_col} 的三次样条插值关系图", fontsize=14)
        plt.legend(fontsize=10)
        plt.grid(True, linestyle='--', alpha=0.7)
        
        # 添加相关系数标注
        corr = np.corrcoef(x, y)[0, 1]
        plt.annotate(f"相关系数: {corr:.4f}", 
                    xy=(0.05, 0.95), 
                    xycoords='axes fraction',
                    bbox=dict(boxstyle="round,pad=0.3", fc="white", ec="gray", alpha=0.8))
        
        # 保存图像（不显示）
        plt.savefig(f"{output_dir}/{x_col}_vs_{y_col}.png", dpi=300, bbox_inches='tight')
        plt.close()

print(f"所有图像已保存至 {output_dir} 目录")