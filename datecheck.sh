#!/bin/bash

DAY=$2
MONTH=$1
YEAR=$3
LEAP=False

if (($YEAR % 4 == 0)) 
then
	if (($YEAR % 100 == 0))
	then
		if (($YEAR % 400 == 0))
		then
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
	1|[Jj]an) 
		if [[ $DAY -le 31 && $DAY -ge 1 ]]
		then
			echo "EXISTS! $MONTH $DAY $YEAR is someone's birthday". 
		else
			echo "BAD INPUT: $MONTH $DAY $YEAR does not have $DAY days."
		fi
		;;
	2|[Ff]eb) echo "wow its febrary";;

	3|[Mm]ar) echo "wow its mars";;
	4|[Aa]pr) echo "wow its april";;
	5|[Mm]ay) echo "wow its may";;
	6|[Jj]un) echo "wow its juni";;
	7|[Jj]ul) echo "wow its january";;
	8|[Aa]ug) echo "wow its january";;
	9|[Ss]ep) echo "wow its january";;
	10|[Oo]ct) echo "wow its january";;
	11|[Nn]ov) echo "wow its january";;
	12|[Dd]ec) echo "wow its january";;
esac
