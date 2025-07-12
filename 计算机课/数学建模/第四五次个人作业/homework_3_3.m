function z = objective(x)
    z = x(1)^2 + x(2)^2 + 3*x(3)^2 + 4*x(4)^2 + 2*x(5)^2 - 8*x(1) -2*x(2) -3*x(3) -x(4) -2*x(5);
end  
function [c, ceq] = constraints(x)
    c = [sum(x) - 400;
        x(1) + 2*x(2) + 2*x(3) + x(4) + 6*x(5) - 800;
        2*x(1) + x(2) + 6*x(3) - 200;
        x(3) + x(4) + 5*x(5) - 200];
    ceq = [];
end
x0 = [100, 100, 30, 40, 20];
lb = zeros(1,5);
ub = [99, 99, 99, 99, 99];
options_ga = optimoptions('ga', 'Display', 'off', 'PopulationSize', 100, 'MaxGenerations', 200);
IntCon = 1:5;
[x_integer, fval_integer] = ga(@objective, 5, [], [], [], [], lb, ub, @constraints, IntCon, options_ga);
% 输出结果
disp('整数解决方案:');
disp(['x = [', num2str(x_integer), ']']);
disp(['目标函数值 = ', num2str(fval_integer)]);
