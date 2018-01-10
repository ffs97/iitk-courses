#!/bin/bash

if [[ $# -eq 0 ]]; then
	"Insufficient number of arguments"
	exit
fi
if ! [[ -e $1 ]]; then
	echo "File does not exist"
	exit
fi

sed 's/^/,/g' < $1 | sed 's/,\([^,]*\),\([^,]*\)/,\2,\1/g' | sed 's/,//'