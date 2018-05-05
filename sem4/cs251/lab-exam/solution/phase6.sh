if (( $# > 1 )); then
	echo "${@:2} Arguments Ignored"
fi

file="devas.txt"
if (( $# > 0 )); then
	file="$1"
fi

i=0
grep -P '^\t' $file | sed 's/\t//g'