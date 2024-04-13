#!/bin/bash

# Tests the regular expressions in csv style test file
# First column of csv file is alphabet, second column is length, third column is regex
# NOTE: alphabet should not be surrounded by [] brackets

# This testing script does not cover:
#   {n,n} notation for repeats
#   Backreferences

testfile="test.csv"
separator=','

total=0
passed=0

while IFS="$separator" read -ra testarr; do
    alpha=${testarr[0]}
    len=${testarr[1]}
    regex=${testarr[2]}

    ./dgrep -a ["$alpha"] -l $len -r "$regex" > act.test
    ./dgrep -a ["$alpha"] -l $len -r '.*' | /bin/grep -E "^($regex)$" > exp.test

    diff=$(/bin/diff act.test exp.test)
    if [ "$diff" != "" ]
    then
        echo "Test failed: $regex"
    else
        let passed++
    fi
    let total++
done < "$testfile"

/bin/rm act.test exp.test
echo "Passed $passed / $total tests"