lambda = 2;

for i = 1:6
    exp_dist = exprnd(1/lambda, 1, 10^i);
    
    dist_mean = mean(exp_dist);
    dist_std = std(exp_dist, 1);
   
    perc_dev_mean = abs(1 - lambda*dist_mean)*100;
    perc_dev_std = abs(1 - lambda*dist_std)*200;
    
    printf('Exponent Sample - 10^%d\n', i);
    printf('\tMean: %f\n', dist_mean);
    printf('\tDeviation from Actual Mean: %f%%\n', perc_dev_mean);
    printf('\tDeviation from Actual Std. Deviation: %f%%\n', perc_dev_std);
endfor