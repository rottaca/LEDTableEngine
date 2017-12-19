#!/bin/bash



find . -type f \( -name '*.cpp' -o -name '*.hpp' \) -not -path "./build/*" -print0 |
    while IFS= read -r -d $'\0' line; do
        echo $line
        uncrustify -c uncrustify.cfg --no-backup $line | exit 1
    done
