#!/bin/bash

inputFile=$1
filename=$(basename "$inputFile")
filename="${filename%.*}"

convert "$inputFile" -resize 20x15\! $filename.bmp

echo "File saved as $filename.bmp"
