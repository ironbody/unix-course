#!/bin/bash

NUM=$(echo $1|grep -o -P '^\d*')
UNIT=$(echo $1|grep -o -P '[KMG]?B$')

echo $NUM
echo $UNIT
