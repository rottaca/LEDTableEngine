#!/bin/bash

DIR=$(dirname $(readlink -f $0))
#echo $DIR
# Redirect output to log file
exec 2>&1
exec >${DIR}/log.txt



echo ">>>>>>> Sleeping for 10 seconds befor start..."
sleep 10
echo ">>>>>>> Starting main ...."
keybardDev=$(find /dev/input/by-path/ -name "*kbd*" | head -1)
if [ -z "$keybardDev" ]; then
  echo ">>>>>>> Failed to find valid keyboard event file!"
else
  cd $DIR/build
  restartCounter=0
  until DISPLAY=:0 stdbuf -oL -eL ./LEDTable -c matrix -k $keybardDev; do
    restartCounter=$((restartCounter + 1))
    if (( $restartCounter == 10 )); then
      echo ">>>>>>> Restart failed!"
      exit 1
    else
      echo ">>>>>>> Restart engine, due to crash! Attempt $restartCounter of 10"
      sleep 1
    fi
  done
fi
