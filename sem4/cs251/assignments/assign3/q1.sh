#!/bin/bash

if [[ $# -ne 1 ]]; then
	echo "Invalid number of Arguments"
	exit
fi
if ! [[ -e $1 ]]; then
	echo "File does not exist"
	exit
fi

declare -a values
IFS=$'\n' read -d' ' -a values < $1

max=${values[0]}
min=${values[0]}
for i in ${values[*]}; do
	if ! [[ $i =~ ^[+-]?[0-9]*\.?[0-9]*$ ]]; then
		echo "$i is not a valid input"
		exit
	fi
	if [[ `echo "$max<$i" | bc -l` -eq 1 ]]; then
		max="$i"
	fi
	if [[ `echo "$min>$i" | bc -l` -eq 1 ]]; then
		min="$i"
	fi
done

nf=`echo "($max) - ($min)" | bc`
for i in ${values[*]}; do
	printf "%.4f\n" `bc <<< "scale=5; (($i)-($min))/($nf)"`
done
