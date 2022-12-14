#!/bin/bash

DAY=$2
MONTH=$1
YEAR=$3
LEAP=False

if [[ $# != 3 ]]; then
	echo "You must enter EXACTLY 3 arguments. Existing"
	exit
fi;

checkDate () {
	# day, month, max days in month
	if [[ $1 -le $3 && $1 -ge 1 ]]
	then
		echo "EXISTS! $2 $DAY $YEAR is somebody's birthday."

	else
		if [[ $2 == "Feb" && $DAY -eq 29 ]]; then
			echo "BAD INPUT: Feb $YEAR does not have 29 days: not a leap year."	
		else
			echo "BAD INPUT: $2 does not have $1 days."
		fi;
	fi
}

if [[ $(expr $YEAR % 4) == 0 ]]; then
	if [[ $(expr $YEAR % 100) == 0 ]]; then
		if [[ $(expr $YEAR % 400) == 0 ]]; then
			LEAP=True
		else
			LEAP=False
		fi
	else
		LEAP=True
	fi
else
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
	checkDate "$DAY" "Oct" 31
	;;
11 | [Nn]ov)
	checkDate "$DAY" "Nov" 30
	;;
12 | [Dd]ec)
	checkDate "$DAY" "Dec" 31
	;;
esac