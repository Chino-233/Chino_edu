%% 数据导入与预处理
data = readtable('溶解氧导电率.csv', 'VariableNamingRule', 'preserve');

% 调整列索引（假设列顺序为：日期, 溶解氧, 导电率, 蜕壳数）
dissolved_oxygen = data{:, 2};   % 溶解氧（第二列）
moulting = data{:, 4};           % 蜕壳数（第四列）

% 清洗无效数据
valid = ~isnan(dissolved_oxygen) & ~isnan(moulting);
dissolved_oxygen = dissolved_oxygen(valid);
moulting = moulting(valid);

%% 线性回归建模
model_do = fitlm(dissolved_oxygen, moulting);

%% 生成可调用预测函数
% 获取回归系数
beta0 = model_do.Coefficients.Estimate(1);  % 截距
beta1 = model_do.Coefficients.Estimate(2);  % 斜率

% 创建匿名函数
predict_moulting = @(x) beta1 * x + beta0;

%% 格式化输出回归函数
fprintf('\n=== 溶解氧与蜕壳数回归函数 ===\n');
fprintf('数学表达式: y = %.4f·x + %.4f\n', beta1, beta0);
fprintf('LaTeX公式: $y = %.4f x + %.4f$\n', beta1, beta0);
fprintf('MATLAB函数句柄: predict_moulting = @(x) %.4f*x + %.4f\n\n', beta1, beta0);

%% 可视化验证（紫色系配色）
figure('Position', [100 100 800 600]);

% 绘制散点图与回归线
scatter(dissolved_oxygen, moulting, 70,...
    'MarkerEdgeColor', [147 112 219]/255,...
    'MarkerFaceColor', [147 112 219]/255,...
    'MarkerEdgeAlpha', 0.7);
hold on;

% 生成预测曲线
x_range = linspace(min(dissolved_oxygen), max(dissolved_oxygen), 100);
y_pred = predict_moulting(x_range);
plot(x_range, y_pred, 'LineWidth', 2.5, 'Color', [75 0 130]/255);

% 图形标注
title('Dissolved Oxygen vs Moulting Regression');
xlabel('Dissolved Oxygen (mg/L)');
ylabel('Moulting Count');
grid on;
box on;

% 添加统计信息（右上角）
text(0.95, 0.90,...
    {sprintf('y = %.4fx + %.4f', beta1, beta0),...
    sprintf('R² = %.3f', model_do.Rsquared.Adjusted)},...
    'Units', 'normalized',...
    'HorizontalAlignment', 'right',...
    'VerticalAlignment', 'top',...
    'BackgroundColor', [1 1 1 0.8],...
    'EdgeColor', [0.6 0.6 0.6],...
    'FontSize', 12);

%% 函数验证示例
sample_do = mean(dissolved_oxygen); % 取溶解氧平均值
predicted_moulting = predict_moulting(sample_do);
fprintf('=== 函数验证 ===\n');
fprintf('当溶解氧为 %.2f mg/L 时，预测蜕壳数 = %.2f\n', sample_do, predicted_moulting);