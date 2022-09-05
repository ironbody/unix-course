#!/bin/bash


NUM=$(echo $1|grep -o -P '^\d*')
UNIT=$(echo $1|grep -o -P '[KMG]?B$')

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