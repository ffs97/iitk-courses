#!/usr/bin/gnuplot

#set term postscript eps enhanced color 22
set term pdf font "Helvetica,10" size 6in,4in
set output 'graph2.pdf'

set title "Time Ranges for Mergesort" offset character 0, 1, 0 font ",26"

set style data histograms
set key off
set border 3

set xtics border in scale 0,0 font ",15"
set ytics border in scale 0,0 font ",15"

set xlabel "Time" offset character 0, -1.5, 0 font ",23"
set ylabel "Frequency" offset character -2.5, 0, 0 font ",23"

set boxwidth 0.003 absolute

bin_width = 0.005;

bin_number(x) = floor(x/bin_width)

rounded(x) = bin_width * ( bin_number(x) + 0.5 )

plot 'data/data2.csv' using (rounded($1)):(1) smooth frequency with boxes
