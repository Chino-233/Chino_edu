from pulp import *

# 创建问题实例，最大化目标函数
prob = LpProblem("Cargo_Loading", LpMaximize)

# 定义物品编号和对应的参数
items = [1, 2, 3, 4, 5, 6]
质量 = [21, 16, 18, 12, 9, 32]
体积 = [4, 2, 3, 2, 1, 4]
冷藏 = [1, 0, 1, 0, 0, 1]  # 需要冷藏标记为1
可燃指数 = [0.2, 0.2, 0.1, 0.2, 0.1, 0.3]
价值 = [12, 11, 13, 10, 9, 15]

# 定义决策变量（整数类型）
x = LpVariable.dicts("物品", items, lowBound=0, cat='Integer')

# 目标函数：总价值最大化
prob += lpSum([价值[i-1] * x[i] for i in items])

# 约束条件
prob += lpSum([质量[i-1] * x[i] for i in items]) <= 400000  # 载重量
prob += lpSum([体积[i-1] * x[i] for i in items]) <= 55000   # 总容积
prob += lpSum([体积[i-1] * x[i] for i in items if 冷藏[i-1] == 1]) <= 8500  # 冷藏容积
prob += lpSum([可燃指数[i-1] * x[i] for i in items]) <= 700  # 可燃指数

# 求解问题
prob.solve()

# 输出结果
print("状态:", LpStatus[prob.status])
print("最大总价值 =", value(prob.objective), "元\n")
print("各物品装载数量：")
for i in items:
    print(f"物品{i}: {int(x[i].value())} 件")