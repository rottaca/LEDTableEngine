#!/bin/bash
dir=$(dirname "$(readlink -f "$0")")

if [[ "$#" -neq 3 ]]; then
  echo "Illigal number of parameters."
  echo "$0 DISPLAY_WIDTH DISPLAY_HEIGHT <filename/foldername>"
  echo "This tool converts the provided image or all images in the provided folder"
  echo "into a downscaled version that is stored at the script location."
  echo "It requires \"convert\" to be installed!"
  exit 1
fi

inputFile=$3
if [ -d $inputFile ]; then
  echo "Specified parameter is directory."
  find "$inputFile" -maxdepth 1 -type f -iname "*.jpg"  -print0 | while IFS= read -r -d $'\0' line; do
      echo "Processing file $line"
      filename=$(basename "$line")
      filename="${filename%.*}"

      convert "$line" -resize $1x$2\! "$dir/$filename.bmp"

      echo "File saved as $dir/$filename.bmp"
  done

elif [-f $inputFile ]; then
  echo "Specified parameter is file."
  filename=$(basename "$inputFile")
  filename="${filename%.*}"

  convert "$inputFile" -resize 20x15\! "$dir/$filename.bmp"

  echo "File saved as $dir/$filename.bmp"

fi
