#!/bin/bash

if [ $# = 1 ]
then
    if [ -d $1 ]
    then
        DIR=$(echo "$(du -h $1 | awk '{print $1}')");
        DIR_SIZE=$(echo "$(du -h $1 | awk '{print $2}')");
        PREMISSIONS=$(ls -l | awk '{if (NR==5) {print $1}}');
        echo "$1 exists";
        

        cd $1/..
        pwd
        #OWNER_PRE=$(ls -l | awk '{if (NR>1) ('$1 ~ /^d.w/') {print}}';
        if [ OWNER_PRE ]
        then
            # warn if dir > 512MB
            echo "In parent folder";
            echo "in if loop";
            echo $OWNER_PRE;
            tar -czvf ./$DIR.tar.gz $DIR
        else 
            echo "can not write the compressed file to the current directory";

        fi;
    else
        echo "$1 doesn't exist";
        echo "Not a directory! Give a directory as input";
    fi;
else
    if [ $# = 0 ]
    then
        echo "No arguments given! Give a directory as input";
    else
        echo "Too many arguments given! Give ONLY one directory as input";
    fi;
fi;

# echo $DIR
# echo $DIR_SIZE
