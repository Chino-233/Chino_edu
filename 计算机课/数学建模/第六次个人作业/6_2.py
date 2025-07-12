import numpy as np
import matplotlib.pyplot as plt
from scipy.integrate import solve_ivp
from scipy.special import jv, yv
from scipy.interpolate import interp1d
import numpy.polynomial.polynomial as poly

plt.rcParams['font.sans-serif'] = ['SimHei']  
plt.rcParams['axes.unicode_minus'] = False  

# 参数设置
n = 1/2
x0 = np.pi/2
y0 = 2
y0_prime = -2/np.pi

# 1. 符号解(解析解)
def symbolic_solution(x):
    return np.sqrt(2*np.pi/x) * np.sin(x)

# 2. 数值解 - 转化为一阶方程组
def bessel_equation(x, y):
    y1, y2 = y
    dy1dx = y2
    dy2dx = -y2/x - (1 - n**2/x**2)*y1
    return [dy1dx, dy2dx]

# 求解数值解
x_span = [x0, 10]
sol = solve_ivp(bessel_equation, x_span, [y0, y0_prime], 
                method='RK45', dense_output=True, rtol=1e-6)

# 数值解函数
def numerical_solution(x):

    if isinstance(x, (int, float)):
        # 检查x是否在求解范围内
        if x < x_span[0] or x > x_span[1]:
            raise ValueError(f"x值{x}超出了求解范围[{x_span[0]}, {x_span[1]}]")
        return sol.sol(x)[0]
    else:
        # 处理数组输入
        x = np.asarray(x)
        # 检查所有值是否在范围内
        if np.any(x < x_span[0]) or np.any(x > x_span[1]):
            raise ValueError(f"x值中有超出求解范围[{x_span[0]}, {x_span[1]}]的值")
        return sol.sol(x)[0]

x_plot = np.linspace(x0, 10, 1000)
y_numerical = numerical_solution(x_plot)
y_analytical = symbolic_solution(x_plot)

# 绘制解的图形
plt.figure(figsize=(10, 6))
plt.plot(x_plot, y_numerical, 'b-', label='数值解')
plt.plot(x_plot, y_analytical, 'r--', label='符号解')
plt.grid(True)
plt.xlabel('x')
plt.ylabel('y')
plt.title(f'Bessel方程 $x^2y\'\' + xy\' + (x^2 - {n}^2)y = 0$ 的解')
plt.legend()
plt.plot(x0, y0, 'ko', label=f'初始点 ({x0:.2f}, {y0:.2f})')
plt.legend()

# 显示图形和解的表达式
plt.figtext(0.5, 0.01, 
            f'符号解: $y(x) = \\sqrt{{\\frac{{2\\pi}}{{x}}}} \\sin(x)$', 
            ha='center', fontsize=12)

plt.tight_layout()
plt.savefig('bessel_solution.png', dpi=300)
plt.show()

# 打印解的表达式
print("\nBessel方程的符号解:")
print("y(x) = sqrt(2π/x) * sin(x)")

z = (x_plot - x_plot.mean()) / (x_plot.max() - x_plot.min())  
coeffs = poly.polyfit(z, y_numerical, 5)
coeffs_rounded = np.round(coeffs, 3)

# 打印数值解的近似多项式表达式
print("\nBessel方程的数值解近似表达式:")
print("y(x) ≈ ", end="")
for i, c in enumerate(coeffs_rounded):
    if i == 0:
        print(f"{c:.3f}", end="")
    elif i == 1:
        print(f" + {c:.3f}·z", end="")
    else:
        print(f" + {c:.3f}·z^{i}", end="")
        
print("\n其中 z = (x - {:.3f}) / {:.3f}".format(x_plot.mean(), x_plot.max() - x_plot.min()))
