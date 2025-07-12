import matplotlib.pyplot as plt
import numpy as np
import matplotlib.font_manager as fm

# 设置中文字体支持
plt.rcParams['font.sans-serif'] = ['SimHei', 'Microsoft YaHei', 'SimSun']
plt.rcParams['axes.unicode_minus'] = False

# 数据
f_values = [100, 250, 500, 750, 1000, 2500, 5000, 7500, 10000, 25000, 50000, 75000, 100000]
time_ms = [1, 12, 54, 114, 218, 1307, 5056, 11600, 21695, 133425, 535612, 1197064, 2523308]

# 计算理论O(f^2)复杂度曲线，以f=1000为基准
k = 218 / (1000**2)  # f=1000时耗时为218ms的比例系数
f_theory = np.linspace(min(f_values), max(f_values), 100)
time_theory = k * f_theory**2

# 计算每个数据点的理论值和误差
theory_points = [k * f**2 for f in f_values]
relative_errors = [100 * (actual - theory) / theory for actual, theory in zip(time_ms, theory_points)]

# 创建数据表格并输出到txt文件
with open('f_performance_data.txt', 'w', encoding='utf-8') as f:
    f.write('楼层数f\t实际运行时间(ms)\t理论运行时间(ms)\t误差\n')
    for i in range(len(f_values)):
        # 格式化数值输出
        f_str = f"{f_values[i]:,}" if f_values[i] >= 1000 else str(f_values[i])
        actual_time_str = f"{time_ms[i]:.2f}"
        theory_time_str = f"{theory_points[i]:.2f}"
        error_str = f"{relative_errors[i]:.2f}%"
        
        # 写入一行数据
        f.write(f"{f_str}\t{actual_time_str}\t{theory_time_str}\t{error_str}\n")
    
    f.write('\n注：理论运行时间基于f=1000时的耗时计算，假设时间复杂度为O(f^2)。固定e=20\n')

# 创建普通折线图
plt.figure(figsize=(12, 7))
plt.plot(f_values, time_ms, 'o-', color='#3498db', linewidth=2, markersize=8, label='实际耗时')
plt.plot(f_theory, time_theory, '--', color='#e74c3c', linewidth=2, label='理论O(f^2)复杂度')

# 添加相对误差标注
for i in range(len(f_values)):
    plt.annotate(f"{relative_errors[i]:.2f}%", 
                 (f_values[i], time_ms[i]), 
                 textcoords="offset points",
                 xytext=(0,10), 
                 ha='center',
                 fontsize=9,
                 bbox=dict(boxstyle="round,pad=0.3", fc="white", ec="gray", alpha=0.8))

plt.xlabel('f值', fontsize=12)
plt.ylabel('耗时 (ms)', fontsize=12)
plt.title('f值与耗时的关系图（含相对误差）', fontsize=14)
plt.grid(True, linestyle='--', alpha=0.7)
plt.legend(fontsize=10)
plt.tight_layout()
plt.savefig('f_vs_time_normal_with_errors.png', dpi=300, bbox_inches='tight')
plt.show()

# 创建对数折线图
plt.figure(figsize=(12, 7))
plt.loglog(f_values, time_ms, 'o-', color='#3498db', linewidth=2, markersize=8, label='实际耗时')
plt.loglog(f_theory, time_theory, '--', color='#e74c3c', linewidth=2, label='理论O(f^2)复杂度')

# 添加相对误差标注
for i in range(len(f_values)):
    plt.annotate(f"{relative_errors[i]:.2f}%", 
                 (f_values[i], time_ms[i]), 
                 textcoords="offset points",
                 xytext=(0,10), 
                 ha='center',
                 fontsize=9,
                 bbox=dict(boxstyle="round,pad=0.3", fc="white", ec="gray", alpha=0.8))

plt.xlabel('f值 (对数坐标)', fontsize=12)
plt.ylabel('耗时 (ms) (对数坐标)', fontsize=12)
plt.title('f值与耗时的对数关系图（含相对误差）', fontsize=14)
plt.grid(True, which="both", linestyle='--', alpha=0.7)
plt.legend(fontsize=10)
plt.tight_layout()
plt.savefig('f_vs_time_log_with_errors.png', dpi=300, bbox_inches='tight')
plt.show()