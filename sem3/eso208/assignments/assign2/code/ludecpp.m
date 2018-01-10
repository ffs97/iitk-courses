function ludecpp(Ag, n)

A = Ag(:, 1:end-1);
b = Ag(:, end);

U = A;
L = zeros(n);
P = eye(n);

for j = 1:n
    L(j, j) = 1;
    if j < n
        for i = j+1:n
            if U(j) < U(i)
                U([i j], :) = U([j i], :);
                P([i j], :) = P([j i], :);
            end
        end
    end
    for i = j+1:n
        L(i, j) = U(i, j) / U(j, j);
        U(i, :) = U(i, :) - L(i, j) * U(j, :);
    end
end

x = zeros(n, 1);
y = zeros(n, 1);

b = P * b;

for j = 1:n
    sum = 0;
    for i = 1:j-1
        sum = sum + L(j, i)*y(i);
    end
    y(j) = b(j) - sum;
end

for j = n:-1:1
    sum = 0;
    for i = n:-1:j+1
        sum = sum + U(j, i)*x(i);
    end
    x(j) = (y(j) - sum) / U(j, j);
end

out = fopen('LUPivoting.txt', 'w');
printMatrix(x, n, 1, out, 'X');
printMatrix(P, n, n, out, 'P');
printMatrix(L, n, n, out, 'L');
printMatrix(U, n, n, out, 'U');