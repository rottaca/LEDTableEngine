#!/bin/bash

DIR=$(dirname $(readlink -f $0))
#echo $DIR
# Redirect output to log file
exec 2>&1
exec >${DIR}/log.txt



echo "Sleeping for 10 seconds befor start..."
sleep 10
echo "Starting main ...."
keybardDev=$(find /dev/input/by-path/ -name "*kbd*" | head -1)
if [ -z "$keybardDev" ]; then
  >2 echo "Failed to find valid keyboard event file!"
else
  DISPLAY=:0 stdbuf -oL -eL /home/pi/LEDTableEngine/build/LEDTable -c matrix -k $keybardDev
fi
