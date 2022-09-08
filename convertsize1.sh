#!/bin/bash


NUM=$(echo $1|grep -o -P '^\d*')
UNIT=$(echo $1|grep -o -P '[KMG]?B$')

declare -a arr_converter

converter () {
    case $2 in
        B)  
        arr_converter[0]=$(echo "Bytes = $1");

        kilo=$(echo "scale=.0; $1 / 1024" | bc -l);
        arr_converter[1]=$(printf "Kilobytes = %.4f" $kilo);

        mega=$(echo "scale=4; $1 / 1024 / 1024" | bc -l);
        arr_converter[2]=$(echo "Megabytes = $mega");

        giga=$(echo "scale=4; $1 / 1024 / 1024 / 1024" | bc -l);
        arr_converter[3]=$(echo "Gigbytes = $giga");
        ;;


        KB)
        bytes=$(( $1 * 1024 ));
        arr_converter[0]=$(printf "Bytes = %.f \n" $bytes);
        
        kilo=$1;
        arr_converter[1]=$(printf "Kilobytes = %.f \n" $1);
        
        mega=$(( $1 / 1024 ));
        arr_converter[2]=$(printf "Megabytes = %.2f \n" $mega);
        
        giga=$(echo "scale=4; $1 / 1024 / 1024" | bc -l);
        arr_converter[3]=$(echo "Gigabytes = $giga");
        ;;

        MB)
        bytes=$(( $1 * 1024 * 1024 ));
        arr_converter[0]=$(printf "Bytes = %.f \n" $bytes);

        kilo=$(( $bytes / 1024 ))
        arr_converter[1]=$(printf "Kilobytes = %.f \n" $kilo);

        arr_converter[2]=$(printf "Megbytes = %.f \n" $1);
        
        giga=$(( $1 / 1024 ));
        arr_converter[3]=$(printf "%.4f \n" $giga);
        ;;

        GB)
        bytes=$(( $1 * 1024 * 1024 * 1024 ));
        arr_converter[0]=$(printf "Bytes = %.f \n" $bytes);
        
        kilo=$(( $bytes / 1024 ))
        arr_converter[1]=$(printf "Kilobytes = %.f \n" $kilo);
        
        mega=$(( $kilo / 1024 ));
        arr_converter[2]=$(printf "Megbytes = %.f \n" $mega);
        
        arr_converter[3]=$(printf "Gigabytes = %.f \n" $1);
        ;;
        esac
}   


converter $NUM $UNIT

echo "..............."
for i in "${arr_converter[*]}"
do
    echo "$i"
    echo "..............."
done
