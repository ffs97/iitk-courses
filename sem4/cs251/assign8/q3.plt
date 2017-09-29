#!/usr/bin/gnuplot

set term postscript eps enhanced color 22
set output 'plots/q3_plot.eps'

set title "Analysis of {/Symbol a} and {/Symbol b}" font ",30"

set style data histograms
set style histogram errorbars gap 0 lw 1
set key right

set datafile separator " "

set xtics border in scale 0,0 offset character 1.4, 0.3, 0 font ",15"
set ytics font ",15"

set yrange [0:100]

set xlabel "Groups" offset character 0, -1, 0 font ",25"
set ylabel "Time" offset character 2, 0, 0 font ",25"

plot \
	newhistogram "Group 1" offset character 3, 0.8, 0 font ",20" lt 1, \
		'distributions/dist_norm_complete_2.txt' using 2:3:4:xtic(1) title "Data 1", \
		'distributions/dist_unif_complete_2.txt' using 2:3:4:xtic(1) title "Data 2", \
	newhistogram "Group 2" offset character 3, 0.8, 0 font ",20" lt 1, \
		'distributions/dist_norm_complete_2.txt' using 5:6:7:xtic(1) notitle, \
		'distributions/dist_unif_complete_2.txt' using 5:6:7:xtic(1) notitle