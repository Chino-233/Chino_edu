% 读取Excel数据（保留原始列标题）
data = readtable('9.12后完整数据.xls', 'Sheet', 'Sheet1', 'VariableNamingRule', 'preserve');

% 删除日期为空的行（通过列位置索引，假设日期在第一列）
dateColumn = data(:, 1);  % 第一列为日期
validRows = ~ismissing(dateColumn);
data = data(validRows, :);

% 提取自变量和因变量
X = data{:, 2:6};  % 温度(2)、盐度(3)、ph(4)、溶解氧(5)、导电率(6)
Y_death = data{:, 8};   % 死亡数（第8列）
Y_molt = data{:, 7};    % 蜕壳数（第7列）

% 删除因变量中的异常值（死亡数非负）
valid_idx = Y_death >= 0 & Y_molt >= 0;
X = X(valid_idx, :);
Y_death = Y_death(valid_idx);
Y_molt = Y_molt(valid_idx);

% 多元回归分析：死亡数
model_death = fitlm(X, Y_death, ...
    'VarNames', {'Temperature', 'Salinity', 'pH', 'DO', 'Conductivity', 'Death'});
fprintf('=== 死亡数回归分析结果 ===\n');
disp(model_death);
fprintf('回归方程：Death = %.4f + %.4f*Temp + %.4f*Sal + %.4f*pH + %.4f*DO + %.4f*Cond\n', ...
    model_death.Coefficients.Estimate);
fprintf('调整后R² = %.4f\n\n', model_death.Rsquared.Adjusted);

% 多元回归分析：蜕壳数
model_molt = fitlm(X, Y_molt, ...
    'VarNames', {'Temperature', 'Salinity', 'pH', 'DO', 'Conductivity', 'Molt'});
fprintf('=== 蜕壳数回归分析结果 ===\n');
disp(model_molt);
fprintf('回归方程：Molt = %.4f + %.4f*Temp + %.4f*Sal + %.4f*pH + %.4f*DO + %.4f*Cond\n', ...
    model_molt.Coefficients.Estimate);
fprintf('调整后R² = %.4f\n', model_molt.Rsquared.Adjusted);