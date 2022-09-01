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
	# if [[ $DAY -gt 31 ]]; then
	# 	echo "Dec does not have $DAY days"
	# 	exit
	# else
	# 	MONTH="Dec"
	# fi
	# ;;
	checkDate "$DAY" "$MONTH" 31
	;;
2 | [Ff]eb)
	if [ $LEAP == True ]; then
		echo "is leap"
		if [[ $DAY -gt 29 ]]; then
			echo "Dec does not have $DAY days"
			exit
		else
			MONTH="Dec"
		fi
	else
		if [[ $DAY -gt 28 ]]; then
			echo "Dec does not have $DAY days"
			exit
		else
			MONTH="Dec"
		fi
	fi
	;;
3 | [Mm]ar)
	if [[ $DAY -gt 31 ]]; then
		echo "Dec does not have $DAY days"
		exit
	else
		MONTH="Dec"
	fi
	;;
4 | [Aa]pr)
	if [[ $DAY -gt 31 ]]; then
		echo "Dec does not have $DAY days"
		exit
	else
		MONTH="Dec"
	fi
	;;
5 | [Mm]ay)
	if [[ $DAY -gt 31 ]]; then
		echo "Dec does not have $DAY days"
		exit
	else
		MONTH="Dec"
	fi
	;;
6 | [Jj]un)
	if [[ $DAY -gt 31 ]]; then
		echo "Dec does not have $DAY days"
		exit
	else
		MONTH="Dec"
	fi
	;;
7 | [Jj]ul)
	if [[ $DAY -gt 31 ]]; then
		echo "Jul does not have $DAY days"
		exit
	else
		MONTH="Jul"
	fi
	;;
8 | [Aa]ug)
	if [[ $DAY -gt 31 ]]; then
		echo "Aug does not have $DAY days"
		exit
	else
		MONTH="Aug"
	fi
	;;
9 | [Ss]ep)
	if [[ $DAY -gt 31 ]]; then
		echo "Sep does not have $DAY days"
		exit
	else
		MONTH="Sep"
	fi
	;;
10 | [Oo]ct)
	if [[ $DAY -gt 31 ]]; then
		echo "Oct does not have $DAY days"
		exit
	else
		MONTH="Oct"
	fi
	;;
11 | [Nn]ov)
	if [[ $DAY -gt 31 ]]; then
		echo "Nov does not have $DAY days"
		exit
	else
		MONTH="Nov"
	fi
	;;
12 | [Dd]ec)
	if [[ $DAY -gt 31 ]]; then
		echo "Dec does not have $DAY days"
		exit
	else
		MONTH="Dec"
	fi
	;;
esac

echo "$MONTH $DAY $YEAR is somebody's birthday!"
