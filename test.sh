#!/bin/bash

# Tests the regular expressions in csv style test file
# First column of csv file is alphabet, second column is length, third column is regex
# NOTE: alphabet should not be surrounded by [] brackets

testfile="test.csv"
separator=','

while IFS="$separator" read -ra testarr; do
    alpha=${testarr[0]}
    len=${testarr[1]}
    regex=${testarr[2]}

    ./dgrep -a ["$alpha"] -l $len -r "$regex" > act.test
    ./dgrep -a ["$alpha"] -l $len -r '.*' | /bin/grep -e "^$regex$" > exp.test

    diff=$(/bin/diff act.test exp.test)
    if [ "$diff" != "" ]
    then
        echo "Test failed: $regex"
    fi
done < "$testfile"

/bin/rm act.test exp.test