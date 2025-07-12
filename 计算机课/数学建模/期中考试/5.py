import numpy as np
from scipy.optimize import minimize

# 定义目标函数
def objective(x):
    production_cost = 50*x[0] + 0.2*x[0]**2 + 50*x[1] + 0.2*x[1]**2 + 50*x[2] + 0.2*x[2]**2
    s1 = x[0] - 40  # 第一季度末库存
    s2 = x[0] + x[1] - 100  # 第二季度末库存
    s3 = x[0] + x[1] + x[2] - 180  # 第三季度末库存
    storage_cost = 4 * (s1 + s2 + s3)
    return production_cost + storage_cost

# 约束条件
constraints = [
    {'type': 'ineq', 'fun': lambda x: x[0] + x[1] - 100},   # 第二季度累计生产需≥100
    {'type': 'ineq', 'fun': lambda x: x[0] + x[1] + x[2] - 180}  # 第三季度累计生产需≥180
]

# 变量边界
bounds = [(40, 100), (0, 100), (0, 100)]  # 各季度的生产量上下限

# 初始猜测
x0 = [40, 60, 80]  # 初始解：按季度需求生产

# 求解问题
result = minimize(objective, x0=x0, method='SLSQP', bounds=bounds, constraints=constraints)

# 输出结果
print("最优生产计划：")
print(f"第一季度生产：{round(result.x[0], 2)} 台")
print(f"第二季度生产：{round(result.x[1], 2)} 台")
print(f"第三季度生产：{round(result.x[2], 2)} 台")
print(f"总费用：{round(result.fun, 2)} 元")

# 判断唯一全局最优解 --------------------------------------------------------
print("\n[唯一全局最优解分析]")

# 分析1：目标函数是否为严格凸函数
# 生产费用部分 f(x_i) = 50x_i + 0.2x_i²，Hessian矩阵为对角阵，二阶导数=0.4 > 0
# 存储费用为线性函数，Hessian=0 → 整体Hessian正定
print("1. 目标函数分析：")
print("   - 生产费用项 f(x_i) = 50x_i + 0.2x_i² 是严格凸函数（二阶导数恒为0.4 > 0）")
print("   - 存储费用为线性函数，不改变凸性")
print("   ⇒ 目标函数整体为严格凸函数")

# 分析2：可行域是否为凸集
print("\n2. 约束条件分析：")
print("   - 线性不等式约束（累计生产≥需求）定义凸集")
print("   - 变量边界约束（0 ≤ x ≤ 100）为凸集")
print("   ⇒ 可行域是凸集")

# 结论
print("\n3. 结论：严格凸目标函数 + 凸可行域 → 存在唯一全局最优解")