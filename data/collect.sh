#!/bin/bash

echo $1

cat stats-0.csv > $1
tail -n +2 stats-1.csv >> $1
tail -n +2 stats-2.csv >> $1
tail -n +2 stats-3.csv >> $1
