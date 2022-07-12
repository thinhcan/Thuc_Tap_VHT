#! /usr/bin/bash

i=0
val=1000000
while [ $i -lt 5 ]
do
    echo  "$val" > "freq.txt"
    timeout 3s ./code_test
    ((val/=10))
    ((i++))
    
done

