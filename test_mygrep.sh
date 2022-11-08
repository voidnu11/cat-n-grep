#!/bin/bash

UNAME_S=`uname -s`

rm -f grep.*.out
#     grep       mygrep
touch grep.0.out grep.1.out

exe=(grep ./mygrep/mygrep)
flags=('' -e -f -s -h -i -v -c -l -n -o -cv)
lenExe=${#exe[@]}
lenFlags=${#flags[@]}

f=0
while [ $f -lt $lenFlags ]; do
    result="\033[1;31mFAIL\033[0m"

    pattern="123"
    if [ $f -eq 2 ]; then
        pattern="pattern.txt"
    fi

    if [ $f -eq 3 ]; then
        file="sample.tt"
    elif [ $f -eq 4 ]; then
        file="sample.txt sample2.txt"
    else
        file="sample.txt"
    fi

    e=0
    while [ $e -lt $lenExe ]; do
        echo "" >> grep.$e.out
        echo "TEST $f -- FLAG '${flags[f]}' --" >> grep.$e.out
        echo "" >> grep.$e.out
        ${exe[e]} ${flags[f]} ${pattern} ${file} >> grep.$e.out
        ((e++))
    done

    comm -2 -3 <(sort grep.1.out) <(sort grep.0.out) > grep_diff

    if [ $UNAME_S == "Linux" ]; then
        size=`stat -c %s grep_diff`
    elif [ $UNAME_S == "Darwin" ]; then
        size=`stat -f %z grep_diff`
    fi

    if [ $size -eq 0 ]; then
        result="\033[1;32mSUCCESS\033[0m"
    fi

    echo -e "TEST $f -- flag(s) ${flags[f]} ---- $result (./mygrep ${flags[f]} ${pattern} ${file})"
    ((f++))
done


