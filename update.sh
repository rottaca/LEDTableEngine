#!/bin/bash
set -e	# Exit on error
exec 2>&1

dir=$(dirname "$(readlink -f "$0")")
echo "Go to source dir: $dir"
cd $dir
echo "Stash changes"
git stash
echo "Pull project"
git pull
echo "Reapply stashed changes"
git stash apply
echo "Goto build directory"
cd build
echo "CMAKE..."
cmake ..
echo "Build project..."
make
