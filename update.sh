#!/bin/bash
set -e	# Exit on error
exec 2>&1

dir=$(dirname "$(readlink -f "$0")")
echo "Go to source dir: $dir"
cd $dir
echo "Pull project"
git pull
echo "Goto build directory"
cd build
echo "CMAKE..."
cmake ..
echo "Build project..."
make
