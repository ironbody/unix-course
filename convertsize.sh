#!/bin/bash


NUM=$(echo $1|grep -o -P '^\d*')
UNIT=$(echo $1|grep -o -P '[KMG]?B$')

sizes=()

fillArray () {
  start=$(( $NUM*1024**$1 ))
  for i in {0..3}
  do
    sizes+=($start)
    start=$(echo "$start" | awk '{printf "%.4f", $1/1024}')
  done
}

case $UNIT in
B)
  fillArray 0
  ;;
KB)
  fillArray 1
  ;;
MB)
  fillArray 2  
  ;;
GB)
  fillArray 3
  ;;
esac

echo "Bytes = ${sizes[0]}"
echo "Kilobytes = ${sizes[1]}"
echo "Megabytes = ${sizes[2]}"
echo "Gigabytes = ${sizes[3]}"
