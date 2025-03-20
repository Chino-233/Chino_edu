import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt
from scipy.optimize import leastsq

# 定义函数 g(x)
def g(x, a, b):
    return 10 * a / (10 * b + (a - 10 * b) * np.exp(-a * np.sin(x)))

# 设置参数和生成数据
a_true = 1.1
b_true = 0.01
x_data = np.arange(1, 21)
y_data = g(x_data, a_true, b_true)

# (1) 使用 curve_fit (Python中的lsqcurvefit对应物)
print("方法1: 使用curve_fit (等价于MATLAB的lsqcurvefit)")
popt, pcov = curve_fit(lambda x, a, b: g(x, a, b), x_data, y_data, p0=[1, 0.01])
print(f"拟合参数: a = {popt[0]:.6f}, b = {popt[1]:.6f}")

# 计算拟合优度R^2
y_pred = g(x_data, *popt)
ss_res = np.sum((y_data - y_pred) ** 2)
ss_tot = np.sum((y_data - np.mean(y_data)) ** 2)
r_squared = 1 - (ss_res / ss_tot)
print(f"R²: {r_squared:.6f}")

# (2) 使用自定义拟合方法(对应MATLAB的fittype和fit)
print("\n方法2: 自定义拟合(对应MATLAB的fittype和fit)")
# 定义残差函数
def residuals(p, x, y):
    a, b = p
    return y - g(x, a, b)

# 初始猜测值
p0 = [1.0, 0.01]
# 使用leastsq拟合
plsq, success = leastsq(residuals, p0, args=(x_data, y_data))
print(f"拟合参数: a = {plsq[0]:.6f}, b = {plsq[1]:.6f}")

# 计算拟合优度R^2
y_fit = g(x_data, *plsq)
ss_res = np.sum((y_data - y_fit) ** 2)
r_squared = 1 - (ss_res / ss_tot)
print(f"R²: {r_squared:.6f}")

# 绘制结果
plt.figure(figsize=(10, 6))
plt.scatter(x_data, y_data, color='black', label='原始数据')
x_fine = np.linspace(1, 20, 1000)
plt.plot(x_fine, g(x_fine, *popt), 'r-', label=f'curve_fit拟合: a={popt[0]:.4f}, b={popt[1]:.4f}')
plt.plot(x_fine, g(x_fine, *plsq), 'g--', label=f'leastsq拟合: a={plsq[0]:.4f}, b={plsq[1]:.4f}')
plt.xlabel('x')
plt.ylabel('g(x)')
plt.legend()
plt.title('函数g(x)的拟合结果比较')
plt.grid(True)
plt.show()