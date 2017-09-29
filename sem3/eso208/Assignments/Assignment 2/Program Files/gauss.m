function gauss(Ag, n)

for j = 1:n-1
    for i = j+1:n
        Ag(i, :) = Ag(i, :) - (Ag((j-1)*n + i) * (Ag(j, :) / Ag((j-1)*n + j)));
    end
end

x = zeros(n, 1);

for j = n:-1:1
    sum = 0;
    for i = n:-1:j+1
        sum = sum + (Ag((i-1)*n + j) * x(i));
    end
    x(j) = (Ag(n*n + j) - sum) / Ag((j-1)*n + j);
end

out = fopen('GaussElimination.txt', 'w');
printMatrix(x, n, 1, out, 'X');
printMatrix(Ag, n, n, out, 'U');