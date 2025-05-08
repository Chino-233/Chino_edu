import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.ticker as ticker
from pathlib import Path

# 设置matplotlib中文显示
plt.rcParams['font.sans-serif'] = ['SimHei']  # 用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False    # 用来正常显示负号

def format_sci_notation(x, pos):
    """格式化科学计数法显示"""
    if x == 0 or np.isnan(x) or np.isinf(x) or x < 0:
        return "0"
    
    # 避免非常小的值导致log10计算问题
    if x < 1e-10:
        return "0"
    
    try:
        exponent = int(np.log10(x))
        coeff = x / 10**exponent
        
        if abs(coeff - 1.0) < 1e-10:
            return r'$10^{%d}$' % exponent
        elif abs(coeff - 5.0) < 1e-10:
            return r'$5{\times}10^{%d}$' % exponent
        else:
            return r'$%d{\times}10^{%d}$' % (round(coeff), exponent)
    except (ValueError, OverflowError):
        # 捕获任何数学错误并返回安全值
        return "0"
        
def load_data():
    """加载分治和暴力算法的数据"""
    # 读取CSV文件
    divide_conquer_df = pd.read_csv('d:/Chino_edu/temp/divide_conquer_benchmark_results.csv')
    brute_force_df = pd.read_csv('d:/Chino_edu/temp/brute_force_benchmark_results.csv')
    
    # 筛选出平均值行
    dc_avg = divide_conquer_df[divide_conquer_df['Test'] == '平均']
    bf_avg = brute_force_df[brute_force_df['Test'] == '平均']
    
    return dc_avg, bf_avg

def create_comparison_plot(dc_data, bf_data, output_dir, log_scale=True):
    """创建分治和暴力算法的性能比较图"""
    plt.figure(figsize=(12, 8))
    
    # 提取数据
    dc_sizes = dc_data['N'].values
    dc_times = dc_data['DivideConquerTime(ms)'].values
    
    bf_sizes = bf_data['N'].values
    bf_times = bf_data['BruteForceTime(ms)'].values
    
    # 绘制线图
    plt.plot(dc_sizes, dc_times, 'o-', linewidth=2, color='#1f77b4', 
             label="分治算法", markersize=8, alpha=0.8)
    plt.plot(bf_sizes, bf_times, 's-', linewidth=2, color='#ff7f0e', 
             label="暴力算法", markersize=8, alpha=0.8)
    
    # 设置标题和坐标轴标签
    title_suffix = " (对数坐标)" if log_scale else ""
    plt.title(f"最近点对算法性能比较{title_suffix}", fontsize=16)
    plt.xlabel("数据规模 (n)", fontsize=14)
    plt.ylabel("平均运行时间 (毫秒)", fontsize=14)
    
    # 设置x轴格式为科学计数法
    formatter = ticker.FuncFormatter(format_sci_notation)
    plt.gca().xaxis.set_major_formatter(formatter)
    
    if log_scale:
        plt.xscale('log')
        plt.yscale('log')
        plt.grid(True, which='major', linestyle='-', alpha=0.7)
        plt.grid(False, which='minor')
    else:
        plt.grid(True, linestyle='--', alpha=0.7)
    
    plt.legend(fontsize=12, loc='best')
    plt.tight_layout()
    
    # 保存图表
    output_path = f"{output_dir}/最近点对算法性能比较{'_log' if log_scale else '_linear'}.png"
    plt.savefig(output_path, dpi=300)
    print(f"已保存图表: {output_path}")
    plt.close()

def create_min_distance_plot(dc_data, bf_data, output_dir, log_scale=True):
    """创建不同规模数据下的最小距离比较图"""
    plt.figure(figsize=(12, 8))
    
    # 提取数据
    dc_sizes = dc_data['N'].values
    dc_min_distances = dc_data['MinDistance'].values
    
    bf_sizes = bf_data['N'].values
    bf_min_distances = bf_data['MinDistance'].values
    
    # 绘制线图
    plt.plot(dc_sizes, dc_min_distances, 'o-', linewidth=2, color='#1f77b4', 
             label="分治算法", markersize=8, alpha=0.8)
    plt.plot(bf_sizes, bf_min_distances, 's-', linewidth=2, color='#ff7f0e', 
             label="暴力算法", markersize=8, alpha=0.8)
    
    # 设置标题和坐标轴标签
    title_suffix = " (对数坐标)" if log_scale else ""
    plt.title(f"不同规模数据下的最小距离比较{title_suffix}", fontsize=16)
    plt.xlabel("数据规模 (n)", fontsize=14)
    plt.ylabel("最小距离", fontsize=14)
    
    # 设置x轴格式为科学计数法
    formatter = ticker.FuncFormatter(format_sci_notation)
    plt.gca().xaxis.set_major_formatter(formatter)
    
    if log_scale:
        plt.xscale('log')
        plt.yscale('log')
        plt.grid(True, which='major', linestyle='-', alpha=0.7)
        plt.grid(False, which='minor')
    else:
        plt.grid(True, linestyle='--', alpha=0.7)
    
    plt.legend(fontsize=12, loc='best')
    plt.tight_layout()
    
    # 保存图表
    output_path = f"{output_dir}/最小距离比较{'_log' if log_scale else '_linear'}.png"
    plt.savefig(output_path, dpi=300)
    print(f"已保存图表: {output_path}")
    plt.close()

def create_theoretical_comparison(dc_data, bf_data, output_dir, log_scale=True):
    """创建理论时间复杂度与实际运行时间的对比图"""
    plt.figure(figsize=(12, 8))
    
    # 提取数据
    dc_sizes = dc_data['N'].values
    dc_times = dc_data['DivideConquerTime(ms)'].values
    
    bf_sizes = bf_data['N'].values
    bf_times = bf_data['BruteForceTime(ms)'].values
    
    # 设置基准点规模
    dc_benchmark_size = 100000  # 分治算法仍使用100000作为基准
    bf_benchmark_size = 100000   # 蛮力法改用1000作为基准
    
    # 寻找最接近基准规模的数据点
    dc_benchmark_idx = np.argmin(np.abs(dc_sizes - dc_benchmark_size))
    bf_benchmark_idx = np.argmin(np.abs(bf_sizes - bf_benchmark_size))
    
    dc_benchmark_size = dc_sizes[dc_benchmark_idx]
    dc_benchmark_time = dc_times[dc_benchmark_idx]
    
    bf_benchmark_size = bf_sizes[bf_benchmark_idx]
    bf_benchmark_time = bf_times[bf_benchmark_idx]
    
    print(f"分治算法基准点: 规模={dc_benchmark_size}, 时间={dc_benchmark_time}毫秒")
    print(f"蛮力法基准点: 规模={bf_benchmark_size}, 时间={bf_benchmark_time}毫秒")
    
    # 计算缩放因子，使理论时间在benchmark_size处与实际时间匹配
    dc_scale = dc_benchmark_time / (dc_benchmark_size * np.log(dc_benchmark_size))
    bf_scale = bf_benchmark_time / (bf_benchmark_size * bf_benchmark_size)
    
    # 计算理论时间
    dc_theoretical = dc_scale * (dc_sizes * np.log(dc_sizes))
    bf_theoretical = bf_scale * (bf_sizes * bf_sizes)
    
    # 绘制分治算法
    plt.plot(dc_sizes, dc_times, 'o-', linewidth=2, color='#1f77b4', 
             label="分治算法实际运行时间", markersize=8, alpha=0.8)
    plt.plot(dc_sizes, dc_theoretical, 'o--', linewidth=2, color='#1f77b4', 
             label="分治算法理论时间 O(n log n)", markersize=0, alpha=0.5)
    
    # 绘制蛮力法
    plt.plot(bf_sizes, bf_times, 's-', linewidth=2, color='#ff7f0e', 
             label="蛮力法实际运行时间", markersize=8, alpha=0.8)
    plt.plot(bf_sizes, bf_theoretical, 's--', linewidth=2, color='#ff7f0e', 
             label="蛮力法理论时间 O(n²)", markersize=0, alpha=0.5)
    
    # 标记基准点
    plt.scatter([dc_benchmark_size], [dc_benchmark_time], color='#1f77b4', s=100, 
                marker='*', edgecolors='black', zorder=10, label="分治算法基准点")
    plt.scatter([bf_benchmark_size], [bf_benchmark_time], color='#ff7f0e', s=100,
                marker='*', edgecolors='black', zorder=10, label="蛮力法基准点")
    
    # 设置标题和坐标轴标签
    title_suffix = " (对数坐标)" if log_scale else ""
    plt.title(f"理论时间复杂度与实际运行时间对比{title_suffix}", fontsize=16)
    plt.xlabel("数据规模 (n)", fontsize=14)
    plt.ylabel("时间 (毫秒)", fontsize=14)
    
    # 设置x轴格式为科学计数法
    formatter = ticker.FuncFormatter(format_sci_notation)
    plt.gca().xaxis.set_major_formatter(formatter)
    
    if log_scale:
        plt.xscale('log')
        plt.yscale('log')
        plt.grid(True, which='major', linestyle='-', alpha=0.7)
        plt.grid(False, which='minor')
    else:
        plt.grid(True, linestyle='--', alpha=0.7)
    
    plt.legend(fontsize=12, loc='best')
    plt.tight_layout()
    
    # 保存图表
    output_path = f"{output_dir}/理论与实际时间对比{'_log' if log_scale else '_linear'}.png"
    plt.savefig(output_path, dpi=300)
    print(f"已保存图表: {output_path}")
    plt.close()
    
    # 返回计算得到的理论时间和比例因子，供创建表格使用
    return {
        'dc_sizes': dc_sizes,
        'dc_times': dc_times,
        'dc_theoretical': dc_theoretical,
        'bf_sizes': bf_sizes,
        'bf_times': bf_times,
        'bf_theoretical': bf_theoretical,
        'dc_benchmark_size': dc_benchmark_size,
        'bf_benchmark_size': bf_benchmark_size
    }

def create_comparison_table(theoretical_data, output_dir):
    """创建规模-时间-误差表格"""
    # 提取数据
    dc_sizes = theoretical_data['dc_sizes']
    dc_times = theoretical_data['dc_times']
    dc_theoretical = theoretical_data['dc_theoretical']
    dc_benchmark_size = theoretical_data['dc_benchmark_size']
    
    bf_sizes = theoretical_data['bf_sizes']
    bf_times = theoretical_data['bf_times']
    bf_theoretical = theoretical_data['bf_theoretical']
    bf_benchmark_size = theoretical_data['bf_benchmark_size']
    
    # 计算误差 (实际时间 - 理论时间) / 理论时间 * 100%
    dc_errors = (dc_times - dc_theoretical) / dc_theoretical * 100
    bf_errors = (bf_times - bf_theoretical) / bf_theoretical * 100
    
    # 创建分治算法表格
    dc_df = pd.DataFrame({
        '数据规模': dc_sizes,
        '实际时间(ms)': dc_times.round(4),
        '理论时间(ms)': dc_theoretical.round(4),
        '误差(%)': dc_errors.round(2)
    })
    
    # 创建暴力算法表格
    bf_df = pd.DataFrame({
        '数据规模': bf_sizes,
        '实际时间(ms)': bf_times.round(4),
        '理论时间(ms)': bf_theoretical.round(4),
        '误差(%)': bf_errors.round(2)
    })
    
    # 添加是否为基准点的标记
    dc_df['是否基准点'] = dc_df['数据规模'] == dc_benchmark_size
    bf_df['是否基准点'] = bf_df['数据规模'] == bf_benchmark_size
    
    # 保存表格
    dc_output_path = f"{output_dir}/分治算法时间误差表.csv"
    bf_output_path = f"{output_dir}/暴力算法时间误差表.csv"
    
    dc_df.to_csv(dc_output_path, index=False, encoding='utf-8-sig')  # 使用utf-8-sig确保Excel可以正确显示中文
    bf_df.to_csv(bf_output_path, index=False, encoding='utf-8-sig')
    
    print(f"已保存分治算法表格: {dc_output_path}")
    print(f"已保存暴力算法表格: {bf_output_path}")
    
    # 创建综合表格
    # 首先找出两种算法共有的数据规模
    common_sizes = set(dc_sizes).intersection(set(bf_sizes))
    if common_sizes:
        common_sizes = sorted(list(common_sizes))
        
        # 筛选共有规模的数据
        dc_filtered_df = dc_df[dc_df['数据规模'].isin(common_sizes)].copy()
        bf_filtered_df = bf_df[bf_df['数据规模'].isin(common_sizes)].copy()
        
        # 确保按照数据规模排序
        dc_filtered_df.sort_values('数据规模', inplace=True)
        bf_filtered_df.sort_values('数据规模', inplace=True)
        
        # 组合成一个表格
        combined_df = pd.DataFrame({
            '数据规模': dc_filtered_df['数据规模'],
            '分治算法实际时间(ms)': dc_filtered_df['实际时间(ms)'],
            '分治算法理论时间(ms)': dc_filtered_df['理论时间(ms)'],
            '分治算法误差(%)': dc_filtered_df['误差(%)'],
            '暴力算法实际时间(ms)': bf_filtered_df['实际时间(ms)'],
            '暴力算法理论时间(ms)': bf_filtered_df['理论时间(ms)'],
            '暴力算法误差(%)': bf_filtered_df['误差(%)'],
            '加速比': (bf_filtered_df['实际时间(ms)'] / dc_filtered_df['实际时间(ms)']).round(2)
        })
        
        # 保存综合表格
        combined_output_path = f"{output_dir}/算法比较综合表.csv"
        combined_df.to_csv(combined_output_path, index=False, encoding='utf-8-sig')
        print(f"已保存综合表格: {combined_output_path}")

def create_speedup_plot(dc_data, bf_data, output_dir, log_y_scale=False):
    """创建加速比图表（暴力算法时间/分治算法时间）"""
    # 首先合并两个数据集，只保留两种算法都有测试的规模
    common_sizes = set(dc_data['N']).intersection(set(bf_data['N']))
    
    if not common_sizes:
        print("没有找到两种算法共有的数据规模，无法创建加速比图表")
        return
        
    common_sizes = sorted(list(common_sizes))
    
    # 提取共有规模的数据
    dc_filtered = dc_data[dc_data['N'].isin(common_sizes)]
    bf_filtered = bf_data[bf_data['N'].isin(common_sizes)]
    
    # 确保数据按照N排序
    dc_filtered = dc_filtered.sort_values('N')
    bf_filtered = bf_filtered.sort_values('N')
    
    # 计算加速比
    sizes = dc_filtered['N'].values
    speedups = bf_filtered['BruteForceTime(ms)'].values / dc_filtered['DivideConquerTime(ms)'].values
    
    plt.figure(figsize=(12, 8))
    plt.plot(sizes, speedups, 'o-', linewidth=2, color='#2ca02c', 
             markersize=8, alpha=0.8)
    
    # 添加加速比=1的参考线
    plt.axhline(y=1, linestyle='--', color='gray', alpha=0.7)
    
    # 设置标题和坐标轴标签
    title_suffix = " (y轴对数坐标)" if log_y_scale else ""
    plt.title(f"分治算法相对于暴力算法的加速比{title_suffix}", fontsize=16)
    plt.xlabel("数据规模 (n)", fontsize=14)
    plt.ylabel("加速比 (暴力算法时间/分治算法时间)", fontsize=14)
    
    # 设置x轴格式为科学计数法
    formatter = ticker.FuncFormatter(format_sci_notation)
    plt.gca().xaxis.set_major_formatter(formatter)
    
    # 设置坐标轴
    plt.xscale('log')
    if log_y_scale:
        plt.yscale('log')
    plt.grid(True, linestyle='--', alpha=0.7)
    
    # 为每个数据点添加标签
    for i, (size, speedup) in enumerate(zip(sizes, speedups)):
        plt.annotate(f"{speedup:.1f}x", 
                    xy=(size, speedup), 
                    xytext=(0, 10),
                    textcoords="offset points",
                    ha='center', fontsize=9)
    
    plt.tight_layout()
    
    # 保存图表
    output_path = f"{output_dir}/算法加速比{'_log' if log_y_scale else ''}.png"
    plt.savefig(output_path, dpi=300)
    print(f"已保存图表: {output_path}")
    plt.close()

def create_range_performance_plot(data, algorithm_type, range_min, range_max, output_dir, log_scale=True):
    """创建指定范围内单个算法的性能图"""
    # 筛选指定范围的数据
    filtered_data = data[(data['N'] >= range_min) & (data['N'] <= range_max)]
    
    if filtered_data.empty:
        print(f"没有找到{range_min}到{range_max}范围内的{algorithm_type}数据")
        return
    
    plt.figure(figsize=(12, 8))
    
    # 提取数据
    sizes = filtered_data['N'].values
    
    if algorithm_type == "分治算法":
        times = filtered_data['DivideConquerTime(ms)'].values
        color = '#1f77b4'
    else:  # 蛮力法
        times = filtered_data['BruteForceTime(ms)'].values
        color = '#ff7f0e'
    
    # 绘制线图
    plt.plot(sizes, times, 'o-', linewidth=2, color=color, 
             label=algorithm_type, markersize=8, alpha=0.8)
    
    # 设置标题和坐标轴标签
    scale_suffix = " (对数坐标)" if log_scale else " (线性坐标)"
    range_suffix = f" ({range_min:,} - {range_max:,})"
    plt.title(f"{algorithm_type}性能{range_suffix}{scale_suffix}", fontsize=16)
    plt.xlabel("数据规模 (n)", fontsize=14)
    plt.ylabel("平均运行时间 (毫秒)", fontsize=14)
    
    # 设置x轴格式为科学计数法
    formatter = ticker.FuncFormatter(format_sci_notation)
    plt.gca().xaxis.set_major_formatter(formatter)
    
    if log_scale:
        plt.xscale('log')
        plt.yscale('log')
        plt.grid(True, which='major', linestyle='-', alpha=0.7)
        plt.grid(False, which='minor')
    else:
        plt.grid(True, linestyle='--', alpha=0.7)
    
    plt.legend(fontsize=12, loc='best')
    plt.tight_layout()
    
    # 保存图表
    scale_str = "log" if log_scale else "linear"
    output_path = f"{output_dir}/{algorithm_type}性能_{range_min}_{range_max}_{scale_str}.png"
    plt.savefig(output_path, dpi=300)
    print(f"已保存图表: {output_path}")
    plt.close()

def create_range_error_plot(theoretical_data, algorithm_type, range_min, range_max, output_dir, log_scale=False):
    """创建指定范围内单个算法的误差分析图"""
    # 提取相应算法的数据
    if algorithm_type == "分治算法":
        sizes = theoretical_data['dc_sizes']
        actual_times = theoretical_data['dc_times']
        theoretical_times = theoretical_data['dc_theoretical']
        benchmark_size = theoretical_data['dc_benchmark_size']
        color = '#1f77b4'
    else:  # 蛮力法
        sizes = theoretical_data['bf_sizes']
        actual_times = theoretical_data['bf_times']
        theoretical_times = theoretical_data['bf_theoretical']
        benchmark_size = theoretical_data['bf_benchmark_size']
        color = '#ff7f0e'
    
    # 计算误差 (实际时间 - 理论时间) / 理论时间 * 100%
    errors = (actual_times - theoretical_times) / theoretical_times * 100
    
    # 筛选指定范围的数据点
    in_range_indices = (sizes >= range_min) & (sizes <= range_max)
    filtered_sizes = sizes[in_range_indices]
    filtered_errors = errors[in_range_indices]
    
    if len(filtered_sizes) == 0:
        print(f"没有找到{range_min}到{range_max}范围内的{algorithm_type}数据")
        return
    
    plt.figure(figsize=(12, 8))
    
    # 绘制误差线图
    plt.plot(filtered_sizes, filtered_errors, 'o-', linewidth=2, color=color, 
             label=f"{algorithm_type}误差", markersize=8, alpha=0.8)
    
    # 添加零误差参考线
    plt.axhline(y=0, linestyle='--', color='gray', alpha=0.7, label="零误差线")
    
    # 设置标题和坐标轴标签
    scale_suffix = " (对数横坐标)" if log_scale else " (线性坐标)"
    range_suffix = f" ({range_min:,} - {range_max:,})"
    plt.title(f"{algorithm_type}误差分析{range_suffix}{scale_suffix}", fontsize=16)
    plt.xlabel("数据规模 (n)", fontsize=14)
    plt.ylabel("误差 (%)", fontsize=14)
    
    # 设置x轴格式为科学计数法
    formatter = ticker.FuncFormatter(format_sci_notation)
    plt.gca().xaxis.set_major_formatter(formatter)
    
    if log_scale:
        plt.xscale('log')
        plt.grid(True, which='major', linestyle='-', alpha=0.7)
        plt.grid(False, which='minor')
    else:
        plt.grid(True, linestyle='--', alpha=0.7)
    
    # 为每个数据点添加误差标签
    for size, error in zip(filtered_sizes, filtered_errors):
        plt.annotate(f"{error:.1f}%", 
                    xy=(size, error), 
                    xytext=(0, 10 if error >= 0 else -15),
                    textcoords="offset points",
                    ha='center', fontsize=9)
    
    # 标记基准点（如果在范围内）
    if range_min <= benchmark_size <= range_max:
        benchmark_error = errors[sizes == benchmark_size][0]
        plt.scatter([benchmark_size], [benchmark_error], color=color, s=100, 
                    marker='*', edgecolors='black', zorder=10,
                    label=f"{algorithm_type}基准点")
    
    plt.legend(fontsize=12, loc='best')
    plt.tight_layout()
    
    # 保存图表
    scale_str = "log" if log_scale else "linear"
    output_path = f"{output_dir}/{algorithm_type}误差分析_{range_min}_{range_max}_{scale_str}.png"
    plt.savefig(output_path, dpi=300)
    print(f"已保存图表: {output_path}")
    plt.close()

def main():
    # 创建输出目录
    output_dir = "d:/Chino_edu/temp/makephoto"
    Path(output_dir).mkdir(parents=True, exist_ok=True)
    
    # 加载数据
    dc_data, bf_data = load_data()
    
    # 创建性能比较图
    create_comparison_plot(dc_data, bf_data, output_dir, log_scale=True)  # 对数坐标
    create_comparison_plot(dc_data, bf_data, output_dir, log_scale=False)  # 线性坐标
    
    # 创建最小距离比较图
    create_min_distance_plot(dc_data, bf_data, output_dir, log_scale=True)  # 对数坐标
    create_min_distance_plot(dc_data, bf_data, output_dir, log_scale=False)  # 线性坐标
    
    # 创建理论与实际时间对比图，并获取数据用于创建表格
    theoretical_data = create_theoretical_comparison(dc_data, bf_data, output_dir, log_scale=True)  # 对数坐标
    create_theoretical_comparison(dc_data, bf_data, output_dir, log_scale=False)  # 线性坐标
    
    # 创建规模-时间-误差表格
    create_comparison_table(theoretical_data, output_dir)
    
    # 创建加速比图表
    create_speedup_plot(dc_data, bf_data, output_dir, log_y_scale=False)  # 常规坐标
    create_speedup_plot(dc_data, bf_data, output_dir, log_y_scale=True)   # y轴对数坐标
    
    # 创建特定范围的性能图
    # 分治算法 100,000 - 1,000,000
    create_range_performance_plot(dc_data, "分治算法", 100000, 1000000, output_dir, log_scale=True)
    create_range_performance_plot(dc_data, "分治算法", 100000, 1000000, output_dir, log_scale=False)

    # 蛮力法 1,000 - 10,000
    create_range_performance_plot(bf_data, "蛮力法", 1000, 10000, output_dir, log_scale=True)
    create_range_performance_plot(bf_data, "蛮力法", 1000, 10000, output_dir, log_scale=False)

    # 蛮力法 10,000 - 100,000
    create_range_performance_plot(bf_data, "蛮力法", 10000, 100000, output_dir, log_scale=True)
    create_range_performance_plot(bf_data, "蛮力法", 10000, 100000, output_dir, log_scale=False)
    
    # 创建特定范围的误差分析图
    # 分治算法 100,000 - 1,000,000
    create_range_error_plot(theoretical_data, "分治算法", 100000, 1000000, output_dir, log_scale=True)
    create_range_error_plot(theoretical_data, "分治算法", 100000, 1000000, output_dir, log_scale=False)

    # 蛮力法 1,000 - 10,000
    create_range_error_plot(theoretical_data, "蛮力法", 1000, 10000, output_dir, log_scale=True)
    create_range_error_plot(theoretical_data, "蛮力法", 1000, 10000, output_dir, log_scale=False)

    # 蛮力法 10,000 - 100,000
    create_range_error_plot(theoretical_data, "蛮力法", 10000, 100000, output_dir, log_scale=True)
    create_range_error_plot(theoretical_data, "蛮力法", 10000, 100000, output_dir, log_scale=False)
    
    create_range_error_plot(theoretical_data, "蛮力法", 1000, 10000, output_dir, log_scale=False)
    create_range_error_plot(theoretical_data, "蛮力法", 1000, 10000, output_dir, log_scale=True)
    
    # 分治法 100,000 - 1,000,000 的误差分析
    create_range_error_plot(theoretical_data, "分治算法", 100000, 1000000, output_dir, log_scale=False)
    create_range_error_plot(theoretical_data, "分治算法", 100000, 1000000, output_dir, log_scale=True)
    
    print("所有图表和表格生成完成!")

if __name__ == "__main__":
    main()