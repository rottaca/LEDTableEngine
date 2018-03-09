#!/bin/bash
set -e	# Exit on error
exec 2>&1

# unique timestamp
t=timestamp-$(date +%s)

dir=$(dirname "$(readlink -f "$0")")
echo "Go to source dir: $dir"
cd $dir

echo "Stash changes"
# stash with message
r=$(git stash save $t)
# check if the value exists
v=$(echo $r|grep $t)
if [ "$v" ]; then
  echo "Nothing to stash"
fi

echo "Pull project"
git pull --no-commit

if [ "$v" ]; then
  echo "Reapply stashed changes"
  git stash apply
fi

echo "Goto build directory"
cd build

echo "CMAKE..."
cmake ..

echo "Build project..."
make
