function printMatrix(M, n, m, file, name)

fprintf(file, '\n%s\n', name);
for i = 1:n
    for j = 1:m
        fprintf(file, '\t%.4f', M(i, j));
    end
    fprintf(file, '\n');
end