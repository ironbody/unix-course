#!/bin/bash

# bzip2 and gzip replace the original files with
# compressed versions

type=""

find_type() {
  if [[ $1 == "gz" ]]
  then
    type="gzip" 
  fi;

  if [[ $1 == "bz2" ]]
  then
    type="bzip2" 
  fi;

  if [[ $1 == "7z" ]]
  then
    type="p7zip" 
  fi;

  if [[ $1 == "lzo" ]]
  then
    type="lzop" 
  fi;
}

filename=$(basename $1)

if [[ ! -f $1 ]]
then
  echo "Not a file. Exiting"
  exit
fi;

if [[ ! -r $1 ]]
then
  echo "No read premission. Exiting"
  exit
fi;

# gzip
gzip -k9c $1 > ./$filename.gz &
# bzip2
bzip2 -k9c $1 > ./$filename.bz2 &
# lzop
lzop -k9c $1 > ./$filename.lzo &
# p7zip, c
p7zip -k -c $1 > ./$filename.7z &
wait

# sort the sizes in asc order and takes out the first row (smallest element)
compress_file_name=$(ls -Srl $filename* | awk '{if (NR==1) {print $9}}')

# get the ending of the compressed file
compress_ending=$(echo $compress_file_name | cut -d "." -f3)

org_size=$(du -b $1 | cut -f1)

comp_size=$(du -b $compress_file_name | cut -f1)

find_type $compress_ending

if [[ $comp_size -ge $org_size ]]
then
  # keep orignal and throw away the compressed files
  ls $filename.* | xargs rm
  echo "All compressed files are larger than original. Keeping orignal and aborting compression."
else
  # remove all files except the smallest compressed file
  rm $1
  ls $filename.* | grep -v $compress_ending | xargs rm
  echo "Most compression obtained with $type. Compressed file is $compress_file_name"
fi;
