import numpy as np
from scipy import interpolate
import matplotlib.pyplot as plt

# 设置中文显示
plt.rcParams['font.sans-serif'] = ['SimHei']  # 用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False    # 用来正常显示负号

# 已知数据点
T = np.array([700, 720, 740, 760, 780])
V = np.array([0.0977, 0.1218, 0.1406, 0.1551, 0.1664])

# 线性插值
f_linear = interpolate.interp1d(T, V, kind='linear')

# 三次样条插值
f_cubic = interpolate.interp1d(T, V, kind='cubic')

# 需要计算的温度点
T_interp = np.array([750, 770])
V_linear = f_linear(T_interp)
V_cubic = f_cubic(T_interp)

# 输出插值结果
print(f"线性插值结果：")
print(f"当T=750时，V={V_linear[0]:.6f}")
print(f"当T=770时，V={V_linear[1]:.6f}")
print(f"\n三次样条插值结果：")
print(f"当T=750时，V={V_cubic[0]:.6f}")
print(f"当T=770时，V={V_cubic[1]:.6f}")

# 生成绘图用的细分点
T_fine = np.linspace(700, 780, 100)
V_linear_fine = f_linear(T_fine)
V_cubic_fine = f_cubic(T_fine)

# 绘图
plt.figure(figsize=(10, 6))
plt.scatter(T, V, color='red', marker='o', s=80, label='原始数据点')
plt.plot(T_fine, V_linear_fine, 'b-', label='线性插值')
plt.plot(T_fine, V_cubic_fine, 'g-', label='三次样条插值')
plt.scatter(T_interp, V_linear, color='blue', marker='x', s=100, label='线性插值点(T=750,770)')
plt.scatter(T_interp, V_cubic, color='green', marker='+', s=100, label='三次样条插值点(T=750,770)')

plt.title('过热蒸汽体积随温度变化的插值结果')
plt.xlabel('温度T(°C)')
plt.ylabel('体积V')
plt.grid(True)
plt.legend()
plt.show()