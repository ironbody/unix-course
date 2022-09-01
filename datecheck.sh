#!/bin/bash

DAY=$2
MONTH=$1
YEAR=$3
LEAP=False

if [[ $DAY -lt 1 ]]; then
	echo "No month has less than 1 day"
	exit
fi

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
	if [[ $DAY -gt 31 ]]; then
		echo "Jan does not have $DAY days"
		exit
	else
		MONTH="Jan"
	fi
	;;
2 | [Ff]eb)
	if [ $LEAP == True ]; then
		echo "is leap"
		if [[ $DAY -gt 29 ]]; then
			echo "Feb does not have $DAY days"
			exit
		else
			MONTH="Feb"
		fi
	else
		if [[ $DAY -gt 28 ]]; then
			echo "Feb does not have $DAY days"
			exit
		else
			MONTH="Feb"
		fi
	fi
	;;
3 | [Mm]ar)
	if [[ $DAY -gt 31 ]]; then
		echo "Mar does not have $DAY days"
		exit
	else
		MONTH="Mar"
	fi
	;;
4 | [Aa]pr)
	if [[ $DAY -gt 31 ]]; then
		echo "Apr does not have $DAY days"
		exit
	else
		MONTH="Apr"
	fi
	;;
5 | [Mm]ay)
	if [[ $DAY -gt 31 ]]; then
		echo "May does not have $DAY days"
		exit
	else
		MONTH="May"
	fi
	;;
6 | [Jj]un)
	if [[ $DAY -gt 31 ]]; then
		echo "Jun does not have $DAY days"
		exit
	else
		MONTH="Jun"
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
