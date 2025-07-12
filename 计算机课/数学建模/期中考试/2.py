import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import fsolve

# 设置中文显示
plt.rcParams['font.sans-serif'] = ['SimHei']  # 用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False  # 用来正常显示负号
# 参数定义
lambda_val = 0.13  # 胃肠道向血液转移率
mu = 0.11          # 血液排除率
initial_drug = 1100  # 初始药量(mg)
blood_volume = 2000  # 血液总量(ml)

# 定义微分方程的解析解
def x_t(t):
    """胃肠道中药量随时间的变化"""
    return initial_drug * np.exp(-lambda_val * t)

def y_t(t):
    """血液中药量随时间的变化"""
    return 7150 * (np.exp(-mu * t) - np.exp(-lambda_val * t))

def concentration(t):
    """血液中药物浓度(微克/ml)"""
    return y_t(t) * 1000 / blood_volume

# 找出血液中药量的峰值
def dy_dt(t):
    """血液中药量的导数"""
    return 7150 * (-mu * np.exp(-mu * t) + lambda_val * np.exp(-lambda_val * t))

# 求导数为0的时间点
t_peak = fsolve(dy_dt, [5])[0]
peak_amount = y_t(t_peak)
peak_concentration = concentration(t_peak)

# 找出何时达到严重中毒阈值(100微克/ml)
def toxic_equation(t):
    return concentration(t) - 100

# 求解何时达到中毒阈值
t_toxic = fsolve(toxic_equation, [2])[0]

# 绘图
t = np.linspace(0, 24, 1000)
plt.figure(figsize=(12, 10))

# 绘制胃肠道药量变化
plt.subplot(2, 1, 1)
plt.plot(t, x_t(t), 'r-', linewidth=2)
plt.title('胃肠道中药量x(t)随时间的变化')
plt.xlabel('时间(小时)')
plt.ylabel('药量(毫克)')
plt.grid(True)

# 绘制血液中药量变化
plt.subplot(2, 1, 2)
plt.plot(t, y_t(t), 'b-', linewidth=2)
plt.axhline(y=200, color='r', linestyle='--', label='严重中毒阈值(200mg)')
plt.axvline(x=t_peak, color='g', linestyle='--', label=f'峰值时间({t_peak:.2f}小时)')
plt.axvline(x=t_toxic, color='m', linestyle='--', label=f'达到中毒阈值时间({t_toxic:.2f}小时)')
plt.plot(t_peak, peak_amount, 'go', markersize=8, label=f'峰值({peak_amount:.2f}mg)')
plt.title('血液中药量y(t)随时间的变化')
plt.xlabel('时间(小时)')
plt.ylabel('药量(毫克)')
plt.grid(True)
plt.legend()

plt.tight_layout()
plt.show()

print(f"峰值时间: {t_peak:.2f}小时")
print(f"峰值药量: {peak_amount:.2f}毫克")
print(f"峰值浓度: {peak_concentration:.2f}微克/毫升")
print(f"达到中毒阈值时间: {t_toxic:.2f}小时")
