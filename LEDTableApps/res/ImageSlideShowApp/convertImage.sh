#!/bin/bash
dir=$(dirname "$(readlink -f "$0")")

inputFile=$1
if [ -d $inputFile ]; then
  echo "Specified parameter is directory."
  find "$inputFile" -maxdepth 1 -type f -iname "*.jpg"  -print0 | while IFS= read -r -d $'\0' line; do
      echo "Processing file $line"
      filename=$(basename "$line")
      filename="${filename%.*}"

      convert "$line" -resize 20x15\! "$dir/$filename.bmp"

      echo "File saved as $dir/$filename.bmp"
  done

elif [-f $inputFile ]; then
  echo "Specified parameter is file."
  filename=$(basename "$inputFile")
  filename="${filename%.*}"

  convert "$inputFile" -resize 20x15\! "$dir/$filename.bmp"

  echo "File saved as $dir/$filename.bmp"
fi
