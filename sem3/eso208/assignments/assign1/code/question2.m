format long
warning('off', 'symbolic:sym:sym:DeprecateExpressions');

% Input prompt for equation
eq = input('Please enter a polynomial function (eg. 600*x^4 - 550*x^3 + 200*x^2 - 20*x - 1): \n', 's');

% Input prompt for Method and Choose Method
switch (input('Enter the Method you want to proceed with: \n(1) Muller Method \n(2) Bairstow Method\n'))
    case 1
        [solution, liml, limu] = muller(eq);
        solution
		
		% Plot function graph
        ezplot(eq, [liml limu]), grid, xlabel('x'), ylabel('f(x)');
    case 2
        roots = bairstow2(eq)
		
		% Plot function graph
        ezplot(eq, [roots(1)-1 roots(2)+1]), grid, xlabel('x'), ylabel('f(x)');
end