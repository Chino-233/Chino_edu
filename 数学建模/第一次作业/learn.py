import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.feature_selection import SelectKBest, f_regression, mutual_info_regression
from sklearn.ensemble import RandomForestRegressor
from sklearn.decomposition import PCA
from sklearn.preprocessing import StandardScaler
import seaborn as sns
from sklearn.linear_model import Lasso, LinearRegression
from sklearn.model_selection import cross_val_score, KFold
from sklearn.metrics import mean_squared_error, r2_score
from sklearn.inspection import permutation_importance

# 设置中文字体
plt.rcParams['font.sans-serif'] = ['SimHei']  # 用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False    # 用来正常显示负号

# 读取数据
df = pd.read_csv('9月11日后缺失值插值填充数据.csv')

# 删除'日期'和'备注'列，这些不是数值特征
if '日期' in df.columns:
    df = df.drop('日期', axis=1)
if '备注' in df.columns:
    df = df.drop('备注', axis=1)

# 打印基本信息
print("数据基本信息:")
print(df.info())
print("\n描述性统计:")
print(df.describe().round(3))

# 1. 皮尔逊相关系数矩阵（线性相关性）
print("\n1. 皮尔逊相关系数矩阵:")
pearson_corr = df.corr(method='pearson')
print(pearson_corr.round(3))

# 分析蜕壳数与环境因素的相关性
print("\n蜕壳数与环境因素的相关性(皮尔逊):")
print(pearson_corr['蜕壳数'].drop('蜕壳数').drop('死亡数').sort_values(ascending=False).round(3))

# 分析死亡数与环境因素的相关性
print("\n死亡数与环境因素的相关性(皮尔逊):")
print(pearson_corr['死亡数'].drop('死亡数').drop('蜕壳数').sort_values(ascending=False).round(3))

# 2. 使用互信息(Mutual Information)分析非线性相关性
# 准备特征和目标变量
X = df.drop(['蜕壳数', '死亡数'], axis=1)
y_molting = df['蜕壳数']
y_death = df['死亡数']

# 计算互信息
mi_molting = mutual_info_regression(X, y_molting)
mi_death = mutual_info_regression(X, y_death)

# 结果整理
mi_scores_molting = pd.Series(mi_molting, index=X.columns).sort_values(ascending=False)
mi_scores_death = pd.Series(mi_death, index=X.columns).sort_values(ascending=False)

print("\n2. 基于互信息的特征重要性(非线性相关):")
print("\n蜕壳数的互信息分数:")
print(mi_scores_molting.round(3))
print("\n死亡数的互信息分数:")
print(mi_scores_death.round(3))

# 3. 随机森林特征重要性
print("\n3. 随机森林特征重要性:")

# 训练随机森林模型(蜕壳数)
rf_molting = RandomForestRegressor(n_estimators=100, random_state=42)
rf_molting.fit(X, y_molting)
rf_importance_molting = pd.Series(rf_molting.feature_importances_, index=X.columns).sort_values(ascending=False)

print("\n蜕壳数的随机森林特征重要性:")
print(rf_importance_molting.round(3))

# 训练随机森林模型(死亡数)
rf_death = RandomForestRegressor(n_estimators=100, random_state=42)
rf_death.fit(X, y_death)
rf_importance_death = pd.Series(rf_death.feature_importances_, index=X.columns).sort_values(ascending=False)

print("\n死亡数的随机森林特征重要性:")
print(rf_importance_death.round(3))

# 4. 排序特征选择 (F值统计)
print("\n4. 基于F检验的特征选择:")

# 蜕壳数的特征选择
selector_molting = SelectKBest(f_regression, k='all')
selector_molting.fit(X, y_molting)
f_scores_molting = pd.Series(selector_molting.scores_, index=X.columns).sort_values(ascending=False)
p_values_molting = pd.Series(selector_molting.pvalues_, index=X.columns).sort_values()

print("\n蜕壳数的F检验分数:")
print(f_scores_molting.round(3))
print("\n蜕壳数的P值(越小越显著):")
print(p_values_molting.round(4))

# 死亡数的特征选择
selector_death = SelectKBest(f_regression, k='all')
selector_death.fit(X, y_death)
f_scores_death = pd.Series(selector_death.scores_, index=X.columns).sort_values(ascending=False)
p_values_death = pd.Series(selector_death.pvalues_, index=X.columns).sort_values()

print("\n死亡数的F检验分数:")
print(f_scores_death.round(3))
print("\n死亡数的P值(越小越显著):")
print(p_values_death.round(4))

# 5. Lasso回归特征重要性
print("\n5. Lasso回归特征选择:")

# 标准化数据
scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)

# 蜕壳数的Lasso回归
lasso_molting = Lasso(alpha=0.1)
lasso_molting.fit(X_scaled, y_molting)
lasso_importance_molting = pd.Series(np.abs(lasso_molting.coef_), index=X.columns).sort_values(ascending=False)

print("\n蜕壳数的Lasso特征系数:")
print(lasso_importance_molting.round(3))

# 死亡数的Lasso回归
lasso_death = Lasso(alpha=0.1)
lasso_death.fit(X_scaled, y_death)
lasso_importance_death = pd.Series(np.abs(lasso_death.coef_), index=X.columns).sort_values(ascending=False)

print("\n死亡数的Lasso特征系数:")
print(lasso_importance_death.round(3))

# 6. 排列重要性 (更稳健的特征重要性评估方法)
print("\n6. 排列重要性评估:")

# 线性回归模型(蜕壳数)
lr_molting = LinearRegression()
lr_molting.fit(X, y_molting)
perm_importance_molting = permutation_importance(lr_molting, X, y_molting, n_repeats=10, random_state=42)
perm_importance_molting_mean = pd.Series(perm_importance_molting.importances_mean, index=X.columns).sort_values(ascending=False)

print("\n蜕壳数的排列重要性:")
print(perm_importance_molting_mean.round(3))

# 线性回归模型(死亡数)
lr_death = LinearRegression()
lr_death.fit(X, y_death)
perm_importance_death = permutation_importance(lr_death, X, y_death, n_repeats=10, random_state=42)
perm_importance_death_mean = pd.Series(perm_importance_death.importances_mean, index=X.columns).sort_values(ascending=False)

print("\n死亡数的排列重要性:")
print(perm_importance_death_mean.round(3))

# 7. 综合结果分析
print("\n7. 综合分析结果:")

# 创建综合特征重要性表格(蜕壳数)
molting_importance = pd.DataFrame({
    '皮尔逊相关系数': pearson_corr['蜕壳数'].drop(['蜕壳数', '死亡数']),
    '互信息分数': mi_scores_molting,
    '随机森林重要性': rf_importance_molting,
    'F检验分数': f_scores_molting,
    'F检验P值': p_values_molting,
    'Lasso系数': lasso_importance_molting,
    '排列重要性': perm_importance_molting_mean
})

print("\n蜕壳数的综合特征重要性分析:")
print(molting_importance.round(3))

# 创建综合特征重要性表格(死亡数)
death_importance = pd.DataFrame({
    '皮尔逊相关系数': pearson_corr['死亡数'].drop(['死亡数', '蜕壳数']),
    '互信息分数': mi_scores_death,
    '随机森林重要性': rf_importance_death,
    'F检验分数': f_scores_death,
    'F检验P值': p_values_death,
    'Lasso系数': lasso_importance_death,
    '排列重要性': perm_importance_death_mean
})

print("\n死亡数的综合特征重要性分析:")
print(death_importance.round(3))

# 将结果保存到Excel文件
with pd.ExcelWriter('机器学习相关性分析结果.xlsx') as writer:
    molting_importance.round(3).to_excel(writer, sheet_name='蜕壳数特征重要性')
    death_importance.round(3).to_excel(writer, sheet_name='死亡数特征重要性')
    pearson_corr.round(3).to_excel(writer, sheet_name='皮尔逊相关系数矩阵')

print("\n分析结果已保存到'机器学习相关性分析结果.xlsx'文件")