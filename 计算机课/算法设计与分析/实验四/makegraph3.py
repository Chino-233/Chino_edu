import matplotlib.pyplot as plt
import numpy as np
import matplotlib.font_manager as fm

# 设置中文字体支持
plt.rcParams['font.sans-serif'] = ['SimHei', 'Microsoft YaHei', 'SimSun']
plt.rcParams['axes.unicode_minus'] = False

# 完整数据
e_values = [10, 100, 1000, 10000, 25000, 50000, 75000, 100000, 250000, 
            500000, 750000, 1000000, 2500000, 5000000, 7500000, 
            10000000, 25000000, 50000000, 75000000, 100000000]
time_ms = [0, 0, 0, 1, 4, 9, 15, 26, 47, 95, 148, 226, 481, 964, 
           1433, 1919, 5075, 10004, 15418, 23286]

# 计算理论O(e)复杂度曲线，以e=1000000为基准
k = 226 / 1000000  # e=1000000时耗时为226ms的比例系数
e_theory = np.linspace(min(e_values), max(e_values), 100)
time_theory = k * e_theory

# 计算每个数据点的理论值和误差
theory_points = [k * e for e in e_values]
# 避免除以零错误
relative_errors = []
for actual, theory in zip(time_ms, theory_points):
    if theory == 0 or actual == 0:
        # 当理论值或实际值为0时，如果两者都为0则误差为0%，否则误差为100%
        if theory == 0 and actual == 0:
            relative_errors.append(0)
        else:
            relative_errors.append(100)
    else:
        relative_errors.append(100 * (actual - theory) / theory)

# 创建数据表格并输出到txt文件
with open('large_e_performance_data.txt', 'w', encoding='utf-8') as f:
    f.write('鸡蛋个数e\t实际运行时间(ms)\t理论运行时间(ms)\t误差\n')
    for i in range(len(e_values)):
        # 格式化数值输出
        e_str = f"{e_values[i]:,}" if e_values[i] >= 1000 else str(e_values[i])
        actual_time_str = f"{time_ms[i]:.2f}"
        theory_time_str = f"{theory_points[i]:.2f}"
        error_str = f"{relative_errors[i]:.2f}%"
        
        # 写入一行数据
        f.write(f"{e_str}\t{actual_time_str}\t{theory_time_str}\t{error_str}\n")
    
    f.write('\n注：理论运行时间基于e=1000000时的耗时计算，假设时间复杂度为O(e)。固定f=500000000\n')

# 创建普通折线图（仅显示千万到一亿的数据）
plt.figure(figsize=(12, 7))

# 筛选千万到一亿范围的数据
normal_indices = [i for i, e in enumerate(e_values) if 10000000 <= e <= 100000000]
normal_e_values = [e_values[i] for i in normal_indices]
normal_time_ms = [time_ms[i] for i in normal_indices]
normal_relative_errors = [relative_errors[i] for i in normal_indices]

# 为普通图创建理论曲线
normal_e_theory = np.linspace(min(normal_e_values), max(normal_e_values), 100)
normal_time_theory = k * normal_e_theory

plt.plot(normal_e_values, normal_time_ms, 'o-', color='#3498db', linewidth=2, markersize=8, label='实际耗时')
plt.plot(normal_e_theory, normal_time_theory, '--', color='#e74c3c', linewidth=2, label='理论O(e)复杂度')

# 添加相对误差标注
for i in range(len(normal_e_values)):
    plt.annotate(f"{normal_relative_errors[i]:.2f}%", 
                (normal_e_values[i], normal_time_ms[i]), 
                textcoords="offset points",
                xytext=(0,10), 
                ha='center',
                fontsize=9,
                bbox=dict(boxstyle="round,pad=0.3", fc="white", ec="gray", alpha=0.8))

plt.xlabel('e值', fontsize=12)
plt.ylabel('耗时 (ms)', fontsize=12)
plt.title('e值与耗时的关系图（千万至一亿，f=500000000）', fontsize=14)
plt.grid(True, linestyle='--', alpha=0.7)
plt.legend(fontsize=10)
plt.tight_layout()
plt.savefig('large_e_vs_time_normal_with_errors.png', dpi=300, bbox_inches='tight')
#plt.show()

# 创建对数折线图（从10000开始的数据）
plt.figure(figsize=(12, 7))

# 筛选e>=10000的数据点
log_indices = [i for i, e in enumerate(e_values) if e >= 10000]
log_e_values = [e_values[i] for i in log_indices]
log_time_ms = [time_ms[i] for i in log_indices]
log_relative_errors = [relative_errors[i] for i in log_indices]

# 为对数图创建理论曲线
log_e_theory = np.linspace(min(log_e_values), max(log_e_values), 100)
log_time_theory = k * log_e_theory

# 绘制数据点
plt.loglog(log_e_values, log_time_ms, 'o-', color='#3498db', linewidth=2, markersize=8, label='实际耗时')
plt.loglog(log_e_theory, log_time_theory, '--', color='#e74c3c', linewidth=2, label='理论O(e)复杂度')

# 添加相对误差标注
for i in range(len(log_e_values)):
    plt.annotate(f"{log_relative_errors[i]:.2f}%", 
                (log_e_values[i], log_time_ms[i]), 
                textcoords="offset points",
                xytext=(0,10), 
                ha='center',
                fontsize=9,
                bbox=dict(boxstyle="round,pad=0.3", fc="white", ec="gray", alpha=0.8))

plt.xlabel('e值 (对数坐标)', fontsize=12)
plt.ylabel('耗时 (ms) (对数坐标)', fontsize=12)
plt.title('e值与耗时的对数关系图（f=500000000）', fontsize=14)
plt.grid(True, which="both", linestyle='--', alpha=0.7)
plt.legend(fontsize=10)
plt.tight_layout()
plt.savefig('large_e_vs_time_log_with_errors.png', dpi=300, bbox_inches='tight')
#plt.show()