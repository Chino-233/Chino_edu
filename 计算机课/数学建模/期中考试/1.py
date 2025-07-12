import numpy as np
import matplotlib.pyplot as plt
from scipy import interpolate
from scipy.optimize import curve_fit
from matplotlib.font_manager import FontProperties
import matplotlib as mpl

# 设置中文显示
plt.rcParams['font.sans-serif'] = ['SimHei']  # 用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False  # 用来正常显示负号

# 数据
s = np.array([0, 5, 15, 25, 35, 45, 55, 65, 75, 85, 95])  # 距离（米）
t = np.array([0, 0.955, 2.435, 3.435, 4.355, 5.230, 6.085, 6.945, 7.815, 8.690, 9.575])  # 时间（秒）

# (1) 绘制距离s对时间t的散点图
plt.figure(figsize=(10, 6))
plt.scatter(t, s, color='red', marker='o', s=50, label='原始数据点')
plt.xlabel('时间t/秒')
plt.ylabel('距离s/米')
plt.title('短跑运动员100m比赛：距离-时间散点图')
plt.grid(True)
plt.legend()
plt.savefig('散点图.png', dpi=300)
 

# (2) 三次样条插值
t_fine = np.linspace(0, 10, 500)  # 创建更密集的时间点用于插值
tck = interpolate.splrep(t, s)
s_spline = interpolate.splev(t_fine, tck)

plt.figure(figsize=(10, 6))
plt.scatter(t, s, color='red', marker='o', s=50, label='原始数据点')
plt.plot(t_fine, s_spline, 'b-', linewidth=2, label='三次样条插值')
plt.xlabel('时间t/秒')
plt.ylabel('距离s/米')
plt.title('短跑运动员100m比赛：三次样条插值曲线')
plt.grid(True)
plt.legend()
plt.savefig('三次样条插值.png', dpi=300)
 

# 获取三次样条插值的详细信息
knots, coeffs, degree = interpolate.splrep(t, s, full_output=0)

print("\n三次样条插值函数参数:")
print(f"节点位置: {knots}")
print(f"函数次数: {degree} (三次样条)")
print("各区间上的多项式系数:")
for i in range(len(coeffs) - 3):
    print(f"区间 {i+1} 上的系数: {coeffs[i:i+4]}")
print("注: 三次样条插值由多个三次多项式分段组成，无法用单一表达式表示")

# (3) 最小二乘法拟合
def quad_func(t, a, b, c):
    return a * t**2 + b * t + c

# 使用curve_fit进行拟合
popt, pcov = curve_fit(quad_func, t, s)
a, b, c = popt

# 输出拟合结果
print(f"拟合结果: s = {a:.4f}*t^2 + {b:.4f}*t + {c:.4f}")

# 获取拟合参数
popt, pcov = curve_fit(quad_func, t, s)
a, b, c = popt

# 输出完整的拟合函数表达式
print("\n最小二乘法拟合函数:")
print(f"s(t) = {a:.4f}t² + {b:.4f}t + {c:.4f}")

# 计算拟合优度R²
residuals = s - quad_func(t, a, b, c)
ss_res = np.sum(residuals**2)
ss_tot = np.sum((s - np.mean(s))**2)
r_squared = 1 - (ss_res / ss_tot)
print(f"拟合优度R²: {r_squared:.4f}")

# 绘制拟合曲线
plt.figure(figsize=(10, 6))
plt.scatter(t, s, color='red', marker='o', s=50, label='原始数据点')
plt.plot(t_fine, quad_func(t_fine, a, b, c), 'g-', linewidth=2, label='二次函数拟合')
plt.xlabel('时间t/秒')
plt.ylabel('距离s/米')
plt.title('短跑运动员100m比赛：二次函数拟合曲线')
plt.grid(True)
plt.legend()
plt.savefig('二次函数拟合.png', dpi=300)
 

# (4) 速度函数分析
# 样条插值的速度函数（导数）
v_spline = interpolate.splev(t_fine, tck, der=1)

# 二次拟合的速度函数（导数）
def v_quad(t, a, b):
    return 2 * a * t + b

v_fit = v_quad(t_fine, a, b)

# 绘制速度函数
plt.figure(figsize=(10, 6))
plt.plot(t_fine, v_spline, 'b-', linewidth=2, label='样条插值的速度函数')
plt.plot(t_fine, v_fit, 'g-', linewidth=2, label='二次拟合的速度函数')
plt.xlabel('时间t/秒')
plt.ylabel('速度v/(m/s)')
plt.title('短跑运动员100m比赛：速度-时间曲线')
plt.grid(True)
plt.legend()
plt.savefig('速度函数.png', dpi=300)
 

# 分析运动员的跑步策略
# 找到最大速度点
max_v_spline_idx = np.argmax(v_spline)
max_v_time = t_fine[max_v_spline_idx]
max_v = v_spline[max_v_spline_idx]

print(f"根据样条插值，运动员在 {max_v_time:.2f} 秒时达到最大速度 {max_v:.2f} m/s")

# 可视化最大速度点
plt.figure(figsize=(10, 6))
plt.plot(t_fine, v_spline, 'b-', linewidth=2, label='速度函数(样条插值)')
plt.scatter([max_v_time], [max_v], color='red', s=100, marker='*', label=f'最大速度: {max_v:.2f} m/s')
plt.xlabel('时间t/秒')
plt.ylabel('速度v/(m/s)')
plt.title('短跑运动员100m比赛：速度-时间曲线及最大速度点')
plt.grid(True)
plt.legend()
plt.savefig('速度分析.png', dpi=300)
 

# 计算各个阶段的平均速度
stages = [(0, 2), (2, 4), (4, 6), (6, 8), (8, 10)]
print("\n各阶段平均速度:")
for start, end in stages:
    mask = (t_fine >= start) & (t_fine <= end)
    avg_v = np.mean(v_spline[mask])
    print(f"时间 {start}-{end} 秒，平均速度: {avg_v:.2f} m/s")

# 绘制加速度函数
a_spline = interpolate.splev(t_fine, tck, der=2)
plt.figure(figsize=(10, 6))
plt.plot(t_fine, a_spline, 'r-', linewidth=2, label='加速度函数(样条插值)')
plt.axhline(y=0, color='k', linestyle='--', alpha=0.3)
plt.xlabel('时间t/秒')
plt.ylabel('加速度a/(m/s²)')
plt.title('短跑运动员100m比赛：加速度-时间曲线')
plt.grid(True)
plt.legend()
plt.savefig('加速度函数.png', dpi=300)
 

# 运动员跑步策略分析
print("\n运动员跑步策略分析:")
print("1. 起跑阶段：运动员在比赛开始时快速加速，加速度最大")
print("2. 中间阶段：保持较高速度，加速度逐渐减小但仍为正")
print("3. 冲刺阶段：接近终点时速度可能略有下降，体现出体力消耗")

print("\n基于三次样条插值的速度函数（分段表示）:")
for i in range(len(t)-1):
    t_i = t[i]
    t_next = t[i+1]
    
    # 获取函数值和导数值
    deriv1 = interpolate.splev([t_i, (t_i+t_next)/2, t_next], tck, der=1)
    deriv2 = interpolate.splev([t_i, (t_i+t_next)/2, t_next], tck, der=2)
    
    # 计算速度函数系数
    h = t_next - t_i
    a = (deriv2[2] - deriv2[0]) / (6*h)
    b = deriv2[0]/2
    c = deriv1[0]
    
    print(f"区间 [{t_i:.3f}, {t_next:.3f}] 上的速度函数: v(t) = {3*a:.6f}(t-{t_i:.3f})² + {2*b:.6f}(t-{t_i:.3f}) + {c:.6f}")