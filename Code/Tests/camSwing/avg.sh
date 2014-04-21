#!/bin/bash
awk '{ total += $1 } END { print total/NR }' camLog.txt
