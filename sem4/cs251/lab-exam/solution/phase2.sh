if (( $# > 1 )); then
	echo "${@:2} Arguments Ignored"
fi

file="phase1.txt"
if (( $# > 0 )); then
	file="$1"
fi

for i in {0..11}; do
    head -$(( $i * 9 + 9 )) $file | tail -9 > phase2-$i.txt
done