import numpy as np
import matplotlib.pyplot as plt
from scipy.integrate import solve_ivp
from numpy.polynomial import Polynomial

plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False

def define_system():
    def system(t, z):
        x, y = z
        dx_dt = -x**3 - y
        dy_dt = x - y**3
        return [dx_dt, dy_dt]
    return system

def solve_system(t_span=(0, 30), z0=[1, 0.5], points=1000):
    system = define_system()
    # 求解微分方程
    sol = solve_ivp(system, t_span, z0, method='RK45', dense_output=True, rtol=1e-5)
    # 创建更密集的时间点
    t = np.linspace(t_span[0], t_span[1], points)
    z = sol.sol(t)
    return t, z[0], z[1]  # 返回时间t、x(t)和y(t)

def fit_polynomial(t, values, degree=10):
    coefs = np.polyfit(t, values, degree)
    coefs[np.abs(coefs) < 1e-10] = 0
    expr = ""
    for i, coef in enumerate(coefs):
        power = degree - i
        if abs(coef) < 1e-10:  # 忽略非常小的系数
            continue
        
        if expr and coef > 0:
            expr += " + "
        elif expr and coef < 0:
            expr += " - "
            coef = abs(coef)
        elif coef < 0 and not expr:  # 如果是第一项且为负
            expr += "-"
            coef = abs(coef)
        
        if power == 0:
            expr += f"{coef:.6f}"
        elif power == 1:
            expr += f"{coef:.6f}t"
        else:
            expr += f"{coef:.6f}t^{power}"
    
    return expr

def print_function_expressions(t, x, y):
    print("微分方程组的数值解函数表达式（多项式拟合）：")
    
    # 获取多项式拟合的函数表达式
    x_expr = fit_polynomial(t, x)
    y_expr = fit_polynomial(t, y)
    
    print("\nx(t) = " + x_expr)
    print("\ny(t) = " + y_expr)
    

def plot_solution(t, x, y):
    plt.figure(figsize=(15, 5))
    
    # 1. 画出x(t)和y(t)的解曲线
    plt.subplot(1, 2, 1)
    plt.plot(t, x, 'b-', label='x(t)')
    plt.plot(t, y, 'r-', label='y(t)')
    plt.xlabel('t')
    plt.ylabel('值')
    plt.title('x(t)和y(t)的解曲线')
    plt.grid(True)
    plt.legend()
    
    # 2. 相平面上的轨线
    plt.subplot(1, 2, 2)
    plt.plot(x, y, 'g-')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.title('相平面轨线')
    plt.grid(True)
    
    plt.tight_layout()
    plt.show()

def main():
    # 求解微分方程组
    t, x, y = solve_system()
    
    # 输出x(t)和y(t)的函数表达式
    print_function_expressions(t, x, y)
    
    # 绘制图形
    plot_solution(t, x, y)

if __name__ == "__main__":
    main()