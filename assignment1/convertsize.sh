#!/bin/bash


NUM=$(echo $1|grep -o -P '^\d*')
UNIT=$(echo $1|grep -o -P '[KMG]?B$')

sizes=()

fillArray () {
  start=$(( $NUM*1024**$1 ))
  for i in {0..3}
  do
    sizes+=($start)
    # start=$(echo "$start" | awk '{printf "%.4f", $1/1024}')
    start=$(echo "scale=4;$start/1024" | bc -l)
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
*)
  echo "Allowed units are B, KB, MB, and GB"
  exit
  ;;
esac

echo "Bytes = ${sizes[0]}" | sed -r "s/\.(0+)$//g"
echo "Kilobytes = ${sizes[1]}" | sed -r "s/\.(0+)$//g"
echo "Megabytes = ${sizes[2]}" | sed -r "s/\.(0+)$//g"
echo "Gigabytes = ${sizes[3]}" | sed -r "s/\.(0+)$//g"
