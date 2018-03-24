#!/bin/bash
# This script starts the LEDTableEngine automatically.
# It tries to detect the valid input devices and launches the Engine.
# If an error occurs, the engines is restarted up to 10 times.
# If a restart is not possible (due to configuration errors),
# the script will abort.


# The i2c device file, that has to be used
# to connect to the game controllers.
# Set to an empty string "" if you don't want
# to use controllers
# e.g. /dev/i2c-1
i2cDev=""

# The keyboard device file, that has to be used
# Set to an empty string "" if you don't want
# to use a keyboard
keyboardDev=""
# If you want to automatically find the first
# keyboard file, uncomment this line:
#keybardDev=$(ls /dev/input/by-path/*kbd* | head -1)

DIR="$(readlink -f $(dirname $(readlink -f $0))/..)"

# Redirect output to log file
exec 2>&1
exec >${DIR}/log.txt

echo ">>>>>>> Kill all running LEDTable executables..."
killall LEDTableMain
echo ">>>>>>> Sleeping for 5 seconds befor start..."
sleep 5
echo ">>>>>>> Try to find the I2C device ....."
startParams=""
# Check if i2c file exists
if [ ! -z "$i2cDev" ] && [ -e "$i2cDev" ];then
  startParams="-i i2c -f $i2cDev"
  echo ">>>>>>> I2C device exists....."
else
  echo ">>>>>>> Failed to find valid i2c device ....."
  echo ">>>>>>> Try to find keyboard instead ....."
  if [ ! -z "$keybardDev" ] &&  [ -e "$keybardDev" ]; then
    echo ">>>>>>> Keyboard found: $keybardDev"
      startParams="-i keyboard -f $keybardDev"
  else
    echo ">>>>>>> Failed to find valid keyboard event file ....."
  fi
fi

if [ ! -z "$startParams" ]; then
  echo ">>>>>>> Starting main ...."
  cd $DIR/build
  restartCounter=0
  until DISPLAY=:0 stdbuf -oL -eL ./LEDTableMain -c matrix $startParams 2>&1; do
    restartCounter=$((restartCounter + 1))
    if [ "$restartCounter" -eq "10" ]; then
      echo ">>>>>>> Restart failed!"
      exit 1
    else
      echo ">>>>>>> Restart engine, due to unexpected exit! Attempt $restartCounter of 10"
      sleep 1
    fi
  done
else
  echo ">>>>>>> Failed to start. No input device found ...."
  exit 1
fi
