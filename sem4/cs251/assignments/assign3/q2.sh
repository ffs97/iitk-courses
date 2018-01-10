#!/bin/bash

if [[ $# -ne 3 ]]; then
	echo "Invalid number of Arguments"
	exit
fi
if ! [[ -e $1 ]] || ! [[ -e $2 ]] || ! [[ -e $3 ]]; then
	echo "File does not exist"
	exit
fi

declare -A names
while IFS=$',' read -a fields; do
	names[${fields[0]}]="${fields[1]}"
done < $1

declare -A marks
while IFS=$',' read -a fields; do
	marks[${fields[0]}]=$(( ${fields[1]} + ${fields[2]} ))
done < $3

while IFS=$',' read -a student; do
	echo "${student[1]},${names[${student[1]}]},${marks[${student[0]}]}"
done < $2