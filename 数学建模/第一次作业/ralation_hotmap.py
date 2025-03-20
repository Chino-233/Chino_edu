import pandas as pd
import numpy as np
from scipy.stats import pearsonr, spearmanr, kendalltau
from sklearn.metrics import mutual_info_score

# 读取数据
df = pd.read_csv('9月11日后缺失值插值填充数据.csv')

# 选择需要分析的列
cols = ['AvgTN', '盐度1', 'pH1', '溶解氧', '导电率', '蜕壳数', '死亡数']
df_selected = df[cols]

# 计算各种相关系数
# 1. Pearson相关系数
pearson_corr = df_selected.corr(method='pearson')   

# 2. Spearman等级相关系数
spearman_corr = df_selected.corr(method='spearman')

# 3. Kendall's Tau相关系数
kendall_corr = df_selected.corr(method='kendall')

# 4. 互信息(Mutual Information)计算
def calculate_mi_matrix(df):
    n = df.shape[1]
    mi_matrix = np.zeros((n, n))
    for i in range(n):
        for j in range(n):
            if i == j:
                mi_matrix[i, j] = 1  # 对角线元素为1
            else:
                # 将数据标准化到相似尺度
                x = (df.iloc[:, i] - df.iloc[:, i].min()) / (df.iloc[:, i].max() - df.iloc[:, i].min())
                y = (df.iloc[:, j] - df.iloc[:, j].min()) / (df.iloc[:, j].max() - df.iloc[:, j].min())
                
                # 计算互信息并标准化
                mi = mutual_info_score(pd.qcut(x, 10, labels=False, duplicates='drop'), 
                                     pd.qcut(y, 10, labels=False, duplicates='drop'))
                mi_max = np.sqrt(mutual_info_score(pd.qcut(x, 10, labels=False, duplicates='drop'), 
                                               pd.qcut(x, 10, labels=False, duplicates='drop')) * 
                               mutual_info_score(pd.qcut(y, 10, labels=False, duplicates='drop'), 
                                               pd.qcut(y, 10, labels=False, duplicates='drop')))
                mi_matrix[i, j] = mi / mi_max if mi_max != 0 else 0
    
    return pd.DataFrame(mi_matrix, index=df.columns, columns=df.columns)

mi_corr = calculate_mi_matrix(df_selected)

# 将结果格式化为表格
# 只关注蜕壳数和死亡数与环境因素的相关性

# 蜕壳数相关性表格
molting_pearson = pearson_corr['蜕壳数'][:-1].sort_values(ascending=False)
molting_spearman = spearman_corr['蜕壳数'][:-1].sort_values(ascending=False)
molting_kendall = kendall_corr['蜕壳数'][:-1].sort_values(ascending=False)
molting_mi = mi_corr['蜕壳数'][:-1].sort_values(ascending=False)

# 死亡数相关性表格
death_pearson = pearson_corr['死亡数'][:-1].sort_values(ascending=False)
death_spearman = spearman_corr['死亡数'][:-1].sort_values(ascending=False)
death_kendall = kendall_corr['死亡数'][:-1].sort_values(ascending=False)
death_mi = mi_corr['死亡数'][:-1].sort_values(ascending=False)

# 创建一个整合的DataFrame
molting_table = pd.DataFrame({
    'Pearson系数': molting_pearson,
    'Spearman系数': molting_spearman,
    'Kendall系数': molting_kendall, 
    '互信息系数': molting_mi
})

death_table = pd.DataFrame({
    'Pearson系数': death_pearson,
    'Spearman系数': death_spearman,
    'Kendall系数': death_kendall,
    '互信息系数': death_mi
})

# 创建一个合并所有数据的字典
all_correlations = {
    "蜕壳数_Pearson": molting_pearson,
    "蜕壳数_Spearman": molting_spearman,
    "蜕壳数_Kendall": molting_kendall,
    "蜕壳数_互信息": molting_mi,
    "死亡数_Pearson": death_pearson,
    "死亡数_Spearman": death_spearman,
    "死亡数_Kendall": death_kendall,
    "死亡数_互信息": death_mi
}

# 将所有相关性数据合并到一个DataFrame中
all_corr_df = pd.DataFrame(all_correlations)

# 将完整相关系数矩阵也合并到一个Excel文件的不同sheet中
with pd.ExcelWriter('相关性分析结果.xlsx') as writer:
    all_corr_df.round(3).to_excel(writer, sheet_name='主要相关性')
    pearson_corr.round(3).to_excel(writer, sheet_name='Pearson相关系数矩阵')
    spearman_corr.round(3).to_excel(writer, sheet_name='Spearman相关系数矩阵')
    kendall_corr.round(3).to_excel(writer, sheet_name='Kendall相关系数矩阵')
    mi_corr.round(3).to_excel(writer, sheet_name='互信息系数矩阵')

# 也保存为单个CSV文件
all_corr_df.round(3).to_csv('相关性分析结果汇总.csv', encoding='utf-8-sig')

print("所有结果已保存为Excel文件和CSV文件")
