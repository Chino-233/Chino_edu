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

def create_comparison_plot(all_data, output_dir, log_scale=True):
    """创建所有算法的性能比较图"""
    plt.figure(figsize=(14, 8))
    
    colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd']
    markers = ['o', 's', '^', 'D', 'x']
    
    # 创建分组数据，用于不同算法下的点显示
    for i, (algorithm, complexity, data) in enumerate(all_data):
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
    
    plt.title("排序算法性能比较", fontsize=16)
    plt.xlabel("数据规模 (n)", fontsize=14)
    plt.ylabel("平均运行时间 (毫秒)", fontsize=14)
    
    # 设置x轴格式为科学计数法
    formatter = ticker.FuncFormatter(format_sci_notation)
    plt.gca().xaxis.set_major_formatter(formatter)
    
    if log_scale:
        plt.xscale('log')
        plt.yscale('log')
        plt.title("排序算法性能比较 (对数坐标)", fontsize=16)
        
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
    
    filename = "sorting_comparison_log.png" if log_scale else "sorting_comparison_linear.png"
    output_path = output_dir / "charts" / filename
    
    # 确保输出目录存在
    output_path.parent.mkdir(exist_ok=True, parents=True)
    
    plt.savefig(output_path, dpi=300)
    print(f"已保存图表: {output_path}")
    plt.close()

def create_theory_vs_actual_plots(all_data, output_dir):
    """为每个算法创建理论与实际运行时间的对比图"""
    # 创建输出目录
    charts_dir = output_dir / "charts"
    charts_dir.mkdir(exist_ok=True, parents=True)
    
    for algorithm, complexity, data in all_data:
        # 线性坐标图
        plt.figure(figsize=(12, 7))
        sizes = [item[0] for item in data]
        actual_times = [item[1] for item in data]
        theoretical_times = [item[2] for item in data]
        
        plt.plot(sizes, actual_times, 'o-', linewidth=2, label="实际运行时间", color='#1f77b4', alpha=0.8)
        plt.plot(sizes, theoretical_times, 's--', linewidth=2, label="理论运行时间", color='#ff7f0e', alpha=0.8)
        
        # 简化标题，去掉复杂度表示
        plt.title(f"{algorithm} 理论与实际运行时间对比", fontsize=16)
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
        annotate_steps = max(1, len(data) // 4)  # 只标注部分点
        for i, (size, actual, theoretical) in enumerate(data):
            if i % annotate_steps == 0 or i == len(data) - 1:  # 只标注部分点
                diff_percent = ((actual - theoretical) / theoretical * 100) if theoretical != 0 else 0
                plt.annotate(f"{diff_percent:.1f}%", 
                            xy=(size, max(actual, theoretical)), 
                            xytext=(0, 10),
                            textcoords="offset points",
                            ha='center', fontsize=9)
        
        plt.tight_layout()
        filename = f"{algorithm}_theory_vs_actual.png"
        output_path = charts_dir / filename
        plt.savefig(output_path, dpi=300)
        print(f"已保存图表: {output_path}")
        plt.close()
        
        # 对数坐标图
        plt.figure(figsize=(12, 7))
        plt.loglog(sizes, actual_times, 'o-', linewidth=2, label="实际运行时间", color='#1f77b4', alpha=0.8)
        plt.loglog(sizes, theoretical_times, 's--', linewidth=2, label="理论运行时间", color='#ff7f0e', alpha=0.8)
        
        plt.title(f"{algorithm} 理论与实际运行时间对比 (对数坐标)", fontsize=16)
        plt.xlabel("数据规模 (n)", fontsize=14)
        plt.ylabel("运行时间 (毫秒)", fontsize=14)
        
        # 只在主要刻度处显示网格线
        plt.grid(True, which='major', linestyle='-', alpha=0.7)
        plt.grid(False, which='minor')
        
        plt.legend(fontsize=12)
        plt.tight_layout()
        
        filename = f"{algorithm}_theory_vs_actual_log.png"
        output_path = charts_dir / filename
        plt.savefig(output_path, dpi=300)
        print(f"已保存图表: {output_path}")
        plt.close()

def create_complexity_group_plots(all_data, output_dir):
    """按照算法复杂度分组创建比较图"""
    # 创建输出目录
    charts_dir = output_dir / "charts"
    charts_dir.mkdir(exist_ok=True, parents=True)
    
    # 分离 O(n²) 和 O(n log n) 算法
    n_squared_algorithms = [(alg, comp, data) for alg, comp, data in all_data if "n²" in comp]
    n_log_n_algorithms = [(alg, comp, data) for alg, comp, data in all_data if "log n" in comp]
    
    # O(n²) 算法比较
    if n_squared_algorithms:
        plt.figure(figsize=(12, 7))
        colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728']
        markers = ['o', 's', '^', 'D']
        
        for i, (algorithm, complexity, data) in enumerate(n_squared_algorithms):
            sizes = [item[0] for item in data]
            actual_times = [item[1] for item in data]
            
            plt.plot(sizes, actual_times, marker=markers[i % len(markers)], 
                    linestyle='-', linewidth=2, color=colors[i % len(colors)], 
                    label=algorithm, markersize=8, alpha=0.8)
        
        plt.title("二次时间复杂度排序算法性能比较", fontsize=16)
        plt.xlabel("数据规模 (n)", fontsize=14)
        plt.ylabel("平均运行时间 (毫秒)", fontsize=14)
        plt.grid(True, linestyle='--', alpha=0.7)
        
        # 设置x轴格式为科学计数法
        formatter = ticker.FuncFormatter(format_sci_notation)
        plt.gca().xaxis.set_major_formatter(formatter)
        
        # 调整x轴刻度
        if max(sizes) > 1000:
            order = 10 ** int(np.log10(max(sizes)) - 1)
            new_ticks = np.array([1, 2, 5]) * order
            while new_ticks[-1] < max(sizes):
                order *= 10
                new_ticks = np.append(new_ticks, np.array([1, 2, 5]) * order)
            new_ticks = new_ticks[new_ticks <= max(sizes)]
            plt.xticks(new_ticks)
            
        plt.legend(fontsize=12)
        plt.tight_layout()
        
        filename = "n_squared_algorithms_comparison.png"
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
            sizes = [item[0] for item in data]
            actual_times = [item[1] for item in data]
            
            plt.plot(sizes, actual_times, marker=markers[i % len(markers)], 
                    linestyle='-', linewidth=2, color=colors[i % len(colors)], 
                    label=algorithm, markersize=8, alpha=0.8)
        
        plt.title("线性对数时间复杂度排序算法性能比较", fontsize=16)
        plt.xlabel("数据规模 (n)", fontsize=14)
        plt.ylabel("平均运行时间 (毫秒)", fontsize=14)
        plt.grid(True, linestyle='--', alpha=0.7)
        
        # 设置x轴格式为科学计数法
        formatter = ticker.FuncFormatter(format_sci_notation)
        plt.gca().xaxis.set_major_formatter(formatter)
        
        # 调整x轴刻度
        if max(sizes) > 1000:
            order = 10 ** int(np.log10(max(sizes)) - 1)
            new_ticks = np.array([1, 2, 5]) * order
            while new_ticks[-1] < max(sizes):
                order *= 10
                new_ticks = np.append(new_ticks, np.array([1, 2, 5]) * order)
            new_ticks = new_ticks[new_ticks <= max(sizes)]
            plt.xticks(new_ticks)
            
        plt.legend(fontsize=12)
        plt.tight_layout()
        
        filename = "n_log_n_algorithms_comparison.png"
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
    
    # 创建图表并指定输出目录
    create_comparison_plot(all_data, data_dir, log_scale=False)  # 线性坐标
    create_comparison_plot(all_data, data_dir, log_scale=True)   # 对数坐标
    create_theory_vs_actual_plots(all_data, data_dir)
    create_complexity_group_plots(all_data, data_dir)
    
    print("所有图表生成完成!")
    print(f"图表已保存到: {data_dir}/charts/")

if __name__ == "__main__":
    plt.rcParams['font.sans-serif'] = ['SimHei']  # 用来正常显示中文标签
    plt.rcParams['axes.unicode_minus'] = False    # 用来正常显示负号
    main()