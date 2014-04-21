awk -F "," '{ total += $1; cc += $2 } END { print total/NR ;print cc/NR }' camLog.txt 
