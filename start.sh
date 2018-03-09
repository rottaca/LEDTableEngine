#!/bin/bash
# This script starts the LEDTableEngine automatically.
# It tries to detect the valid input devices and launches the Engine.
# If an error occurs, the engines is restarted up to 10 times.
# If a restart is not possible (due to configuration errors),
# the script will abort.


# The i2c device file, that has to be used
# to connect to the game controllers
i2cDev="/dev/i2c-1"

DIR=$(dirname $(readlink -f $0))
#echo $DIR
# Redirect output to log file
exec 2>&1
exec >${DIR}/log.txt

echo ">>>>>>> Kill all running LEDTable executables..."
killall LEDTable
echo ">>>>>>> Sleeping for 10 seconds befor start..."
sleep 10
echo ">>>>>>> Try to find the I2C device ....."
startParams=""
# Check if i2c file exists
if [ -e "$i2cDev" ];then
  startParams="-i i2c -I $i2cDev"
  echo ">>>>>>> I2C device exists....."
else
  echo ">>>>>>> Failed to find valid i2c device ....."
  echo ">>>>>>> Try to find keyboard instead ....."
  # Automatically find keyboard file
  keybardDev=$(find /dev/input/by-path/ -name "*kbd*" | head -1)
  if [ -z "$keybardDev" ]; then
    echo ">>>>>>> Failed to find valid keyboard event file!"
  else
  echo ">>>>>>> Keyboard found: $keybardDev"
    startParams="-i keyboard -k $keybardDev"
  fi
fi

if [ ! -z "$startParams" ]; then
  echo ">>>>>>> Starting main ...."
  cd $DIR/build
  restartCounter=0
  until DISPLAY=:0 stdbuf -oL -eL ./LEDTable -c matrix $startParams; do
    restartCounter=$((restartCounter + 1))
    if [ "$restartCounter" -eq "10" ]; then
      echo ">>>>>>> Restart failed!"
      exit 1
    else
      echo ">>>>>>> Restart engine, due to crash! Attempt $restartCounter of 10"
      sleep 1
    fi
  done
else
  echo ">>>>>>> Failed to start. No input device found ...."
  exit 1
fi
