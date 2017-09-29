#!/bin/gnuplot

set terminal svg enhanced size 1000 700 fname "Times" fsize 20
set output "recursive.svg"
set title "Recursive Algorithm (log(Time) vs n)"
set xlabel "n"
set ylabel "log(Time)"
set xrange[10:40]

plot "./recursive.dat" using 1:2 title "" with linespoints lc rgb 'red' lt 1 lw 1 pt 1 ps 0.5
