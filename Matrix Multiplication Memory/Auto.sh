#!/usr/bin/env bash

for n in 16, 64, 256, 1024, 4096, 16384
do
    for m in 1677721600, 13421772800
    do
             echo "Let's do it for n=$n, m=$m"
             time ./prog4 $n $m
    done
done
