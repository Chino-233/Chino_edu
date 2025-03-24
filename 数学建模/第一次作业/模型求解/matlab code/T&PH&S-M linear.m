%% 数据导入与预处理
data = readtable('温度盐度ph.csv', 'VariableNamingRule', 'preserve');

% 调整列索引（假设列顺序为：日期, 温度, 盐度, pH, 蜕壳数）
temp = data{:, 2};       % 温度
salinity = data{:, 3};   % 盐度
ph = data{:, 4};         % pH
moulting = data{:, 5};   % 蜕壳数

% 清洗无效数据
valid = ~isnan(temp) & ~isnan(salinity) & ~isnan(ph) & ~isnan(moulting);
temp = temp(valid);
salinity = salinity(valid);
ph = ph(valid);
moulting = moulting(valid);

%% 线性回归及绘图（低饱和度配色方案）
figure('Position', [100 100 1200 1000]);

% ----------------------
% 子图1：温度 vs 蜕壳数（珊瑚色系）
% ----------------------
subplot(2, 2, 1);
model_temp = fitlm(temp, moulting);
% 设置颜色（低饱和度珊瑚色）
scatter_color = [255, 127, 80]/255;    % 珊瑚色散点
line_color = [205, 92, 92]/255;        % 印度红回归线

scatter(temp, moulting, 70,...
    'MarkerEdgeColor', scatter_color,...
    'MarkerFaceColor', scatter_color,...
    'MarkerEdgeAlpha', 0.7); 
hold on;
x_range = linspace(min(temp), max(temp), 100);
[ypred, ~] = predict(model_temp, x_range');
plot(x_range, ypred, 'LineWidth', 2.5, 'Color', line_color);
title('Temperature vs Moulting');
xlabel('Temperature (°C)');
ylabel('Moulting Count');
grid on;
box on;

% 添加统计信息（居右显示）
text(0.95, 0.90,...
    sprintf('y = %.2fx + %.2f\nR² = %.3f',...
    model_temp.Coefficients.Estimate(2),...
    model_temp.Coefficients.Estimate(1),...
    model_temp.Rsquared.Adjusted),...
    'Units', 'normalized',...
    'HorizontalAlignment', 'right',...
    'BackgroundColor', [1 1 1 0.8],...
    'EdgeColor', [0.5 0.5 0.5]);

% ----------------------
% 子图2：盐度 vs 蜕壳数（灰绿色系）
% ----------------------
subplot(2, 2, 2);
model_salinity = fitlm(salinity, moulting);
% 设置颜色（低饱和度灰绿色）
scatter_color = [144, 238, 144]/255;   % 浅绿色散点
line_color = [34, 139, 34]/255;        % 森林绿回归线

scatter(salinity, moulting, 70,...
    'MarkerEdgeColor', scatter_color,...
    'MarkerFaceColor', scatter_color,...
    'MarkerEdgeAlpha', 0.7);
hold on;
x_range = linspace(min(salinity), max(salinity), 100);
[ypred, ~] = predict(model_salinity, x_range');
plot(x_range, ypred, 'LineWidth', 2.5, 'Color', line_color);
title('Salinity vs Moulting');
xlabel('Salinity (ppt)');
ylabel('Moulting Count');
grid on;
box on;

% 统计信息标注
text(0.95, 0.90,...
    sprintf('y = %.2fx + %.2f\nR² = %.3f',...
    model_salinity.Coefficients.Estimate(2),...
    model_salinity.Coefficients.Estimate(1),...
    model_salinity.Rsquared.Adjusted),...
    'Units', 'normalized',...
    'HorizontalAlignment', 'right',...
    'BackgroundColor', [1 1 1 0.8],...
    'EdgeColor', [0.5 0.5 0.5]);

% ----------------------
% 子图3：pH vs 蜕壳数（淡蓝色系）
% ----------------------
subplot(2, 2, 3);
model_ph = fitlm(ph, moulting);
% 设置颜色（低饱和度蓝）
scatter_color = [173, 216, 230]/255;   % 淡蓝色散点
line_color = [70, 130, 180]/255;       % 钢蓝色回归线

scatter(ph, moulting, 70,...
    'MarkerEdgeColor', scatter_color,...
    'MarkerFaceColor', scatter_color,...
    'MarkerEdgeAlpha', 0.7);
hold on;
x_range = linspace(min(ph), max(ph), 100);
[ypred, ~] = predict(model_ph, x_range');
plot(x_range, ypred, 'LineWidth', 2.5, 'Color', line_color);
title('pH vs Moulting');
xlabel('pH Value');
ylabel('Moulting Count');
grid on;
box on;

% 统计信息标注
text(0.95, 0.90,...
    sprintf('y = %.2fx + %.2f\nR² = %.3f',...
    model_ph.Coefficients.Estimate(2),...
    model_ph.Coefficients.Estimate(1),...
    model_ph.Rsquared.Adjusted),...
    'Units', 'normalized',...
    'HorizontalAlignment', 'right',...
    'BackgroundColor', [1 1 1 0.8],...
    'EdgeColor', [0.5 0.5 0.5]);

%% 增强图形美观度
% 设置全局字体
set(gcf, 'DefaultAxesFontName', 'Arial', 'DefaultTextFontName', 'Arial');
% 统一坐标轴风格
set(findall(gcf,'Type','axes'),...
    'Box','on',...
    'LineWidth', 1.2,...
    'FontSize', 11,...
    'GridAlpha', 0.2);
% 添加整体标题
sgtitle('Environmental Factors vs Moulting Counts',...
    'FontSize', 14, 'FontWeight', 'bold', 'Color', [0.3 0.3 0.3]);