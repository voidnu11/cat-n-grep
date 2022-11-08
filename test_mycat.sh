#!/bin/bash

UNAME_S=`uname -s`

rm -f cat.*.out
#     cat       mycat
touch cat.0.out cat.1.out

exe=(cat ./mycat/mycat)
if [ $UNAME_S == "Linux" ]; then
    flags=("" -b -e -n -s -t -v -E -T)
elif [ $UNAME_S == "Darwin" ]; then
    flags=("" -b -e -n -s -t -v)
fi
lenExe=${#exe[@]}
lenFlags=${#flags[@]}

f=0
while [ $f -lt $lenFlags ]; do
    result="\033[1;31mFAIL\033[0m"

    if [ $f -eq 0 ]; then
        file="sample.txt sample2.txt"
    elif [ $f -eq 2 ]; then
        file="sample.txt sample2.txt"
    else
        file="sample.txt"
    fi

    e=0
    while [ $e -lt $lenExe ]; do
        echo "" >> cat.$e.out
        echo "TEST $f -- FLAG(s) ${flags[f]} --" >> cat.$e.out
        echo "" >> cat.$e.out
        ${exe[0]} ${flags[f]} ${file} >> cat.$e.out
        ((e++))
    done

    comm -2 -3 <(sort cat.0.out) <(sort cat.1.out) > cat_diff

    if [ $UNAME_S == "Linux" ]; then
        size=`stat -c %s cat_diff`
    elif [ $UNAME_S == "Darwin" ]; then
        size=`stat -f %z cat_diff`
    fi

    if [ $size -eq 0 ]; then
        result="\033[1;32mSUCCESS\033[0m"
    fi

    echo -e "TEST $f -- flag(s) ${flags[f]} ---- $result (./mycat ${flags[f]} ${file})"

    ((f++))
done
