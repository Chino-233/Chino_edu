maxWeight = [10, 16, 8];  % 各货舱质量限制(吨)
maxVolume = [6800, 8700, 5300];  % 各货舱体积限制(立方米)
unitVolume = [480, 650, 580, 390];  % 各货物单位体积(立方米/吨)
profit = [3100, 3800, 3500, 2850];  % 各货物单位利润(元/吨)
c = -[profit, profit, profit];  % 目标函数
% 质量约束
A_weight = zeros(3, 12);
for i = 1:3
    A_weight(i, (i-1)*4+1:(i-1)*4+4) = 1;
end
b_weight = maxWeight';
% 体积约束
A_volume = zeros(3, 12);
for i = 1:3
    for j = 1:4
        A_volume(i, (i-1)*4+j) = unitVolume(j);
    end
end
b_volume = maxVolume';
%平衡约束
A_balance = zeros(2, 12);
A_balance(1, 1:4) = 16;
A_balance(1, 5:8) = -10;
A_balance(2, 5:8) = 8;
A_balance(2, 9:12) = -16;
b_balance = [0; 0];
% 合并
A = [A_weight; A_volume];
b = [b_weight; b_volume];
Aeq = A_balance;
beq = b_balance;
lb = zeros(12, 1);
options = optimoptions('linprog', 'Display', 'iter');
[x, fval] = linprog(c, A, b, Aeq, beq, lb, [], options);
% 输出结果
fprintf('最优解：\n');
fprintf('总利润：%.2f元\n', -fval);
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