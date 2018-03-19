#!/bin/bash

HEIGHT=30
WIDTH=80

dir="$(dirname "$(readlink -f "$0")")/.."

APP_DIR_SRC="$(readlink -f $dir/LEDTableApps/src)"
APP_DIR_HDR="$(readlink -f $dir/LEDTableApps/include/LEDTableApps)"
APP_CMAKE_FILE="$(readlink -f $dir/LEDTableApps/CMakeLists.txt)"
TEMPLATE_DIR="$(readlink -f $dir/scripts/templates)"

# Create a temporary file and make sure it goes away when we're done
echo "Create temporary file..."
tmp_file=$(tempfile 2>/dev/null) || tmp_file=/tmp/test$$
trap "rm -f $tmp_file" 0 1 2 5 15

whiptail --clear --title 'Create new application' --msgbox 'Create a new application, based on a template!' $HEIGHT $WIDTH
echo "Gather application name..."
whiptail --clear --inputbox 'New applicaton name (CamelCase):' $HEIGHT $WIDTH "" 2> $tmp_file
# Check for abort
if (( $? != 0 )); then
  exit 1
fi
echo "Check if name is valid..."
# Parse result and check (very simple) for camel case
APP_NAME=$(cat $tmp_file | sed -rn 's/([A-Z][a-zA-Z]+)/\1/p')
# Make first char lower case
APP_NAME_FILE=$(cat $tmp_file | sed -rn 's/([A-Z])([a-zA-Z]+)/\l\1\2/p')
if [ -z "$APP_NAME" ] || [ -z "$APP_NAME_FILE" ]; then
  echo "Invalid application name."
  exit 1
fi

# Check if there is already a file with that name
res=$(find $APP_DIR_HDR -type f | grep "$APP_NAME_FILE.hpp")
# If we got something as a result, there is already an existing app
if [[ ! -z "$res" ]]; then
  echo "An application with this name already exists!"
  exit 1
fi

echo "Select template type..."
whiptail --clear --menu "Select your application template for $APP_NAME:" $HEIGHT $WIDTH 0 \
           "1" "RGB Color Mode (complex colors, multi purpose)" \
           "2" "Palette Color Mode (limited colors, multi purpose)" \
           "3" "Shader Application (RGB, convinience class, for dyn. animation)" 2> $tmp_file
# Check for abort
if (( $? != 0 )); then
 exit 1
fi

APP_TEMPLATE_TYPE=$(cat $tmp_file)

echo "Copy template files..."
case $APP_TEMPLATE_TYPE in
  1 )
  cp $TEMPLATE_DIR/templateRGBApp.cpp $APP_DIR_SRC/$APP_NAME_FILE.cpp
  cp $TEMPLATE_DIR/templateRGBApp.hpp $APP_DIR_HDR/$APP_NAME_FILE.hpp
    ;;
  2 )
  cp $TEMPLATE_DIR/templatePaletteApp.cpp $APP_DIR_SRC/$APP_NAME_FILE.cpp
  cp $TEMPLATE_DIR/templatePaletteApp.hpp $APP_DIR_HDR/$APP_NAME_FILE.hpp
    ;;
  3 )
  cp $TEMPLATE_DIR/templateShaderApp.cpp $APP_DIR_SRC/$APP_NAME_FILE.cpp
  cp $TEMPLATE_DIR/templateShaderApp.hpp $APP_DIR_HDR/$APP_NAME_FILE.hpp
    ;;
esac

echo "Replace placeholders in header with content..."
sed -ri "s/TEMPLATE_APP_INC_GUARD_NAME/\U$APP_NAME/g" $APP_DIR_HDR/$APP_NAME_FILE.hpp
sed -ri "s/TEMPLATE_APP_NAME/$APP_NAME/g" $APP_DIR_HDR/$APP_NAME_FILE.hpp
echo "Replace placeholders in source with content..."
sed -ri "s/TEMPLATE_APP_NAME/$APP_NAME/g" $APP_DIR_SRC/$APP_NAME_FILE.cpp
sed -ri "s/TEMPLATE_APP_INC_HEADER/$APP_NAME_FILE/g" $APP_DIR_SRC/$APP_NAME_FILE.cpp
echo "Add class to CMakeLists.txt..."
sed -ri $'s/(SET\\(APP_SRC.*)/\\1\\\n  src\\/'$APP_NAME_FILE'\.cpp/g' $APP_CMAKE_FILE

echo "Your application has been created!
The header file is located at:
$APP_DIR_HDR/$APP_NAME_FILE.hpp
The source file is located at:
$APP_DIR_SRC/$APP_NAME_FILE.cpp

To add the application to the menu, add the include to your header file to
the main.cpp. Also insert the following snippet to the menu creation section
in the main.cpp. (Have a look at the existing code to get an idea how it works):
    TextMenu::MenuEntry(\"$APP_NAME\",
    std::make_shared<AppLauncher>(c, std::make_shared<$APP_NAME>()))" > $tmp_file
whiptail --title 'Create new application' --textbox $tmp_file $HEIGHT $WIDTH
# print to commandline
echo "########################################"
cat $tmp_file
echo "########################################"
