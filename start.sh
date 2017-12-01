#!/bin/bash

DIR=$(dirname $(readlink -f $0))
#echo $DIR
# Redirect output to log file
exec 2>&1
exec >${DIR}/log.txt



echo "Sleeping for 10 seconds befor start..."
sleep 10
echo "Starting main ...."
DISPLAY=:0 stdbuf -oL -eL /usr/bin/python ${DIR}/main.py --debug
