#!/bin/bash

if [[ $# -ne 6 ]]; then
	echo "Insufficient number of arguments"
	exit
fi
if ! [[ $1 =~ ^[0-9]+$ ]] || ! [[ $2 =~ ^[0-9]+$ ]] || ! [[ $3 =~ ^[0-9]+$ ]]; then
	echo "Ivalid Arguments"
	exit
fi
if [[ $1 -eq 0 ]] || [[ $2 -eq 0 ]] || [[ $3 -eq 0 ]]; then
	"Arguments cannot be 0"
	exit
fi
if [[ $2 -lt $3 ]]; then
	"Number of TAs cannot be less than number of days"
fi
if ! [[ -e $4 ]]; then
	echo "The file $4 does not exist"
	exit
fi
if ! date -d"$5 +$6"; then
	exit
fi

IFS=$'\n' students=( $( sort --random-sort <<< "`seq 1 $1`" ))
IFS=$'\n' tas=( $( sort --random-sort <<< "`seq 1 $2`"))

d=`expr $3 - 1`

for i in `seq 1 $2`; do
	taW[`expr $i - 1`]=0
done

tta=0
tst=0
dc=1
while IFS= read -r weight; do
	stc=`awk "BEGIN{print int((($weight/100)*$1)+0.5)}"`
	if [ `expr $tst + $stc` -ne $1 ] && [ $dc -eq $3 ]; then
		stc=$(($1-$tst))
	fi

	tac=`awk "BEGIN{print int((($stc/$1)*$2)+0.5)}"`
	if [ `expr $tta + $tac` -ne $2 ] && [ $dc -eq $3 ]; then
		tac=$(($2-$tta))
	fi
	if [ $tac -eq 0 ]; then
		tac=1
	fi
	taRd[`expr $dc - 1`]=$tac

	echo "Day $dc Students:"
	echo "${students[@]:$tst:$stc}" | sed 's/ /\n/g'
	tst=$(($tst+$stc))

	cta=0
	while [ $stc -gt 0 ]; do
		taW[`expr $tta + $cta`]=$((${taW[`expr $tta + $cta`]}+1))
		cta=$((($cta+1)%$tac))
		stc=$(($stc-1))
	done

	tta=$(($tta+$tac))
	dc=$(($dc+1))
	echo ""
done < $4

stc=0
tac=0
for i in `seq 0 $d`; do
	echo "Day `expr $i + 1` Mapping:"
	for j in `seq 1 $tt ${taRd[$i]}`; do
		cst=$(date -d"$5")
		for k in `seq 1 ${taW[$tac]}`; do
			cstd=$(date -d"$cst" +'%l:%M%p');
			echo "${tas[$tac]} ${students[$stc]}$cstd"
			stc=$(($stc+1))
			cst=$(date -d"$cst +$6")
		done
		tac=$(($tac+1))
	done
	echo ""
done