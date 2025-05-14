import numpy as np
import matplotlib.pyplot as plt
from scipy.integrate import solve_ivp

plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False

# 参数设置
d = 100    
v1 = 1     
v2 = 2    
k = v1/v2 

def analytical_solution():
    theta = np.arccos(-k)
    T = d / (v2 * np.sin(theta))

    t = np.linspace(0, T, 100)
    
    # 计算轨迹
    x_analytical = (v2 * np.cos(theta) + v1) * t
    y_analytical = v2 * np.sin(theta) * t
    
    return t, x_analytical, y_analytical, T, theta

def ode_system(t, z):
    x, y = z
    # 计算最优船头角度
    theta = np.arccos(-k)
    
    # 船的速度分量
    dx_dt = v2 * np.cos(theta) + v1
    dy_dt = v2 * np.sin(theta)
    
    return [dx_dt, dy_dt]

def numerical_solution():
    # 初始条件
    z0 = [0, 0]  # [x0, y0]
    
    # 估计积分终止时间（使用解析解的渡河时间）
    _, _, _, T_analytical, _ = analytical_solution()
    
    # 数值积分
    t_span = [0, 1.2 * T_analytical]  # 稍微延长一点确保能到达对岸
    sol = solve_ivp(ode_system, t_span, z0, method='RK45', 
                    dense_output=True, rtol=1e-6, atol=1e-9)
    
    # 提取结果
    t_dense = np.linspace(0, sol.t[-1], 100)
    z_dense = sol.sol(t_dense)
    x_numerical = z_dense[0]
    y_numerical = z_dense[1]
    
    # 找到恰好到达对岸的时间点
    idx = np.argmin(np.abs(y_numerical - d))
    T_numerical = t_dense[idx]
    
    return t_dense[:idx+1], x_numerical[:idx+1], y_numerical[:idx+1], T_numerical

# 计算解析解
t_analytical, x_analytical, y_analytical, T_analytical, theta = analytical_solution()

# 计算数值解
t_numerical, x_numerical, y_numerical, T_numerical = numerical_solution()

# 计算和绘图
plt.figure(figsize=(12, 10))

# 绘制航行轨迹
plt.subplot(2, 1, 1)
plt.plot(x_analytical, y_analytical, 'b-', label='解析解')
plt.plot(x_numerical, y_numerical, 'r--', label='数值解')
plt.scatter([0, 0], [0, d], c=['green', 'red'], s=100, 
           label='起点A和终点B')
plt.xlabel('x(m) - 沿河流方向')
plt.ylabel('y(m) - 垂直河流方向')
plt.title('小船渡河轨迹')
plt.grid(True)
plt.legend()
plt.axis('equal')

# 绘制位置随时间变化
plt.subplot(2, 2, 3)
plt.plot(t_analytical, x_analytical, 'b-', label='x(t) - 解析解')
plt.plot(t_numerical, x_numerical, 'r--', label='x(t) - 数值解')
plt.xlabel('时间 t(s)')
plt.ylabel('x 位置 (m)')
plt.grid(True)
plt.legend()

plt.subplot(2, 2, 4)
plt.plot(t_analytical, y_analytical, 'b-', label='y(t) - 解析解')
plt.plot(t_numerical, y_numerical, 'r--', label='y(t) - 数值解')
plt.xlabel('时间 t(s)')
plt.ylabel('y 位置 (m)')
plt.grid(True)
plt.legend()

plt.tight_layout()

# 输出计算结果
print(f"解析解结果:")
print(f"  船头角度: {np.degrees(theta):.2f}度 (相对于y轴)")
print(f"  渡河时间: {T_analytical:.2f}秒")
print(f"数值解结果:")
print(f"  渡河时间: {T_numerical:.2f}秒")
print(f"  到达终点时的坐标: ({x_numerical[-1]:.2f}, {y_numerical[-1]:.2f})")

plt.show()
