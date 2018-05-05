if (( $# > 1 )); then
	echo "${@:2} Arguments Ignored"
fi

file="rashis.txt"
if (( $# > 0 )); then
	file="$1"
fi

i=0
head -12 $file | 
while read -r line; do
    s=$( uniq -c phase2-$i.txt | sed -e 's/^\s\+//g' -e's/,/ /g' | tr $'\n' ' ' )
    echo $line" : "$s
    i=$(( $i + 1 ))
done