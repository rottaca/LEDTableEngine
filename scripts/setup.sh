#!/bin/bash
shopt -s nullglob

###################
## Check a few things for the setup
###################
BASE_DIR="$(dirname "$(readlink -f "$0")")/.."
CONFIG_FILE_PATH="LEDTableEngine/include/LEDTableEngine/configuration.hpp"
START_SCRIPT_PATH="scripts/start.sh"

# Check if our display tool is available
DISPLAY_CMD=`command -v whiptail`

if [[ -z "$DISPLAY_CMD" ]]; then
  echo "###############################################################"
  echo "The command whiptail is not available and has to be installed"
  echo "###############################################################"

  # Install whiptail
  sudo apt-get install whiptail && echo "Now, please restart the script!" && exit 0
fi

function abortSetup {
  $DISPLAY_CMD --clear --title "Setup Installer Script for LED Matrix Engine"\
               --msgbox "Setup aborted due to an error! Please try again or create an issue on GitHub!" 0 0
   exit 1
}

$DISPLAY_CMD --clear --title "Setup Installer Script for LED Matrix Engine"\
             --msgbox "\
This script is going to setup the LED Matrix Engine framework from GitHub:
  https://github.com/rottaca/LEDTableEngine" 0 0

# Check if we are on a raspberry pi
IS_RASPBERRY_PI=`cat /proc/device-tree/model 2>/dev/null | grep "Raspberry"`

if [[ -z "$IS_RASPBERRY_PI" ]]; then
  $DISPLAY_CMD --clear --title "Setup Installer Script for LED Matrix Engine"\
               --msgbox "\
  This is not a Raspberry Pi. The script will continue but the setup may fail!" 0 0
elif ($DISPLAY_CMD --clear --title "Setup Installer Script for LED Matrix Engine"\
               --yesno "Raspberry Pi detected!
If you are going to use the I2C controllers, you have to enable I2C in your
operating system. This tool will now start raspi-config and you have to select the
following options:
    (5) Interfacing Options
     -> (P5) I2C
        -> Answer all questions with YES!

Would you like to start raspi-config or skip this setup step because you already did this?
" 0 0) then

  sudo raspi-config
  $DISPLAY_CMD --clear --title "Setup Installer Script for LED Matrix Engine"\
               --msgbox "\
  We now have to reboot the computer. After booting, restart the setup and skip this setup step!" 0 0
  sudo reboot
  exit 0
fi

###################
## Install libraries
###################
if ($DISPLAY_CMD --clear --title "Setup Installer Script for LED Matrix Engine" \
    --yesno "\
The script will now install the following libraries:
- Git (git)
- SDL2 (libsdl2-dev)
- SDL2-Image (libsdl2-image-dev)
- g++ (g++)
- CMake  (cmake)
- I2C-Tools (i2c-tools)
- libi2c-dev (libi2c-dev)

Please enter your password if required and follow the instructions!
If you already installed all libraries, you can skip this setup step.

Would you like to install the libraries?
" 0 0) then
  # Install the required libraries
  sudo apt-get install libsdl2-dev libsdl2-image-dev g++ cmake i2c-tools libi2c-dev || abortSetup

  # Create default user if onone exists, otherwise git stash might fail
  if [[ -z "$(git config user.email)" ]]; then
    echo "Creating new (empty) git user account to allow stashing"
    git config user.email "(none)"
    git config user.name "Template User"
  fi
fi
###################
## Edit configuration header
###################

if ($DISPLAY_CMD --clear --title "Setup Installer Script for LED Matrix Engine" \
    --yesno "\
All required libraries are now installed!\

Please have a look at the configuration file and setup everything
correctly (e.g. width and height of matrix , mirroring, ...).
If you missed something, just restart the setup script.

The configuration will be opened in your default text editor. Open file or keep defaults?" 0 0) then
  select-editor || abortSetup
  sensible-editor "$BASE_DIR/$CONFIG_FILE_PATH" || abortSetup
fi

###################
## Build project
###################
$DISPLAY_CMD --clear --title "Setup Installer Script for LED Matrix Engine" \
               --msgbox "The script will now build the project the first time." 0 0

mkdir "$BASE_DIR/build" 2>/dev/null
cd "$BASE_DIR/build" || abortSetup

TMP_FILE=$(mktemp)

cmake .. | tee $TMP_FILE
# Cmake failed ?
if [[ "${PIPESTATUS[0]}" -ne "0" ]]; then
  abortSetup
fi
make || abortSetup

MSG="Build was successful!"
CTRL_SUPPORT=$(cat $TMP_FILE | grep "lib2c-dev found")
if [[ -z "$CTRL_SUPPORT" ]]; then
  MSG="$MSG
But controller support is disabeled! If you want to use the controllers,
please check if everything is setup correctly. Otherwise, continue the setup."
fi

$DISPLAY_CMD --clear --title "Setup Installer Script for LED Matrix Engine" --msgbox "$MSG" 0 0


###################
## Render to monitor or to matrix ?
###################
DISPLAY_MODE="matrix"

# $($DISPLAY_CMD --clear --title "Setup Installer Script for LED Matrix Engine" \
#              --menu "Please select, which display mode you want to use for your initial test." 0 0 0 \
#              "desktop" "Use the simulation environment to test you installation / controllers." \
#              "matrix" "Render the content to your LED Matrix." 3>&1 1>&2 2>&3)

# exitstatus=$?
# if [ $exitstatus -ne 0 ]; then
#  abortSetup
# fi
#
# if [[ "$DISPLAY_MODE" == "desktop" ]]; then
#   $DISPLAY_CMD --clear --title "Setup Installer Script for LED Matrix Engine" \
#                --msgbox "\
# You selected the simulation environment. This mode renders the content to a window
# on your computer monitor. Therefore, a graphical interface is required.
# If you don't have a desktop but only a command line interface, start the graphical
# environment and restart the script.
#
# For a Raspberry Pi (raspbian) execute the following in the command line:
#    startx
#
# If you are running a graphical interface, just continue with the setup." 0 0
# fi

###################
## Select the input device: keyboard or i2c?
###################
INPUT_DEVICE=$($DISPLAY_CMD --clear --title "Setup Installer Script for LED Matrix Engine" \
             --menu "Please select, which input device, you want to use for your initial test." 0 0 0 \
             "keyboard" "Use the keyboard as input device." \
             "i2c" "Use the i2c controllers as input device." 3>&1 1>&2 2>&3)

exitstatus=$?
if [ $exitstatus -ne 0 ]; then
 abortSetup
fi

if [[ "$INPUT_DEVICE" == "keyboard" ]]; then
  OPTIONS=()
  for f in /dev/input/by-path/*kbd*; do
    OPTIONS+=("$f")
    OPTIONS+=("_")
  done
  if [[ "${#OPTIONS[@]}" == "0" ]]; then
    $DISPLAY_CMD --clear --title "Setup Installer Script for LED Matrix Engine" \
                 --msgbox "Keyboard could not be detected! Please try again" 0 0

     abortSetup
  else
    INPUT_DEVICE_FILE=$($DISPLAY_CMD --clear --title "Setup Installer Script for LED Matrix Engine" --menu "\
You selected the keyboard as input device. Please select your keyboard from the list below.
If you only have a single keyboard connected, there should be only a single entry." 0 0 0 ${OPTIONS[@]} 3>&1 1>&2 2>&3)
    exitstatus=$?
    if [ $exitstatus -ne 0 ]; then
     abortSetup
    fi
  fi



elif [[ "$INPUT_DEVICE" == "i2c" ]]; then
  OPTIONS=()
  for f in /dev/i2c-*; do
    OPTIONS+=("$f")
    if [[ "$f" == "/dev/i2c-1" ]]; then
      OPTIONS+=("Usually_correct_for_a_Raspberry")
    else
      OPTIONS+=("_")
    fi
  done
  if [[ "${#OPTIONS[@]}" == "0" ]]; then
    $DISPLAY_CMD --clear --title "Setup Installer Script for LED Matrix Engine" \
                 --msgbox "I2C device file could not be detected!" 0 0
      abortSetup
  else
    INPUT_DEVICE_FILE=$($DISPLAY_CMD --clear --title "Setup Installer Script for LED Matrix Engine" --menu "\
You selected the i2c controller as input device. Please select your i2c device file from the list below.\
If you have multiple i2c device files, try it out or use the i2cdetect tool (see manual setup)." 0 0 0 ${OPTIONS[@]} 3>&1 1>&2 2>&3)
    exitstatus=$?
    if [ $exitstatus -ne 0 ]; then
       abortSetup
    fi
  fi

fi
###################
## Start framework
###################
if ($DISPLAY_CMD --clear --title "Setup Installer Script for LED Matrix Engine" \
    --yesno "Would you like to start the framework with your matrix display for the first time?
You need to have everything connected (e.g. controllers and matrix), otherwise the test fails.
If the test fails, you can still continue the setup.
To leave the framework, use CTRL-C and restart the setup." 0 0) then
  TMP_FILE=$(mktemp)

  if [[ "$INPUT_DEVICE" == "keyboard" ]]; then
    sudo $BASE_DIR/build/LEDTableMain -c $DISPLAY_MODE -i $INPUT_DEVICE -f $INPUT_DEVICE_FILE > $TMP_FILE 2>&1 &
  elif [[ "$INPUT_DEVICE" == "i2c" ]]; then
    $BASE_DIR/build/LEDTableMain -c $DISPLAY_MODE -i $INPUT_DEVICE -f $INPUT_DEVICE_FILE  > $TMP_FILE 2>&1 &
  fi

  if ($DISPLAY_CMD --clear --title "Setup Installer Script for LED Matrix Engine" \
      --yesno "Did the engine start successfully? Do the controllers/keyboard work?." 0 0) then
    killall LEDTableMain
  else
    $DISPLAY_CMD --clear --title "Setup Installer Script for LED Matrix Engine" \
                 --msgbox "Please have a look at the output to identify the error (Output will be displayed after closing this message)!" 0 0
    echo "###################################"
    echo "########### LOG OUTPUT ############"
    echo "###################################"
    cat $TMP_FILE
    killall LEDTableMain
    exit 0
  fi
fi

###################
## Install framework as cron job
###################
if ($DISPLAY_CMD --title "Setup Installer Script for LED Matrix Engine" \
    --yesno "Would you like to setup the automated start script (cron job)?
It allows you to automatically start the framework after booting
and it is the final setup step. This will always use the matrix as display device!" 0 0) then

  $DISPLAY_CMD --clear --title "Setup Installer Script for LED Matrix Engine" \
                   --msgbox "To setup the automated start, please insert the i2c device file name in the script header." 0 0

  select-editor || abortSetup
  sensible-editor "$BASE_DIR/$START_SCRIPT_PATH" || abortSetup

  $DISPLAY_CMD --clear --title "Setup Installer Script for LED Matrix Engine" \
                   --msgbox "The script will now register a new cron job that executes start script after booting." 0 0

  # Add script to cron tab only once
  CRONCMD="/bin/sh /home/pi/LEDTableEngine/scripts/start.sh"
  CRONJOB="@reboot $CRONCMD"
  ( crontab -l | grep -v -F "SHELL=/bin/sh" ; echo "SHELL=/bin/sh" ) | crontab -
  ( crontab -l | grep -v -F "$CRONCMD" ; echo "$CRONJOB" ) | crontab -


  $DISPLAY_CMD --clear --title "Setup Installer Script for LED Matrix Engine" \
          --msgbox "Cronjob installed! Test the autostart by rebooting the computer.
If an error occurs, check the log file, located at $BASE_DIR/log.txt" 0 0
fi

$DISPLAY_CMD --clear --title "Setup Installer Script for LED Matrix Engine" \
        --msgbox "You successfully completed the setup!
If you want to test a different configuration, just restart the setup script." 0 0
