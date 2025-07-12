clc;
% 导入数据
x = [-2, -1.7, -1.4, -1.1, -0.8, -0.5, -0.2, 0.1, 0.4, 0.7, 1, 1.3, 1.6, 1.9, 2.2, 2.5, 2.8, 3.1, 3.4, 3.7, 4, 4.3, 4.6, 4.9];
y = [0.1029, 0.1174, 0.1316, 0.1448, 0.1566, 0.1662, 0.1733, 0.1775, 0.1785, 0.1764, 0.1711, 0.1630, 0.1526, 0.1402, 0.1266, 0.1122, 0.0977, 0.0835, 0.0702, 0.0588, 0.0479, 0.0373, 0.0291, 0.0224];
n = length(x);

% 第一部分：寻找最佳多项式阶数
max_degree = 10;
results = zeros(max_degree, 2); 

for degree = 1:max_degree
    p = polyfit(x, y, degree);
    y_fit = polyval(p, x);
    % 计算决定系数 R²
    r2 = 1 - sum((y - y_fit).^2) / sum((y - mean(y)).^2);
    adjusted_r2 = 1 - (1 - r2) * (n - 1) / (n - degree - 1);
    results(degree, :) = [degree, r2];
end

% 显示结果
fprintf('多项式拟合结果比较：\n');
fprintf('阶数\tR²\t调整R²\n');
fprintf('----------------------------------------\n');
for i = 1:max_degree
    fprintf('%d\t%.6f\t%.6f\n', ...
        results(i, 1), results(i, 2), ...
        1 - (1 - results(i, 2)) * (n - 1) / (n - i - 1));
end

% 阶数选择
best_degree = find(diff(results(:, 2)) < 0.01, 1) + 1;
if isempty(best_degree)
    best_degree = max_degree;
end
fprintf('\n根据R²增量分析，建议多项式阶数为: %d\n', best_degree);

% 计算多项式系数
poly_coef = polyfit(x, y, best_degree);

% 输出多项式系数
fprintf('\n多项式系数 (从高阶到低阶):\n');
for i = 1:length(poly_coef)
    fprintf('x^%d 系数: %.6e\n', best_degree-(i-1), poly_coef(i));
end

% 计算拟合值和残差标准差
y_poly = polyval(poly_coef, x);
residual = y - y_poly;
residual_std = sqrt(mean(residual.^2));
fprintf('\n残差标准差: %.6f\n', residual_std);

% 多项式表达式的字符串形式
poly_str = '';
for i = 1:length(poly_coef)
    power = best_degree - (i-1);
    coef = poly_coef(i);
    
    if i == 1
        poly_str = sprintf('%.6fx^%d', coef, power);
    else
        if coef >= 0
            sign = '+';
        else
            sign = '-';
            coef = abs(coef);
        end
        
        if power == 1
            poly_str = [poly_str, sprintf(' %s %.6fx', sign, coef)];
        elseif power == 0
            poly_str = [poly_str, sprintf(' %s %.6f', sign, coef)];
        else
            poly_str = [poly_str, sprintf(' %s %.6fx^%d', sign, coef, power)];
        end
    end
end

% 计算95%置信区间
confidence_interval = 1.96 * residual_std;

% 计算拟合优度指标
r_squared = 1 - sum((y - y_poly).^2) / sum((y - mean(y)).^2);
adjusted_r2 = 1 - (1 - r_squared) * (n - 1) / (n - best_degree - 1);
fprintf('拟合优度 R² = %.6f\n', r_squared);
fprintf('调整R² = %.6f\n', adjusted_r2);
fprintf('多项式表达式：\ny = %s\n', poly_str);
fprintf('95%% 置信区间: ±%.4f\n', confidence_interval);

% 绘制图像
figure('Position', [100, 100, 900, 600]);
subplot(2, 1, 1);
plot(x, y, 'bo', 'MarkerSize', 6, 'DisplayName', '原始数据点');
hold on;
x_fine = linspace(min(x), max(x), 200);
y_fine = polyval(poly_coef, x_fine);
plot(x_fine, y_fine, 'r-', 'LineWidth', 1.5, 'DisplayName', [num2str(best_degree), '阶多项式拟合']);
xlabel('x');
ylabel('y');
title('多项式拟合结果', 'FontSize', 12);
grid on;
legend('Location', 'best');


text(0.05, 0.85, ['y = ', poly_str], 'Units', 'normalized', 'FontSize', 9);
text(0.05, 0.78, ['R² = ', num2str(r_squared, '%.6f')], 'Units', 'normalized');
text(0.05, 0.71, ['调整R² = ', num2str(adjusted_r2, '%.6f')], 'Units', 'normalized');

% 绘制残差图
subplot(2, 1, 2);
stem(x, residual, 'filled', 'MarkerSize', 4);
xlabel('x');
ylabel('残差 (y - y_{fit})');
title('残差分析', 'FontSize', 12);
grid on;
hold on;
plot([min(x), max(x)], [0, 0], 'k--', 'LineWidth', 1);
plot([min(x), max(x)], [confidence_interval, confidence_interval], 'r--', 'LineWidth', 1);
plot([min(x), max(x)], [-confidence_interval, -confidence_interval], 'r--', 'LineWidth', 1);
text(0.05, 0.9, ['95% 置信区间: \pm', num2str(confidence_interval, '%.4f')], ...
    'Units', 'normalized', 'Color', 'r');