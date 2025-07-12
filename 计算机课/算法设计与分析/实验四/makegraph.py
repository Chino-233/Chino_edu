import matplotlib.pyplot as plt
import numpy as np
import matplotlib.font_manager as fm

# 设置中文字体支持
plt.rcParams['font.sans-serif'] = ['SimHei', 'Microsoft YaHei', 'SimSun']
plt.rcParams['axes.unicode_minus'] = False

# 数据
e_values = [100, 250, 500, 750, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000]
time_ms = [1077, 2570, 5306, 7678, 10658, 15499, 20647, 25751, 30831, 35819, 40960, 45936, 51263]

# 计算理论O(e)复杂度曲线，以e=1000为基准
k = 10658 / 1000  # e=1000时耗时为10658ms的比例系数
e_theory = np.linspace(min(e_values), max(e_values), 100)
time_theory = k * e_theory

# 计算每个数据点的理论值和误差
theory_points = [k * e for e in e_values]
relative_errors = [100 * (actual - theory) / theory for actual, theory in zip(time_ms, theory_points)]

# 创建数据表格并输出到txt文件
with open('e_performance_data.txt', 'w', encoding='utf-8') as f:
    f.write('鸡蛋个数e\t实际运行时间(ms)\t理论运行时间(ms)\t误差\n')
    for i in range(len(e_values)):
        # 格式化数值输出
        e_str = f"{e_values[i]:,}" if e_values[i] >= 1000 else str(e_values[i])
        actual_time_str = f"{time_ms[i]:.2f}"
        theory_time_str = f"{theory_points[i]:.2f}"
        error_str = f"{relative_errors[i]:.2f}%"
        
        # 写入一行数据
        f.write(f"{e_str}\t{actual_time_str}\t{theory_time_str}\t{error_str}\n")
    
    f.write('\n注：理论运行时间基于e=1000时的耗时计算，假设时间复杂度为O(e)\n')

# 创建普通折线图
plt.figure(figsize=(12, 7))
plt.plot(e_values, time_ms, 'o-', color='#3498db', linewidth=2, markersize=8, label='实际耗时')
plt.plot(e_theory, time_theory, '--', color='#e74c3c', linewidth=2, label='理论O(e)复杂度')

# 添加相对误差标注
for i in range(len(e_values)):
    plt.annotate(f"{relative_errors[i]:.2f}%", 
                 (e_values[i], time_ms[i]), 
                 textcoords="offset points",
                 xytext=(0,10), 
                 ha='center',
                 fontsize=9,
                 bbox=dict(boxstyle="round,pad=0.3", fc="white", ec="gray", alpha=0.8))

plt.xlabel('e值', fontsize=12)
plt.ylabel('耗时 (ms)', fontsize=12)
plt.title('e值与耗时的关系图（含相对误差）', fontsize=14)
plt.grid(True, linestyle='--', alpha=0.7)
plt.legend(fontsize=10)
plt.tight_layout()
plt.savefig('e_vs_time_normal_with_errors.png', dpi=300, bbox_inches='tight')
plt.show()

# 创建对数折线图
plt.figure(figsize=(12, 7))
plt.loglog(e_values, time_ms, 'o-', color='#3498db', linewidth=2, markersize=8, label='实际耗时')
plt.loglog(e_theory, time_theory, '--', color='#e74c3c', linewidth=2, label='理论O(e)复杂度')

# 添加相对误差标注
for i in range(len(e_values)):
    plt.annotate(f"{relative_errors[i]:.2f}%", 
                 (e_values[i], time_ms[i]), 
                 textcoords="offset points",
                 xytext=(0,10), 
                 ha='center',
                 fontsize=9,
                 bbox=dict(boxstyle="round,pad=0.3", fc="white", ec="gray", alpha=0.8))

plt.xlabel('e值 (对数坐标)', fontsize=12)
plt.ylabel('耗时 (ms) (对数坐标)', fontsize=12)
plt.title('e值与耗时的对数关系图（含相对误差）', fontsize=14)
plt.grid(True, which="both", linestyle='--', alpha=0.7)
plt.legend(fontsize=10)
plt.tight_layout()
plt.savefig('e_vs_time_log_with_errors.png', dpi=300, bbox_inches='tight')
plt.show()