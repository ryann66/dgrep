#!/bin/bash

len="10"

while read -r test; do
    ./dgrep -l$len -r $test > act.test
    ./dgrep -l$len -r '.*' | /bin/grep -e $test > exp.test
done < test.txt