a_true = 1.1;
b_true = 0.01;
x_data = 1:20;

% 计算模拟观测值
y_data = zeros(size(x_data));
for i = 1:length(x_data)
    y_data(i) = 10*a_true/(10*b_true + (a_true-10*b_true)*exp(-a_true*sin(x_data(i))));
end

% 使用 lsqcurvefit 进行拟合
disp('方法1: 使用lsqcurvefit拟合')
fun = @(p,x) 10*p(1)./(10*p(2) + (p(1)-10*p(2)).*exp(-p(1)*sin(x)));
p0 = [1, 0.01]; 
lb = [];
ub = []; 
options = optimoptions('lsqcurvefit', 'Display', 'off');
[popt, resnorm, ~, ~, output] = lsqcurvefit(fun, p0, x_data, y_data, lb, ub, options);

% 输出
fprintf('拟合参数: a = %.6f, b = %.6f\n', popt(1), popt(2));

% 计算R^2
y_pred = fun(popt, x_data);
ss_res = sum((y_data - y_pred).^2);
ss_tot = sum((y_data - mean(y_data)).^2);
r_squared = 1 - (ss_res / ss_tot);
fprintf('R²: %.6f\n', r_squared);

% 使用 fittype 和 fit 进行拟合
disp('方法2: 使用fittype和fit拟合');
ft = fittype('10*a/(10*b + (a-10*b)*exp(-a*sin(x)))','independent','x','coefficients',{'a','b'});
opts = fitoptions(ft);
opts.StartPoint = [1 0.01];
[fitobj, gof] = fit(x_data', y_data', ft, opts);

% 输出拟合结果
fprintf('拟合参数: a = %.6f, b = %.6f\n', fitobj.a, fitobj.b);
fprintf('R²: %.6f\n', gof.rsquare);

% 画图
figure;
scatter(x_data, y_data, 'ko', 'DisplayName', '原始数据');
hold on;

x_fine = linspace(1, 20, 1000);
y_lsqcurvefit = fun(popt, x_fine);
y_fit = feval(fitobj, x_fine);

plot(x_fine, y_lsqcurvefit, 'r-', 'LineWidth', 1.5, 'DisplayName', sprintf('lsqcurvefit: a=%.4f, b=%.4f', popt(1), popt(2)));
plot(x_fine, y_fit, 'g--', 'LineWidth', 1.5, 'DisplayName', sprintf('fit: a=%.4f, b=%.4f', fitobj.a, fitobj.b));

xlabel('x');
ylabel('g(x)');
title('函数g(x)的拟合结果比较');
grid on;
legend('Location', 'best');