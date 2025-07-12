import pandas as pd
import numpy as np
from scipy import interpolate
from scipy import stats
import matplotlib.pyplot as plt
import os
import matplotlib as mpl

# 设置中文字体支持
mpl.rcParams['font.sans-serif'] = ['SimHei']
mpl.rcParams['axes.unicode_minus'] = False

# 读取CSV数据
file_path = r"d:\chino_edu\数学建模\第一次作业\梭子蟹_归一化.csv"
data = pd.read_csv(file_path)

# 筛选9月11日及以后的数据
# 创建一个函数来判断日期是否在9月11日或之后
def is_after_sept_11(date_str):
    # 检查是否为非字符串值（如NaN或浮点数）
    if not isinstance(date_str, str):
        return False
    
    try:
        # 处理月份和日期
        month = int(date_str.split('月')[0])
        day = int(date_str.split('月')[1].replace('日', ''))
        
        # 如果月份大于9，或者是9月份但日期大于等于11
        return (month > 9) or (month == 9 and day >= 11)
    except (IndexError, ValueError, AttributeError):
        # 如果解析失败，则此日期不符合条件
        return False

# 应用过滤条件
data = data[data['日期'].apply(is_after_sept_11)]
print(f"筛选后数据从9月11日开始，共 {len(data)} 行")

# 创建保存图片和数据的目录
output_dir = r"d:\chino_edu\数学建模\第一次作业\插值结果_9月11日后"
corr_output_dir = r"d:\chino_edu\数学建模\第一次作业\相关性图_9月11日后"
data_output_dir = r"d:\chino_edu\数学建模\第一次作业\插值数据_9月11日后"
os.makedirs(output_dir, exist_ok=True)
os.makedirs(corr_output_dir, exist_ok=True)
os.makedirs(data_output_dir, exist_ok=True)

# 提取需要进行插值可视化的列
x_columns = ['AvgTN', '盐度1', 'pH1', '溶解氧', '导电率']
y_columns = ['蜕壳数', '死亡数']

# 绘制每一对x-y变量的插值图
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
        plt.title(f"{x_col} 与 {y_col} 的三次样条插值关系图 (9月11日后)", fontsize=14)
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

# 复制原始数据框创建新的DataFrame用于存储插值后的数据
filled_data = data.copy()

# 获取所有数值列
numeric_columns = data.select_dtypes(include=[np.number]).columns.tolist()

# 对每一列单独进行插值处理
for col in numeric_columns:
    # 检查列是否有缺失值
    if data[col].isna().any():
        # 获取非缺失值的索引和数据
        valid_indices = data[col].notna()
        if valid_indices.sum() < 4:  # 三次样条插值至少需要4个点
            print(f"列 {col} 有效数据点不足，无法进行插值")
            continue
        
        # 获取行索引和对应的非缺失值
        x_valid = np.where(valid_indices)[0]  # 有效数据的行索引
        y_valid = data.loc[valid_indices, col].values  # 有效数据值
        
        # 获取缺失值的索引
        x_missing = np.where(~valid_indices)[0]  # 缺失数据的行索引
        
        if len(x_missing) > 0:  # 如果有缺失值需要插值
            try:
                # 进行三次样条插值
                tck = interpolate.splrep(x_valid, y_valid, s=0)
                
                # 对缺失值进行插值
                y_interpolated = interpolate.splev(x_missing, tck, der=0)
                
                # 填充缺失值
                filled_data.loc[~valid_indices, col] = y_interpolated
                print(f"列 '{col}' 的 {len(x_missing)} 个缺失值已通过插值填充")
                
                # 处理可能出现的负值
                if col in ['蜕壳数', '死亡数', '溶解氧', '导电率', '盐度1']:
                    neg_count = (filled_data[col] < 0).sum()
                    if neg_count > 0:
                        filled_data.loc[filled_data[col] < 0, col] = 0
                        print(f"列 '{col}' 中 {neg_count} 个负值已设置为0")
            except Exception as e:
                print(f"列 {col} 插值失败: {str(e)}")
        else:
            print(f"列 '{col}' 没有缺失值")

# 保存填充缺失值后的数据表格
filled_data.to_csv(f"{data_output_dir}/9月11日后缺失值插值填充数据.csv", index=False)
print(f"9月11日后保留时间结构的插值数据已保存至: {data_output_dir}/9月11日后缺失值插值填充数据.csv")

# ---------- 新增代码：对插值后的数据绘制相关性图和拟合曲线 ----------

# 对插值填充后的数据进行相关性分析和绘图
for x_col in x_columns:
    for y_col in y_columns:
        # 获取插值后的完整数据
        x = filled_data[x_col].values
        y = filled_data[y_col].values
        
        # 计算相关系数
        pearson_corr, p_value = stats.pearsonr(x, y)
        
        # 创建散点图
        plt.figure(figsize=(10, 6))
        plt.scatter(x, y, c='royalblue', marker='o', s=40, alpha=0.7, label='插值后数据')
        
        # 添加线性拟合
        slope, intercept, r_value, p_value, std_err = stats.linregress(x, y)
        line_x = np.linspace(min(x), max(x), 100)
        line_y = slope * line_x + intercept
        plt.plot(line_x, line_y, 'r-', linewidth=2, label=f'线性拟合: y = {slope:.4f}x + {intercept:.4f}')
        
        # 添加多项式拟合（三次多项式）
        poly_coef = np.polyfit(x, y, 3)
        poly_fn = np.poly1d(poly_coef)
        poly_x = np.linspace(min(x), max(x), 100)
        poly_y = poly_fn(poly_x)
        plt.plot(poly_x, poly_y, 'g--', linewidth=2, label='三次多项式拟合')
        
        # 设置图表标题和标签
        plt.title(f"{x_col} 与 {y_col} 的相关性分析 (9月11日后插值数据)", fontsize=14)
        plt.xlabel(x_col, fontsize=12)
        plt.ylabel(y_col, fontsize=12)
        plt.grid(True, linestyle='--', alpha=0.3)
        
        # 添加相关系数标注
        annotation_text = f"Pearson相关系数: {pearson_corr:.4f}\n"
        annotation_text += f"p值: {p_value:.4g}\n"
        annotation_text += f"线性拟合R²: {r_value**2:.4f}"
        
        plt.annotate(annotation_text, 
                    xy=(0.05, 0.95), 
                    xycoords='axes fraction',
                    bbox=dict(boxstyle="round,pad=0.5", fc="white", ec="gray", alpha=0.8),
                    fontsize=10,
                    verticalalignment='top')
        
        plt.legend(loc='best', fontsize=10)
        
        # 保存图像
        plt.savefig(f"{corr_output_dir}/{x_col}_vs_{y_col}_相关性.png", dpi=300, bbox_inches='tight')
        plt.close()

print(f"所有图像已保存至 {output_dir} 和 {corr_output_dir} 目录")
print(f"所有插值数据已保存至 {data_output_dir} 目录")

# 生成并绘制相关系数热力图
plt.figure(figsize=(12, 10))
numeric_data = filled_data[x_columns + y_columns]
corr_matrix = numeric_data.corr()
im = plt.imshow(corr_matrix, cmap='coolwarm', interpolation='none')

# 添加相关系数标签
for i in range(len(corr_matrix.columns)):
    for j in range(len(corr_matrix.columns)):
        text = plt.text(j, i, f"{corr_matrix.iloc[i, j]:.2f}",
                       ha="center", va="center", color="black" if abs(corr_matrix.iloc[i, j]) < 0.7 else "white")

plt.colorbar(im, label='相关系数')
plt.xticks(range(len(corr_matrix.columns)), corr_matrix.columns, rotation=45, ha='right')
plt.yticks(range(len(corr_matrix.columns)), corr_matrix.columns)
plt.title('环境因子与生物指标的相关性热力图', fontsize=16)
plt.tight_layout()
plt.savefig(f"{corr_output_dir}/相关性热力图.png", dpi=300, bbox_inches='tight')
plt.close()