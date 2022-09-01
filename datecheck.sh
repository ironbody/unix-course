#!/bin/bash

DAY=$2
MONTH=$1
YEAR=$3
LEAP=False

if [[ $DAY -lt 1 ]]; then
	echo "No month has less than 1 day"
	exit
fi

if (($YEAR % 4 == 0)); then
	if (($YEAR % 100 == 0)); then
		if (($YEAR % 400 == 0)); then
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
1 | [Jj]an) echo "wow its january" ;;
2 | [Ff]eb) echo "wow its january" ;;
3 | [Mm]ar) echo "wow its january" ;;
4 | [Aa]pr) echo "wow its january" ;;
5 | [Mm]ay) echo "wow its january" ;;
6 | [Jj]un) echo "wow its january" ;;
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
