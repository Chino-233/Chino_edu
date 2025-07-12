%% 数据导入与预处理
data = readtable('溶解氧导电率.csv', 'VariableNamingRule', 'preserve');

% 调整列索引（假设列顺序为：日期, 溶解氧, 导电率, 蜕壳数, 死亡数）
dissolved_oxygen = data{:, 2};   % 溶解氧（第二列）
conductivity = data{:, 3};       % 导电率（第三列）
death = data{:, 5};              % 死亡数（第五列）

% 清洗无效数据
valid = ~isnan(dissolved_oxygen) & ~isnan(conductivity) & ~isnan(death);
DO = dissolved_oxygen(valid);
COND = conductivity(valid);
DEATH = death(valid);

%% 双变量线性回归分析
figure('Position', [100 100 1200 500]);

% =====================
% 子图1：溶解氧 vs 死亡数
% =====================
subplot(1,2,1);
model_do = fitlm(DO, DEATH);

% 设置配色（蓝色系）
scatter_color = [100, 149, 237]/255;  % 矢车菊蓝
line_color = [0, 0, 139]/255;         % 深蓝色

% 绘制图形
scatter(DO, DEATH, 70,...
    'MarkerEdgeColor', scatter_color,...
    'MarkerFaceColor', scatter_color,...
    'MarkerEdgeAlpha', 0.7);
hold on;
x_range = linspace(min(DO), max(DO), 100);
plot(x_range, predict(model_do, x_range'),...
    'LineWidth', 2.5, 'Color', line_color);

% 图形标注
title('Dissolved Oxygen vs Death Count');
xlabel('DO (mg/L)');
ylabel('Death Count');
ylim([0 1]);  % 统一y轴范围
grid on;
box on;

% 统计标注
text(0.95, 0.90,...
    {sprintf('y = %.3fx + %.3f', model_do.Coefficients.Estimate(2), model_do.Coefficients.Estimate(1)),...
    sprintf('R² = %.3f', model_do.Rsquared.Adjusted)},...
    'Units', 'normalized',...
    'HorizontalAlignment', 'right',...
    'VerticalAlignment', 'top',...
    'BackgroundColor', [1 1 1 0.8],...
    'EdgeColor', [0.6 0.6 0.6],...
    'FontSize', 11);

% =====================
% 子图2：导电率 vs 死亡数
% =====================
subplot(1,2,2);
model_cond = fitlm(COND, DEATH);

% 设置配色（绿色系）
scatter_color = [152, 251, 152]/255;  % 浅绿色
line_color = [34, 139, 34]/255;       % 森林绿

% 绘制图形
scatter(COND, DEATH, 70,...
    'MarkerEdgeColor', scatter_color,...
    'MarkerFaceColor', scatter_color,...
    'MarkerEdgeAlpha', 0.7);
hold on;
x_range = linspace(min(COND), max(COND), 100);
plot(x_range, predict(model_cond, x_range'),...
    'LineWidth', 2.5, 'Color', line_color);

% 图形标注
title('Conductivity vs Death Count');
xlabel('Conductivity (μS/cm)');
ylabel('Death Count');
ylim([0 1]);  % 统一y轴范围
grid on;
box on;

% 统计标注
text(0.95, 0.90,...
    {sprintf('y = %.3fx + %.3f', model_cond.Coefficients.Estimate(2), model_cond.Coefficients.Estimate(1)),...
    sprintf('R² = %.3f', model_cond.Rsquared.Adjusted)},...
    'Units', 'normalized',...
    'HorizontalAlignment', 'right',...
    'VerticalAlignment', 'top',...
    'BackgroundColor', [1 1 1 0.8],...
    'EdgeColor', [0.6 0.6 0.6],...
    'FontSize', 11);

%% 输出回归函数
fprintf('\n=== 溶解氧回归函数 ===\n');
fprintf('y = %.4f·DO + %.4f\n', model_do.Coefficients.Estimate(2), model_do.Coefficients.Estimate(1));
fprintf('R² = %.3f (p=%.4f)\n', model_do.Rsquared.Adjusted, model_do.Coefficients.pValue(2));

fprintf('\n=== 导电率回归函数 ===\n');
fprintf('y = %.4f·COND + %.4f\n', model_cond.Coefficients.Estimate(2), model_cond.Coefficients.Estimate(1));
fprintf('R² = %.3f (p=%.4f)\n', model_cond.Rsquared.Adjusted, model_cond.Coefficients.pValue(2));

%% 图形美化
set(findall(gcf,'Type','axes'),...
    'FontName', 'Arial',...
    'FontSize', 11,...
    'LineWidth', 1.2,...
    'GridAlpha', 0.2);
set(gcf, 'Color', 'w');