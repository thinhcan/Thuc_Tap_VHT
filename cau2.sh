#! /usr/bin/bash

i=0
val=1000000
 while [ $i -lt 5 ]
 do
    > time_and_interval.txt
    echo  "$val" > "freq.txt"
    timeout 60s ./last_code
     ((val/=10))
     ((i++))
    
 done

