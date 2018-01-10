% function [roots] = bairstow2(eq)

eq = 'x^3 - 11*x^2 + 32*x - 22';
f = sym2poly(sym(eq));

% Input prompt for required values
% r = input('Enter a value for r: ');
% s = input('Enter a value for s: ');
% maxiters = input('Enter the maximum iterations you want: ');
r = -0.1;
s = -0.1;
maxiters = 20;

sol = [r; s];

% Looping through all iterations
for n = 1:maxiters
    [p, q] = deconv(f, [1 -r -s]);
    q = q([end-1 end]);
    p = p([end-1 end]);
    q(2) = q(2) + q(1)*r;
%     sol = sol - [-r2(2) r2(1); -r2(1)*sol(2) r2(1)*sol(1)-r2(2)]*r1/(sol(2)*r2(1)^2 + r2(2)*(r2(2) - sol(1)*r2(1)));

    rt = r;
    r = -(f(end-1) + s*p(1)) / p(2)
    s = -(f(end) + rt*q(1)) / p(2)
    
    if (q(1) == 0 && q(2) == 0)
        break
    end
end
t = sqrt(sol(1)^2 - 4*sol(2));
roots = [(-t-sol(1))/2 (t-sol(1))/2]