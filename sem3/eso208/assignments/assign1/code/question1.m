format long
warning('off', 'symbolic:sym:sym:DeprecateExpressions');

% Prompt input for equation
eq = input('Please enter a function (eg. 600*x^4 - 550*x^3 + 200*x^2 - 20*x - 1): \n', 's');

errors = [];
solution = 0;
liml = -1;
limu = 1;
% Prompt input for Method and choose the method
switch (input('Enter the Method you want to proceed with: \n(1) Bisection Method \n(2) False-Position Method\n(3) Modified False-Position Method\n(4) Newton-Raphson Method\n(5) Secant Method\n'))
    case 1
        [errors, solution, liml, limu] = bisection(eq);
    case 2
        [errors, solution, liml, limu] = falseposition(eq);
    case 3
        [errors, solution, liml, limu] = modfalseposition(eq);
    case 4
        [errors, solution, liml, limu] = newtonraphson(eq);
    case 5
        [errors, solution, liml, limu] = secant(eq);
end

solution

% Plotting error graph
subplot(1, 2, 1)
plot(errors), grid, xlabel('Iterations'), ylabel('Relative Approximate Error');

% Plotting function graph
subplot(1, 2, 2)
ezplot(eq, [liml limu]), grid, xlabel('x'), ylabel('f(x)');