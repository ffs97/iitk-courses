#!/usr/bin/gnuplot

set term postscript eps enhanced color 22
set output 'plots/q2_plot.eps'

set title "Different Distributions" font ",30"

set style data histograms
set style histogram errorbars gap 0 lw 1
set key right

set datafile separator " "

set xtics border in scale 0,0 offset character 0, 0.3, 0 font ",15"
set ytics font ",15"

set yrange [0:100]

set xlabel "Values" offset character 0, -2, 0 font ",25"
set ylabel "Frequency" offset character 2, 0, 0 font ",25"

plot \
	newhistogram "Distribution 1\n(Normal)" offset character 0.3, 1.6, 0 font ",20" lt 2, 'distributions/dist_norm_complete_1.txt' using 2:3:4:xtic(1) title "Data" , \
	newhistogram "Distribution 2\n(Uniform)" offset character 0.3, 1.6, 0 font ",20" lt 2, 'distributions/dist_unif_complete_1.txt' using 2:3:4:xtic(1) notitle
