function val = simpson(f, a, b)
    c = (a+b)/2;
    global points;
    points = [points a b c];
    h = abs(b-a)/6;
    val =  h*(f(a) + 4*f(c) + f(b));
    plot(a, f(a), 's');
    plot(b, f(b), 's');
    plot(c, f(c), 's');
end