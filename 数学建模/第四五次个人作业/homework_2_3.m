profit = [
    0 4 6 7 7 7 7;
    0 2 4 6 8 9 10;
    0 3 5 7 8 8 8;
    0 4 5 6 6 6 6
];
n = 6; 
m = 4; 
nvars = m * 7;
%目标函数
f = zeros(nvars, 1);
for i = 1:m
    for j = 1:7
        f((i-1)*7 + j) = -profit(i, j);
    end
end
%约束条件1
Aeq1 = zeros(m, nvars);
for i = 1:m
    Aeq1(i, (i-1)*7+1:(i-1)*7+7) = ones(1, 7);
end
beq1 = ones(m, 1);
% 约束条件2
Aeq2 = zeros(1, nvars);
for i = 1:m
    for j = 1:7
        Aeq2(1, (i-1)*7 + j) = j - 1;
    end
end
beq2 = n;
%求解
Aeq = [Aeq1; Aeq2];
beq = [beq1; beq2];
intcon = 1:nvars;
lb = zeros(nvars, 1);
ub = ones(nvars, 1);
options = optimoptions('intlinprog', 'Display', 'off');
[x, fval] = intlinprog(f, intcon, [], [], Aeq, beq, lb, ub, options);
% 输出结果
allocation = zeros(1, m);
for i = 1:m
    for j = 1:7
        if x((i-1)*7 + j) > 0.5
            allocation(i) = j - 1;
            break;
        end
    end
end
enterprises = {'甲', '乙', '丙', '丁'};
fprintf('最优分配方案：\n');
for i = 1:m
    fprintf('%s企业：%d台设备\n', enterprises{i}, allocation(i));
end
fprintf('最大总利润：%.2f千万元\n', -fval);