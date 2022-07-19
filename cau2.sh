#! /usr/bin/bash

i=0
val=100
# while [ $i -lt 5 ]
# do
    > time_and_interval.txt
    echo  "$val" > "freq.txt"
    timeout 5s ./last_code
#     ((val/=10))
#     ((i++))
    
# done

