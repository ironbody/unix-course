#!/bin/sh
FILE=$1

if [ ! -f $FILE ]
then 
	echo "no file"
fi

NAME=`basename $FILE | cut -d '.' -f 1`
EXT=`basename $FILE | cut -d '.' -f 2`

if [ "$EXT" != "calc" ]
then
	echo "no"
fi

OUT_FILE="${NAME}.s"

echo ".section .data
string: 
 	.asciz	\"%d\\\n\"

sym: 
	.ds.d	26

.section .text
	.global main
	.type main, @function
main:
	push 	%rbx" > $OUT_FILE

./bin/calc3i.exe < $FILE >> $OUT_FILE

echo "	pop 	%rbx
	xor 	%rax, %rax
	ret" >> $OUT_FILE

gcc -m64 -fPIC -g -o $NAME $OUT_FILE -L./lib -lgcd -lfact -llntwo

# 1. writes the required prologue (e.g. define data and text segment, define the
# symbol table) into a new file with the ending ’.s’
# 2. appends this file with the output of your compiler (i.e., calc3i.exe) for the
# given ’.calc’ file
# 3. appends an epilogue (e.g. for calling the exit function/system call)

# gcc -m64 -g -no-pie "test2.s" 