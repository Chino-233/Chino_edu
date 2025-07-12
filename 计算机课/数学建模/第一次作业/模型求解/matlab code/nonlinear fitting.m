% ==================== 数据读取与预处理 ====================

% 读取温度、盐度、pH 值数据
data1 = readtable('温度盐度ph.csv', 'ReadVariableNames', true);
if ~istable(data1)
    error('数据必须是表格类型。请确保数据已正确加载。');
end

% 提取温度、盐度、pH 值和蜕壳数
temperature_raw = data1{:, 2};
salinity_raw = data1{:, 3};
ph_raw = data1{:, 4};
moltingCount1_raw = data1{:, 6};

% 删除缺失值
valid_idx1 = all(~isnan([temperature_raw, salinity_raw, ph_raw, moltingCount1_raw]), 2);
temperature = temperature_raw(valid_idx1);
salinity = salinity_raw(valid_idx1);
ph = ph_raw(valid_idx1);
moltingCount1 = moltingCount1_raw(valid_idx1);

% 检查数据维度
assert(isequal(numel(temperature), numel(salinity), numel(ph), numel(moltingCount1)),...
       '变量维度不一致');

% 读取溶解氧数据
data2 = readtable('溶解氧导电率.csv', 'ReadVariableNames', true);
if ~istable(data2)
    error('数据必须是表格类型。请确保数据已正确加载。');
end

% 提取溶解氧和蜕壳数
do_raw = data2{:, 2};
moltingCount2_raw = data2{:, 4};

% 删除缺失值
valid_idx2 = all(~isnan([do_raw, moltingCount2_raw]), 2);
do_data = do_raw(valid_idx2);
moltingCount2 = moltingCount2_raw(valid_idx2);

% 检查数据维度
assert(isequal(numel(do_data), numel(moltingCount2)),...
       '变量维度不一致');

% ==================== 多项式拟合分析 ====================

% 定义分析参数
predictor_names = {'温度', '盐度', 'pH值', '溶解氧'};
predictors = {temperature, salinity, ph, do_data};
colors = {'#FF4500', '#32CD32', '#4169E1', '#FF1493'};  % 橙、绿、蓝、粉红
max_order = 5;

% 初始化存储结果的变量
results = struct();

% 创建 figure
figure('Position', [100, 100, 1200, 800], 'Color', 'w');

% 对每个自变量分别进行多项式拟合
for p = 1:4
    % 提取当前自变量和因变量
    x_data = predictors{p};
    if p <= 3
        moltingCount = moltingCount1; % 温度、盐度、pH 值的蜕壳数
    else
        moltingCount = moltingCount2; % 溶解氧的蜕壳数
    end
    
    % 寻找最佳阶数
    adj_rsq = zeros(max_order, 1);
    for order = 1:max_order
        X = zeros(length(x_data), order);
        for o = 1:order
            X(:, o) = x_data.^o;
        end
        
        try
            model = fitlm(X, moltingCount);
            adj_rsq(order) = model.Rsquared.Adjusted;
        catch
            adj_rsq(order) = -inf;
        end
    end
    
    [~, best_order] = max(adj_rsq);
    
    % 最终模型拟合
    X = zeros(length(x_data), best_order);
    for o = 1:best_order
        X(:, o) = x_data.^o;
    end
    final_model = fitlm(X, moltingCount);
    
    % 提取拟合函数
    coefficients = final_model.Coefficients.Estimate;
    equation = 'y = ';
    for o = 1:best_order
        equation = [equation, sprintf('%.4f x^%d + ', coefficients(o), o)];
    end
    equation = [equation(1:end-3), ' + ', num2str(coefficients(end))];
    results(p).Predictor = predictor_names{p};
    results(p).Equation = equation;
    results(p).Rsquared = final_model.Rsquared.Adjusted;
    results(p).Order = best_order;
    
    % 生成拟合曲线数据
    x_fit = linspace(min(x_data), max(x_data), 100)';
    X_fit = zeros(100, best_order);
    for o = 1:best_order
        X_fit(:, o) = x_fit.^o;
    end
    y_fit = predict(final_model, X_fit);
    
    % 绘制子图
    subplot(2, 2, p);
    hold on;
    
    % 绘制原始数据点
    scatter(x_data, moltingCount, 40, 'MarkerEdgeColor', colors{p},...
        'MarkerFaceColor', [0.7 0.7 0.7], 'LineWidth', 1.2, 'DisplayName', '原始数据');
    
    % 绘制拟合曲线
    plot(x_fit, y_fit, 'LineWidth', 2.5, 'Color', colors{p}, 'DisplayName', '拟合曲线');
    
    % 设置 y 轴范围为 0 到 1
    ylim([0, 1]);
    
    % 添加图例
    legend('Location', 'best', 'FontSize', 10);
    
    % 添加标题和标签
    title(sprintf('%s与蜕壳数的多项式拟合', predictor_names{p}),...
        'FontSize', 12, 'FontWeight', 'bold', 'Color', [0.2 0.2 0.2]);
    xlabel(predictor_names{p}, 'FontSize', 10, 'FontWeight', 'bold');
    ylabel('蜕壳数', 'FontSize', 10, 'FontWeight', 'bold');
    
    % 添加统计信息
    text(0.05, 0.92, sprintf('R²_{adj} = %.3f', final_model.Rsquared.Adjusted),...
        'Units', 'normalized', 'FontSize', 10, 'Color', [0.2 0.2 0.2],...
        'BackgroundColor', [1 1 1 0.7]);
    
    text(0.05, 0.82, sprintf('最佳阶数: %d', best_order),...
        'Units', 'normalized', 'FontSize', 10, 'Color', [0.2 0.2 0.2],...
        'BackgroundColor', [1 1 1 0.7]);
    
    % 美化图形
    set(gca, 'FontSize', 10, 'LineWidth', 1.2, 'Box', 'on',...
        'XColor', [0.3 0.3 0.3], 'YColor', [0.3 0.3 0.3]);
    grid on;
end

% 调整子图间距
set(gcf, 'Color', 'w');
set(gca, 'LooseInset', get(gca, 'TightInset'));

% ==================== 输出拟合函数 ====================

fprintf('===== 拟合函数 =====\n');
for p = 1:4
    fprintf('自变量: %s\n', results(p).Predictor);
    fprintf('最佳拟合函数: %s\n', results(p).Equation);
    fprintf('调整R²: %.3f\n', results(p).Rsquared);
    fprintf('最佳阶数: %d\n\n', results(p).Order);
end