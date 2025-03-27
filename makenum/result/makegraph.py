import matplotlib.pyplot as plt
import numpy as np
import re
import os
from pathlib import Path
import matplotlib.ticker as ticker

def parse_result_file(file_path):
    """解析结果文件，提取算法名称和性能数据"""
    with open(file_path, 'r', encoding='utf-8') as f:
        content = f.read()
    
    # 提取算法名称
    algorithm_name = re.search(r'=+ (\w+)性能测试结果', content).group(1)
    
    # 提取时间复杂度
    complexity = re.search(r'时间复杂度: (O\(.+\))', content).group(1)
    
    # 提取数据表
    lines = content.strip().split('\n')
    data = []
    for line in lines[6:]:  # 跳过前6行（标题和表头）
        if line.strip():
            parts = line.strip().split('\t')
            if len(parts) >= 3:
                try:
                    size = int(parts[0].strip())
                    actual_time = float(parts[1].strip())
                    theoretical_time = float(parts[2].strip())
                    data.append((size, actual_time, theoretical_time))
                except (ValueError, IndexError):
                    continue
    
    return algorithm_name, complexity, data

def format_sci_notation(x, pos):
    """格式化科学计数法显示"""
    if x == 0:
        return "0"
    
    exponent = int(np.log10(x))
    coeff = x / 10**exponent
    
    if coeff == 1:
        return r'$10^{%d}$' % exponent
    elif coeff == 5:
        return r'$5{\times}10^{%d}$' % exponent
    else:
        return r'$%d{\times}10^{%d}$' % (coeff, exponent)

def create_comparison_plot(all_data, output_dir, log_scale=True, x_range=None):
    """
    创建所有算法的性能比较图
    
    参数:
    - all_data: 所有算法的数据
    - output_dir: 输出目录
    - log_scale: 是否使用对数坐标
    - x_range: 可选的x轴范围限制, 格式为(min, max)
    """
    plt.figure(figsize=(14, 8))
    
    colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd']
    markers = ['o', 's', '^', 'D', 'x']
    
    # 创建分组数据，用于不同算法下的点显示
    for i, (algorithm, complexity, data) in enumerate(all_data):
        # 如果指定了x范围，过滤数据
        if x_range:
            filtered_data = [(size, actual, theo) for size, actual, theo in data 
                            if x_range[0] <= size <= x_range[1]]
            if not filtered_data:  # 如果过滤后没有数据，跳过此算法
                continue
            sizes = [item[0] for item in filtered_data]
            actual_times = [item[1] for item in filtered_data]
        else:
            sizes = [item[0] for item in data]
            actual_times = [item[1] for item in data]
        
        # 简化标签，去掉复杂度表示
        label = f"{algorithm}"
        
        # 调整点的大小和透明度，解决小规模数据点聚集问题
        if log_scale:
            markersize = 8
        else:
            # 使用固定值而不是列表，根据整体数据规模设置合适的点大小
            # 数据规模越大，点越小，以避免拥挤
            max_size = max(sizes)
            if max_size > 1000000:
                markersize = 5
            elif max_size > 100000:
                markersize = 6
            else:
                markersize = 8
        
        alpha = 0.8 if log_scale else 0.7
        
        # 使用线图绘制，每个算法使用统一大小的点
        plt.plot(sizes, actual_times, marker=markers[i % len(markers)], 
                 linestyle='-', linewidth=2, color=colors[i % len(colors)], 
                 label=label, markersize=markersize, alpha=alpha)
    
    # 设置标题和坐标轴标签
    range_suffix = f" (范围: {x_range[0]/10000}万 - {x_range[1]/10000}万)" if x_range else ""
    plt.title(f"排序算法性能比较{range_suffix}", fontsize=16)
    plt.xlabel("数据规模 (n)", fontsize=14)
    plt.ylabel("平均运行时间 (毫秒)", fontsize=14)
    
    # 设置x轴格式为科学计数法
    formatter = ticker.FuncFormatter(format_sci_notation)
    plt.gca().xaxis.set_major_formatter(formatter)
    
    if log_scale:
        plt.xscale('log')
        plt.yscale('log')
        plt.title(f"排序算法性能比较 (对数坐标){range_suffix}", fontsize=16)
        
        # 只在主要刻度处显示网格线
        plt.grid(True, which='major', linestyle='-', alpha=0.7)
        plt.grid(False, which='minor')
    else:
        plt.grid(True, linestyle='--', alpha=0.7)
        
        # 调整x轴刻度，使用合适的科学计数法表示
        if max(sizes) > 1000:
            order = 10 ** int(np.log10(max(sizes)) - 1)
            new_ticks = np.array([1, 2, 5]) * order
            while new_ticks[-1] < max(sizes):
                order *= 10
                new_ticks = np.append(new_ticks, np.array([1, 2, 5]) * order)
            new_ticks = new_ticks[new_ticks <= max(sizes)]
            plt.xticks(new_ticks)
    
    plt.legend(fontsize=12, loc='best')
    plt.tight_layout()
    
    # 构建文件名
    range_str = f"_{x_range[0]//10000}w_to_{x_range[1]//10000}w" if x_range else ""
    filename = f"sorting_comparison_log{range_str}.png" if log_scale else f"sorting_comparison_linear{range_str}.png"
    output_path = output_dir / "charts" / filename
    
    # 确保输出目录存在
    output_path.parent.mkdir(exist_ok=True, parents=True)
    
    plt.savefig(output_path, dpi=300)
    print(f"已保存图表: {output_path}")
    plt.close()

def create_theory_vs_actual_plots(all_data, output_dir, x_range=None):
    """
    为每个算法创建理论与实际运行时间的对比图
    
    参数:
    - all_data: 所有算法的数据
    - output_dir: 输出目录
    - x_range: 可选的x轴范围限制, 格式为(min, max)
    """
    # 创建输出目录
    charts_dir = output_dir / "charts"
    charts_dir.mkdir(exist_ok=True, parents=True)
    
    for algorithm, complexity, data in all_data:
        # 如果指定了x范围，过滤数据
        if x_range:
            filtered_data = [(size, actual, theo) for size, actual, theo in data 
                            if x_range[0] <= size <= x_range[1]]
            if not filtered_data:  # 如果过滤后没有数据，跳过此算法
                continue
            sizes = [item[0] for item in filtered_data]
            actual_times = [item[1] for item in filtered_data]
            theoretical_times = [item[2] for item in filtered_data]
        else:
            sizes = [item[0] for item in data]
            actual_times = [item[1] for item in data]
            theoretical_times = [item[2] for item in data]
        
        # 如果数据点太少，跳过
        if len(sizes) < 2:
            print(f"警告: {algorithm} 在指定范围内数据点太少，跳过绘图")
            continue
            
        # 线性坐标图
        plt.figure(figsize=(12, 7))
        plt.plot(sizes, actual_times, 'o-', linewidth=2, label="实际运行时间", color='#1f77b4', alpha=0.8)
        plt.plot(sizes, theoretical_times, 's--', linewidth=2, label="理论运行时间", color='#ff7f0e', alpha=0.8)
        
        # 设置标题和范围后缀
        range_suffix = f" (范围: {x_range[0]/10000}万 - {x_range[1]/10000}万)" if x_range else ""
        plt.title(f"{algorithm} 理论与实际运行时间对比{range_suffix}", fontsize=16)
        plt.xlabel("数据规模 (n)", fontsize=14)
        plt.ylabel("运行时间 (毫秒)", fontsize=14)
        plt.grid(True, linestyle='--', alpha=0.7)
        
        # 设置x轴格式为科学计数法
        formatter = ticker.FuncFormatter(format_sci_notation)
        plt.gca().xaxis.set_major_formatter(formatter)
        
        # 调整x轴刻度，使用合适的科学计数法表示
        if max(sizes) > 1000:
            order = 10 ** int(np.log10(max(sizes)) - 1)
            new_ticks = np.array([1, 2, 5]) * order
            while new_ticks[-1] < max(sizes):
                order *= 10
                new_ticks = np.append(new_ticks, np.array([1, 2, 5]) * order)
            new_ticks = new_ticks[new_ticks <= max(sizes)]
            plt.xticks(new_ticks)
            
        plt.legend(fontsize=12)
        
        # 添加标注，显示差异率，但避免太多标记重叠
        annotate_steps = max(1, len(sizes) // 4)  # 只标注部分点
        for i in range(len(sizes)):
            if i % annotate_steps == 0 or i == len(sizes) - 1:  # 只标注部分点
                size, actual, theoretical = sizes[i], actual_times[i], theoretical_times[i]
                diff_percent = ((actual - theoretical) / theoretical * 100) if theoretical != 0 else 0
                plt.annotate(f"{diff_percent:.1f}%", 
                            xy=(size, max(actual, theoretical)), 
                            xytext=(0, 10),
                            textcoords="offset points",
                            ha='center', fontsize=9)
        
        plt.tight_layout()
        
        # 构建文件名
        range_str = f"_{x_range[0]//10000}w_to_{x_range[1]//10000}w" if x_range else ""
        filename = f"{algorithm}_theory_vs_actual{range_str}.png"
        output_path = charts_dir / filename
        plt.savefig(output_path, dpi=300)
        print(f"已保存图表: {output_path}")
        plt.close()
        
        # 对数坐标图
        plt.figure(figsize=(12, 7))
        plt.loglog(sizes, actual_times, 'o-', linewidth=2, label="实际运行时间", color='#1f77b4', alpha=0.8)
        plt.loglog(sizes, theoretical_times, 's--', linewidth=2, label="理论运行时间", color='#ff7f0e', alpha=0.8)
        
        plt.title(f"{algorithm} 理论与实际运行时间对比 (对数坐标){range_suffix}", fontsize=16)
        plt.xlabel("数据规模 (n)", fontsize=14)
        plt.ylabel("运行时间 (毫秒)", fontsize=14)
        
        # 只在主要刻度处显示网格线
        plt.grid(True, which='major', linestyle='-', alpha=0.7)
        plt.grid(False, which='minor')
        
        plt.legend(fontsize=12)
        plt.tight_layout()
        
        # 构建文件名
        filename = f"{algorithm}_theory_vs_actual_log{range_str}.png"
        output_path = charts_dir / filename
        plt.savefig(output_path, dpi=300)
        print(f"已保存图表: {output_path}")
        plt.close()

def create_complexity_group_plots(all_data, output_dir, x_range=None):
    """
    按照算法复杂度分组创建比较图
    
    参数:
    - all_data: 所有算法的数据
    - output_dir: 输出目录
    - x_range: 可选的x轴范围限制, 格式为(min, max)
    """
    # 创建输出目录
    charts_dir = output_dir / "charts"
    charts_dir.mkdir(exist_ok=True, parents=True)
    
    # 分离 O(n²) 和 O(n log n) 算法
    n_squared_algorithms = [(alg, comp, data) for alg, comp, data in all_data if "n²" in comp]
    n_log_n_algorithms = [(alg, comp, data) for alg, comp, data in all_data if "log n" in comp]
    
    # 设置标题和范围后缀
    range_suffix = f" (范围: {x_range[0]/10000}万 - {x_range[1]/10000}万)" if x_range else ""
    
    # O(n²) 算法比较
    if n_squared_algorithms:
        plt.figure(figsize=(12, 7))
        colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728']
        markers = ['o', 's', '^', 'D']
        
        for i, (algorithm, complexity, data) in enumerate(n_squared_algorithms):
            # 如果指定了x范围，过滤数据
            if x_range:
                filtered_data = [(size, actual, theo) for size, actual, theo in data 
                               if x_range[0] <= size <= x_range[1]]
                if not filtered_data:  # 如果过滤后没有数据，跳过此算法
                    continue
                sizes = [item[0] for item in filtered_data]
                actual_times = [item[1] for item in filtered_data]
            else:
                sizes = [item[0] for item in data]
                actual_times = [item[1] for item in data]
            
            # 如果数据点太少，跳过
            if len(sizes) < 2:
                print(f"警告: {algorithm} 在指定范围内数据点太少，跳过绘图")
                continue
                
            plt.plot(sizes, actual_times, marker=markers[i % len(markers)], 
                    linestyle='-', linewidth=2, color=colors[i % len(colors)], 
                    label=algorithm, markersize=8, alpha=0.8)
        
        plt.title(f"二次时间复杂度排序算法性能比较{range_suffix}", fontsize=16)
        plt.xlabel("数据规模 (n)", fontsize=14)
        plt.ylabel("平均运行时间 (毫秒)", fontsize=14)
        plt.grid(True, linestyle='--', alpha=0.7)
        
        # 设置x轴格式为科学计数法
        formatter = ticker.FuncFormatter(format_sci_notation)
        plt.gca().xaxis.set_major_formatter(formatter)
        
        # 调整x轴刻度
        if sizes and max(sizes) > 1000:
            order = 10 ** int(np.log10(max(sizes)) - 1)
            new_ticks = np.array([1, 2, 5]) * order
            while new_ticks[-1] < max(sizes):
                order *= 10
                new_ticks = np.append(new_ticks, np.array([1, 2, 5]) * order)
            new_ticks = new_ticks[new_ticks <= max(sizes)]
            plt.xticks(new_ticks)
            
        plt.legend(fontsize=12)
        plt.tight_layout()
        
        # 构建文件名
        range_str = f"_{x_range[0]//10000}w_to_{x_range[1]//10000}w" if x_range else ""
        filename = f"n_squared_algorithms_comparison{range_str}.png"
        output_path = charts_dir / filename
        plt.savefig(output_path, dpi=300)
        print(f"已保存图表: {output_path}")
        plt.close()
    
    # O(n log n) 算法比较
    if n_log_n_algorithms:
        plt.figure(figsize=(12, 7))
        colors = ['#9467bd', '#8c564b', '#e377c2', '#7f7f7f']
        markers = ['o', 's', '^', 'D']
        
        for i, (algorithm, complexity, data) in enumerate(n_log_n_algorithms):
            # 如果指定了x范围，过滤数据
            if x_range:
                filtered_data = [(size, actual, theo) for size, actual, theo in data 
                               if x_range[0] <= size <= x_range[1]]
                if not filtered_data:  # 如果过滤后没有数据，跳过此算法
                    continue
                sizes = [item[0] for item in filtered_data]
                actual_times = [item[1] for item in filtered_data]
            else:
                sizes = [item[0] for item in data]
                actual_times = [item[1] for item in data]
                
            # 如果数据点太少，跳过
            if len(sizes) < 2:
                print(f"警告: {algorithm} 在指定范围内数据点太少，跳过绘图")
                continue
                
            plt.plot(sizes, actual_times, marker=markers[i % len(markers)], 
                    linestyle='-', linewidth=2, color=colors[i % len(colors)], 
                    label=algorithm, markersize=8, alpha=0.8)
        
        plt.title(f"线性对数时间复杂度排序算法性能比较{range_suffix}", fontsize=16)
        plt.xlabel("数据规模 (n)", fontsize=14)
        plt.ylabel("平均运行时间 (毫秒)", fontsize=14)
        plt.grid(True, linestyle='--', alpha=0.7)
        
        # 设置x轴格式为科学计数法
        formatter = ticker.FuncFormatter(format_sci_notation)
        plt.gca().xaxis.set_major_formatter(formatter)
        
        # 调整x轴刻度
        if sizes and max(sizes) > 1000:
            order = 10 ** int(np.log10(max(sizes)) - 1)
            new_ticks = np.array([1, 2, 5]) * order
            while new_ticks[-1] < max(sizes):
                order *= 10
                new_ticks = np.append(new_ticks, np.array([1, 2, 5]) * order)
            new_ticks = new_ticks[new_ticks <= max(sizes)]
            plt.xticks(new_ticks)
            
        plt.legend(fontsize=12)
        plt.tight_layout()
        
        # 构建文件名
        range_str = f"_{x_range[0]//10000}w_to_{x_range[1]//10000}w" if x_range else ""
        filename = f"n_log_n_algorithms_comparison{range_str}.png"
        output_path = charts_dir / filename
        plt.savefig(output_path, dpi=300)
        print(f"已保存图表: {output_path}")
        plt.close()

def main():
    # 数据文件的目录
    data_dir = Path('d:/chino_edu/makenum/result')
    
    # 搜索目录中的结果文件
    result_files = list(data_dir.glob('*Sort_results.txt'))
    all_data = []
    
    # 解析所有结果文件
    for file_path in result_files:
        try:
            algorithm_name, complexity, data = parse_result_file(file_path)
            if data:  # 确保有数据
                all_data.append((algorithm_name, complexity, data))
                print(f"成功解析 {algorithm_name} 的数据，共 {len(data)} 个数据点")
            else:
                print(f"警告: {file_path} 中没有找到数据")
        except Exception as e:
            print(f"处理文件 {file_path} 时出错: {e}")
    
    if not all_data:
        print("未找到任何数据，请检查文件路径和格式")
        return
    
    # 定义范围
    full_range = None  # 不限制，包含所有数据点
    large_range = (100000, 1000000)  # 10万到100万
    
    # 创建图表并指定输出目录
    
    # 对数坐标系图表 - 使用全部数据点
    create_comparison_plot(all_data, data_dir, log_scale=True, x_range=full_range)  
    
    # 普通坐标系图表 - 只使用大于10万的数据点
    create_comparison_plot(all_data, data_dir, log_scale=False, x_range=large_range)  
    
    # 同样处理理论与实际比较图和复杂度分组图
    # 对数坐标
    create_theory_vs_actual_plots(all_data, data_dir, x_range=full_range)
    create_complexity_group_plots(all_data, data_dir, x_range=full_range)
    
    # 普通坐标
    create_theory_vs_actual_plots(all_data, data_dir, x_range=large_range)
    create_complexity_group_plots(all_data, data_dir, x_range=large_range)
    
    print("所有图表生成完成!")
    print(f"图表已保存到: {data_dir}/charts/")

if __name__ == "__main__":
    plt.rcParams['font.sans-serif'] = ['SimHei']  # 用来正常显示中文标签
    plt.rcParams['axes.unicode_minus'] = False    # 用来正常显示负号
    main()