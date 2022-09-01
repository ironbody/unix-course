#!/bin/bash

DAY=$2
MONTH=$1
YEAR=$3
LEAP=False

checkDate () {
	# day, month, max days in month
	if [[ $1 -le $3 ]]
	then
		echo "1 - 31 in $2"
	else	
		echo "more than $3 days in $2"
	fi
}



if [[ $DAY -lt 1 ]]; then
	echo "No month has less than 1 day"
	exit
fi

if [[ $(expr $YEAR % 4) == 0 ]]; then
	if [[ $(expr $YEAR % 100) == 0 ]]; then
		if [[ $(expr $YEAR % 400) == 0 ]]; then
			echo "here 1"
			LEAP=True
		else
			echo "here 2"
			LEAP=False
		fi
	else
		echo "here 3"
		LEAP=True
	fi
else
	echo "here 4"
	LEAP=False
fi

case $MONTH in

1 | [Jj]an)
	checkDate "$DAY" "Jan" 31
	;;
2 | [Ff]eb)
	if [ $LEAP == True ]; then
		checkDate "$DAY" "Feb" 29
	else
		checkDate "$DAY" "Feb" 28
	fi
	;;
3 | [Mm]ar)
	checkDate "$DAY" "Mar" 31
	;;
4 | [Aa]pr)
	checkDate "$DAY" "Apr" 30
	;;
5 | [Mm]ay)
	checkDate "$DAY" "May" 31
	;;
6 | [Jj]un)
	checkDate "$DAY" "Jun" 30
	;;
7 | [Jj]ul)
	checkDate "$DAY" "Jul" 31
	;;
8 | [Aa]ug)
	checkDate "$DAY" "Aug" 31
	;;
9 | [Ss]ep)
	checkDate "$DAY" "Sep" 30 
	;;
10 | [Oo]ct)
	checkDate "$DAY" "Feb" 31
	;;
11 | [Nn]ov)
	checkDate "$DAY" "Feb" 30
	;;
12 | [Dd]ec)
	checkDate "$DAY" "Feb" 31
	;;
esac

echo "$MONTH $DAY $YEAR is somebody's birthday!"
