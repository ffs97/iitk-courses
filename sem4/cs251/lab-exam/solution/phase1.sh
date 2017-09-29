if (( $# > 1 )); then
	echo "${@:2} Arguments Ignored"
fi

file=""
if [ -n "$1" ]; then
	file="nakshatras.txt"
else
	file="$1"
fi

cat $file | awk '{ for(i=0; i<4; i++) print }' > phase1.txt
