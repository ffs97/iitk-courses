function printtofile(x, y, n, title, filename)

out = fopen(filename, 'w');
fprintf(out,'x \t  %s\n', title);

for i = 1:n
    fprintf(out, '%.5f   %.5f\n', x(i), y(i));   
end

end