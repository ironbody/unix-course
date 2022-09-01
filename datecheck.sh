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

echo $LEAP

case $MONTH in
	\(1|jan\)) echo -n "wow its january";;
esac
