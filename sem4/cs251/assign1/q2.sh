#!/bin/bash

if [ -n "$1" ]; then
	if [ -n "$2" ] && [ -n "$3" ]; then
		grep "$2" $1 | grep "$3"
	else
		echo "Search strings count must be 2"
	fi
else
	echo "Filename is missing"
fi