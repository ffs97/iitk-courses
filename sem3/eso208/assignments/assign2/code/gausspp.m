function gausspp(Ag, n)

P = eye(n);

for j = 1:n-1
    for i = j+1:n
        if Ag(j) < Ag(i)
            Ag([i j], :) = Ag([j i], :);
            P([i j], :) = P([j i], :);
        end
    end
end

for j = 1:n-1
    for i = j+1:n
        Ag(i, :) = Ag(i, :) - (Ag(i, j) * (Ag(j, :) / Ag(j, j)));
    end
end

x = zeros(n, 1);

for j = n:-1:1
    sum = 0;
    for i = n:-1:j
        sum = sum + (Ag(j, i) * x(i));
    end
    x(j) = (Ag(j, n+1) - sum) / Ag(j, j);
end

out = fopen('GaussEliminationPivoting.txt', 'w');
printMatrix(x, n, 1, out, 'X');
printMatrix(P, n, n, out, 'P');
printMatrix(Ag, n, n, out, 'U');