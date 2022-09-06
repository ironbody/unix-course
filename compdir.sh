#!/bin/bash

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

DIR="$(cd "$(dirname $1)"; pwd)/$(basename "$1")"
echo $DIR
echo $PWD
case $DIR in
  $PWD/$(basename "$1")) echo "here";;
  *) echo "elsewhere";;
esac
