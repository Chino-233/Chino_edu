import pandas as pd
import numpy as np
from scipy import interpolate
import matplotlib.pyplot as plt
import os
import matplotlib as mpl

# 设置中文字体支持
mpl.rcParams['font.sans-serif'] = ['SimHei']
mpl.rcParams['axes.unicode_minus'] = False

# 读取CSV数据
file_path = r"d:\chino_edu\数学建模\第一次作业\梭子蟹_归一化.csv"
data = pd.read_csv(file_path)

# 创建保存图片和数据的目录
output_dir = r"d:\chino_edu\数学建模\第一次作业\插值结果"
data_output_dir = r"d:\chino_edu\数学建模\第一次作业\插值数据"
os.makedirs(output_dir, exist_ok=True)
os.makedirs(data_output_dir, exist_ok=True)

# 提取需要进行插值可视化的列
x_columns = ['AvgTN', '盐度1', 'pH1', '溶解氧', '导电率']
y_columns = ['蜕壳数', '死亡数']

# 绘制每一对x-y变量的插值图，这部分保持原来的功能
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

# 保存整体插值数据表格
# 获取所有数值列
numeric_columns = data.select_dtypes(include=[np.number]).columns.tolist()

# 选择一个基准列作为插值的参考（例如使用行索引作为参考）
reference_points = np.arange(len(data))
# 生成更密集的插值点
reference_new = np.linspace(0, len(data)-1, 500)  # 生成500个插值点

# 创建一个新的DataFrame用于存储插值后的数据
interpolated_df = pd.DataFrame()

# 对每一列单独进行插值
for col in numeric_columns:
    # 获取非缺失值的索引和数据
    valid_indices = data[col].notna()
    if valid_indices.sum() < 4:  # 至少需要4个点进行三次样条插值
        print(f"列 {col} 有效数据点不足，跳过插值")
        continue
        
    col_data = data.loc[valid_indices, col].values
    col_indices = reference_points[valid_indices]
    
    # 对数据点按索引排序
    sort_idx = np.argsort(col_indices)
    col_indices_sorted = col_indices[sort_idx]
    col_data_sorted = col_data[sort_idx]
    
    # 进行三次样条插值
    try:
        tck = interpolate.splrep(col_indices_sorted, col_data_sorted, s=0)
        col_new = interpolate.splev(reference_new, tck, der=0)
        interpolated_df[col] = col_new
    except Exception as e:
        print(f"列 {col} 插值失败: {str(e)}")

# 保存完整的插值数据表格
interpolated_df.to_csv(f"{data_output_dir}/完整插值数据.csv", index=False)
print(f"完整插值数据已保存至: {data_output_dir}/完整插值数据.csv")

print(f"所有图像已保存至 {output_dir} 目录")
print(f"所有插值数据已保存至 {data_output_dir} 目录")