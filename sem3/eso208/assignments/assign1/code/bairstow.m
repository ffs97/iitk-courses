function [roots] = bairstow(eq)

f = sym2poly(sym(eq));

% Input prompt for required values
r = input('Enter a value for r: ');
s = input('Enter a value for s: ');
maxiters = input('Enter the maximum iterations you want: ');

sol = [-r; -s];

% Looping through all iterations
for n = 1:maxiters
    [p, r1] = deconv(f, [1 sol(1) sol(2)]);
    r1 = r1([end-1 end]);
    [~, r2] = deconv(p, [1 sol(1) sol(2)]);
    r2 = r2([end-1 end]);
    sol = sol - [-r2(2) r2(1); -r2(1)*sol(2) r2(1)*sol(1)-r2(2)]*r1/(sol(2)*r2(1)^2 + r2(2)*(r2(2) - sol(1)*r2(1)));
    
    if (r1(1) == 0 && r1(2) == 0)
        break
    end
end
t = sqrt(sol(1)^2 - 4*sol(2));
roots = [(-t-sol(1))/2 (t-sol(1))/2];