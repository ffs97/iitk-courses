function val = adaptive(f, a, b, err, whole)
    c = (a+b)/2;
    left = simpson(f, a, c);
    right = simpson(f, c, b);
    if (abs(left+right - whole) <= err)
        val = left+right + (left+right - whole)/15;
    else
        val = adaptive(f, a, c, err, left) + adaptive(f, c, b, err, right);
    end
end