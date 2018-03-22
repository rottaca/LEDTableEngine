#!/bin/bash
#set -e	# Exit on error
exec 2>&1

# unique timestamp
t=timestamp-$(date +%s)

dir="$(readlink -f $(dirname $(readlink -f $0))/..)"
echo ">>>>>>>> Go to source dir: $dir"
cd $dir || exit 1

echo ">>>>>>>> Stash changes"
# stash with message
r=$(git stash save $t)
# check if the value exists
v=$(echo $r | grep $t)

echo ">>>>>>>> Pull project"
git pull --no-commit  || exit 1

if [ "$v" ]; then
  echo ">>>>>>>> Reapply stashed changes"
  git stash apply  || exit 1
fi

echo ">>>>>>>> Goto build directory"
cd build  || exit 1

echo ">>>>>>>> CMAKE..."
cmake ..  || exit 1

echo ">>>>>>>> Build project..."
make || exit 1
