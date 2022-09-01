#!/bin/bash

DAY=$2
MONTH=$1
YEAR=$3

if ((YEAR % 4 == 0)) 
then
	echo "divde by 4"
else 
	echo "can not"
fi
