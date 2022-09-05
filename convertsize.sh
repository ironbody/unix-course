#!/bin/bash

NUM=$(echo $1|grep -o '^\d*')
UNIT=$(echo $1|grep -o '')

echo $NUM
