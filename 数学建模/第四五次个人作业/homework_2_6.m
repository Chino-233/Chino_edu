f = [20, 90, 80, 70, 30];
A = [-1, -1, 0, 0, -1;
    0, 0, -1, -1, 0;
    3, 0, 2, 0, 0;
    0, 3, 0, 2, 1];
b = [-30; -30; 120; 48];
intcon = 1:5;
lb = zeros(5, 1);
ub = [];
options = optimoptions('intlinprog', 'Display', 'iter');
[x, fval, exitflag, output] = intlinprog(f, intcon, A, b, [], [], lb, ub, options);
%输出结果
fprintf('最优解:\n');
for i = 1:5
    fprintf('x%d = %d\n', i, x(i));
end
fprintf('最小目标函数值: Z₁ = %d\n', fval);