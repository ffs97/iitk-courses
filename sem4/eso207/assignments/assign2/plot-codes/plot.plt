#!/bin/gnuplot

set terminal svg enhanced size 1000 700 fname "Times" fsize 20
set xlabel "Queries"
set ylabel "Time"
set output "complete.svg"
set title "Comparision of Plots"

set yrange[0:0.015]

set style data linespoints
plot "./perfectly_balanced.dat" using 1:2 title "Perfect Balanced" ps 0.5, \
	"./nearly_balanced1.dat" using 1:2 title "Nearly Balanced (r 3/4)" ps 0.5, \
	"./nearly_balanced2.dat" using 1:2 title "Nearly Balanced (r 5/6)" ps 0.5, \
	"./nearly_balanced3.dat" using 1:2 title "Nearly Balanced (r 7/9)" ps 0.5, \

#plot "./perfectly_balanced.dat" using 1:2 title "Perfectly Balanced" ps 0.5
#plot "./nearly_balanced1.dat" using 1:2 title "Nearly Balanced (r 3/4)" ps 0.5
#plot "./nearly_balanced2.dat" using 1:2 title "Nearly Balanced (r 5/6)" ps 0.5
#plot "./nearly_balanced3t.dat" using 1:2 title "Nearly Balanced (r 7/9)" ps 0.5