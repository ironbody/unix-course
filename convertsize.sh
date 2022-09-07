#!/bin/bash


NUM=$(echo $1|grep -o -P '^\d*')
UNIT=$(echo $1|grep -o -P '[KMG]?B$')

<<<<<<< HEAD
convert () {
    case $2 in
        B)
        printf "%.2f \n" $1; # b
        nums=$(( $1 / 1000 ));
        printf "%.4f \n" $nums; # kb
        nums1=$(( $nums / 1000 ));
        printf "%.3f \n" $nums1 # mb
        nums2=$(( $nums1 / 1000 ));
        printf "%.f \n" $nums2; # gb
        ;;
        esac
}   


convert $NUM $UNIT
=======
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

>>>>>>> a505b7c2b956ff2a82d5115c65719a93a7b3e9c3
