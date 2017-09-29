#!/bin/bash

if [ -n "$1" ] && [ -n "$2" ]; then
	if [[ $( echo "$1>$2" | bc -l ) == 1 ]]; then
		echo "$1 > $2"
	elif [[ $( echo "$1<$2" | bc -l ) == 1 ]]; then
		echo "$1 < $2"
	else
		echo "$1 = $2"
	fi
else
	echo "Please input 2 numbers"
fi