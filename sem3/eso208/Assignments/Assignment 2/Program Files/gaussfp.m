function gaussfp(Ag, n)

P = 1:n;

for j = 1:n-1

    max = zeros(n, 1);
    maxIndex = j * ones(n, 1);

    for k = j:n
        for i = j:n
            if abs(Ag((i-1)*n + k)) > max(k);
                max(k) = Ag((i-1)*n + k);
                maxIndex(k) = i;
            end
        end
    end
    
    for i = j+1:n
        if max(j) < max(i)
            t = Ag(j, :);
            Ag(j, :) = Ag(i, :);
            Ag(i, :) = t;
            
            t = max(j);
            max(j) = max(i);
            max(i) = t;
            
            t = maxIndex(j);
            maxIndex(j) = maxIndex(i);
            maxIndex(i) = t;
        end
    end
    
    t = P(maxIndex(j));
    P(maxIndex(j)) = j;
    P(j) = t;
    
    t = Ag(:, maxIndex(j));
    Ag(:, maxIndex(j)) = Ag(:, j);
    Ag(:, j) = t;
    
    for i = j+1:n
        Ag(i, :) = Ag(i, :) - (Ag((j-1)*n + i) * (Ag(j, :) / Ag((j-1)*n + j)));
    end
    
end

sol = zeros(n, 1);

for j = n:-1:1
    sum = 0;
    for i = n:-1:j
        sum = sum + (Ag((i-1)*n + j) * sol(i));
    end
    sol(j) = (Ag(n*n + j) - sum) / Ag((j-1)*n + j);
end

x = sol;
for i = 1:n
    x(P(i)) = sol(i);
end

out = fopen('GaussEliminationFullPivoting.txt', 'w');
printMatrix(x, n, 1, out, 'X');
printMatrix(Ag, n, n, out, 'U');