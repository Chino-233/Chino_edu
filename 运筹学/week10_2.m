clear;
clc;

% 参数设置
rent = [5000, 2000, 3000];         % 设备租金
cost = [280, 30, 200];             % 生产成本（元/件）
price = [400, 40, 300];            % 销售价格（元/件）
labor_hours = [5, 1, 4];           % 人工工时（h/件）
equip_hours = [3, 0.5, 2];         % 设备工时（h/件）
equip_avail_hours = [300, 300, 300]; % 设备可用工时
total_labor_hours = 2000;          % 总可用人工工时
clothing_names = ["西服", "衬衫", "羽绒服"];
unit_profit = price - cost;
% 目标函数系数（求最大值，所以取负值）
f = [-unit_profit, rent];  % [-120, -10, -100, 5000, 2000, 3000]
A = [];
b = [];

% 1. 人工工时限制
A_labor = [labor_hours, 0, 0, 0];
b_labor = total_labor_hours;
A = [A; A_labor];
b = [b; b_labor];

% 2. 设备工时限制
for i = 1:3
    A_equip = zeros(1, 6);
    A_equip(i) = equip_hours(i);
    A_equip(i+3) = -equip_avail_hours(i);
    A = [A; A_equip];
    b = [b; 0];
end

% 3. 设备租用约束（如果不租用设备，则不能生产对应服装）
M = 1000;  % 一个足够大的数
for i = 1:3
    A_rent = zeros(1, 6);
    A_rent(i) = 1;
    A_rent(i+3) = -M;
    A = [A; A_rent];
    b = [b; 0];
end

% 求解
intcon = 1:6; 
lb = zeros(6, 1);
ub = [inf*ones(3,1); ones(3,1)]; 
[x, fval] = intlinprog(f, intcon, A, b, [], [], lb, ub);

% 计算利润
production = x(1:3);
rent_decision = x(4:6);
sales_revenue = price * production;
production_cost = cost * production;
equipment_rent = rent * rent_decision;
net_profit = sales_revenue - production_cost - equipment_rent;

% 结果显示
fprintf('西服生产数量: %.0f 件\n', x(1));
fprintf('衬衫生产数量: %.0f 件\n', x(2));
fprintf('羽绒服生产数量: %.0f 件\n', x(3));
fprintf('是否租用西服设备: %d\n', x(4));
fprintf('是否租用衬衫设备: %d\n', x(5));
fprintf('是否租用羽绒服设备: %d\n', x(6));

fprintf('\n利润分析:\n');
fprintf('销售收入: %.2f 元\n', sales_revenue);
fprintf('生产成本: %.2f 元\n', production_cost);
fprintf('设备租金: %.2f 元\n', equipment_rent);
fprintf('最大月净利润: %.2f 元\n', net_profit);

fprintf('\n资源使用情况:\n');
fprintf('人工工时使用: %.2f/%.0f 小时 (%.2f%%)\n', labor_hours*production, total_labor_hours, labor_hours*production/total_labor_hours*100);

for i = 1:3
    if rent_decision(i) > 0.5
        fprintf('%s设备工时使用: %.2f/%.0f 小时 (%.2f%%)\n', clothing_names(i), equip_hours(i)*production(i), equip_avail_hours(i), equip_hours(i)*production(i)/equip_avail_hours(i)*100);
    end
end