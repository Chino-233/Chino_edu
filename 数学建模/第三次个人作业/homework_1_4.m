% 货机装载问题线性规划求解
% 定义变量表示各货舱中各种货物的装载量(吨)
% x_ij: i表示货舱(1-前舱,2-中舱,3-后舱), j表示货物类型(1,2,3,4)

% 常量定义
maxWeight = [10, 16, 8];  % 各货舱质量限制(吨)
maxVolume = [6800, 8700, 5300];  % 各货舱体积限制(立方米)
unitVolume = [480, 650, 580, 390];  % 各货物单位体积(立方米/吨)
profit = [3100, 3800, 3500, 2850];  % 各货物单位利润(元/吨)

% 定义变量 [x_11, x_12, x_13, x_14, x_21, x_22, x_23, x_24, x_31, x_32, x_33, x_34]
c = -[profit, profit, profit];  % 目标函数系数(取负值转为最小化问题)

% 约束条件
% 1. 质量约束: 各货舱装载总质量不超过最大限制
A_weight = zeros(3, 12);
for i = 1:3
    A_weight(i, (i-1)*4+1:(i-1)*4+4) = 1;
end
b_weight = maxWeight';

% 2. 体积约束: 各货舱装载总体积不超过最大限制
A_volume = zeros(3, 12);
for i = 1:3
    for j = 1:4
        A_volume(i, (i-1)*4+j) = unitVolume(j);
    end
end
b_volume = maxVolume';

% 3. 平衡约束: 各货舱装载质量与最大容许量成比例
% 16*(前舱总质量) = 10*(中舱总质量)
% 8*(中舱总质量) = 16*(后舱总质量)
A_balance = zeros(2, 12);
A_balance(1, 1:4) = 16;
A_balance(1, 5:8) = -10;
A_balance(2, 5:8) = 8;
A_balance(2, 9:12) = -16;
b_balance = [0; 0];

% 组合所有约束
A = [A_weight; A_volume];
b = [b_weight; b_volume];
Aeq = A_balance;
beq = b_balance;

% 非负约束
lb = zeros(12, 1);

% 求解线性规划问题
options = optimoptions('linprog', 'Display', 'iter');
[x, fval] = linprog(c, A, b, Aeq, beq, lb, [], options);

% 输出结果
fprintf('最优解：\n');
fprintf('总利润：%.2f元\n', -fval);

% 输出各货舱装载情况
cargoNames = {'货物1', '货物2', '货物3', '货物4'};
cabinNames = {'前舱', '中舱', '后舱'};

fprintf('\n装载方案：\n');
fprintf('%-10s%-10s%-10s%-10s\n', '货舱', '货物类型', '装载量(吨)', '占用体积(m³)');
fprintf('-----------------------------------------------\n');

for i = 1:3
    for j = 1:4
        idx = (i-1)*4 + j;
        if x(idx) > 1e-5  % 只显示非零装载量
            fprintf('%-10s%-10s%-10.2f%-10.2f\n', cabinNames{i}, cargoNames{j}, ...
                x(idx), x(idx) * unitVolume(j));
        end
    end
end

% 验证约束条件
fprintf('\n约束条件验证：\n');
for i = 1:3
    totalWeight = sum(x((i-1)*4+1:(i-1)*4+4));
    totalVolume = sum(x((i-1)*4+1:(i-1)*4+4) .* unitVolume');
    fprintf('%s: 装载总质量 = %.2f吨 (限制%.2f吨), 装载总体积 = %.2f m³ (限制%.2f m³), 装载率 = %.2f%%\n', ...
        cabinNames{i}, totalWeight, maxWeight(i), totalVolume, maxVolume(i), ...
        totalWeight/maxWeight(i)*100);
end