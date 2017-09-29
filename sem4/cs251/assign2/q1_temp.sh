# if [ -n "$1" ]; then
# 	if [ -e $1 ]; then
# 		while IFS= read -r line
# 		do
# 			l=$( echo "$line" | grep -o "," | wc -l )
# 			l=`expr $l + 1`
# 			switched=""
# 			for i in `seq 1 2 $l`; do
# 				t=$( echo "$line" | cut -d, -f`expr $i + 1`,$i | sed 's/\([^,]*\),\([^,]*\)/\2,\1/' )
# 				switched="$switched,$t"
# 			done
# 			echo "$switched" | sed 's/,//'
# 		done < $1
# 	else
# 		echo "File does not Exist"
# 	fi
# else
# 	echo "Input Filename not specified"
# fi

# while IFS=',' read -a fields; do
# 	l=$((${#fields[*]}-1))
# 	line=""
# 	for i in `seq 0 2 $l`; do
# 		temp="${fields[$i]}"
# 		line="${line},${fields[`expr $i + 1`]},${fields[$i]}"
# 	done
# 	echo "$line"
# done < $1

if [ -n "$1" ]; then
	if [ -e $1 ]; then 
		sed 's/^/,/g' < $1 |
			sed 's/,\([^,]*\),\([^,]*\)/,\2,\1/g' |
					sed 's/,//'
	else
		echo "File Does Not Exist"
	fi
else
	echo "Input File Not Specified"
fi