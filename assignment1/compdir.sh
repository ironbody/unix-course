#!/bin/bash

compressfile() {
  tar -czf $1.tgz $1
  echo "Directory $1 archived as $1.tgz"
}

if [[ $# -ne 1 ]]
then
  echo "No arguments given! Give a directory as input"
  exit
fi

if [[ ! -d $1 ]]
then 
  echo "compdir.sh: cannot find directory $1"
  exit
fi
# echo "$(dirname $1)"

DIR="$(cd "$(dirname $1)"; pwd)/$(basename "$1")"
# echo $DIR
# echo $PWD

case $DIR in
  $PWD/$(basename "$1"))
  	# tom
  ;;
  *) 
  	echo "compdir.sh: you must specify a subdirectory"
  	exit
  ;;
esac

if [[ ! -w $1 ]]
then
  echo "can not write the compressed file to the current directory"
  exit
fi


DIR_SIZE=$(du -cb $1 | grep 'total' | awk '{print $1}')
LARGE_SIZE=$((512 * 1000000))
# warn if dir > 512MB
# echo $DIR_SIZE
# echo $LARGE_SIZE
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

# echo "compressing file"
compressfile $1