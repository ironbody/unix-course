#!/bin/bash

compressfile() {
  tar -czvf $1.tar.gz $1
}

if [[ $# -ne 1 ]]
then
  echo "pass exactly one argument"
  exit
fi

if [[ ! -d $1 ]]
then 
  echo "argument is not a directory"
  exit
fi
echo "$(dirname $1)"

DIR="$(cd "$(dirname $1)"; pwd)/$(basename "$1")"
echo $DIR
echo $PWD

case $DIR in
  $PWD/$(basename "$1"))
  	# tom
  ;;
  *) 
  	echo "elsewhere"
  	exit
  ;;
esac

if [[ ! -w $1 ]]
then
  echo "can not write the compressed file to the current directory"
fi


DIR_SIZE=$(du -cb $1 | grep 'total' | awk '{print $1}')
LARGE_SIZE=$((512 * 1000000))
# warn if dir > 512MB
echo $DIR_SIZE
echo $LARGE_SIZE
if [[ $DIR_SIZE -ge $LARGE_SIZE ]]
then
read -r -p "Are you sure? [y/N] " response
case "$response" in
    [yY][eE][sS]|[yY]) 
      echo "Warning: the directory is 512 MB. Proceed? [y/n]"
      compressfile
      # do_something
      ;;
    [nN]o|[nN])
      echo "Aborting compression"
      exit
      #do_something_else
      ;;
    *)
      echo ""
      ;;
esac
fi

echo "compressing file"
compressfile $1