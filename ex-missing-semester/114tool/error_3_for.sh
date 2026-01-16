#!/usr/bin/env bash

RECORD="buggy.log"
echo '' > $RECORD

for ((count=0;;count++)); do
    bash buggy.sh &>> $RECORD
    if [[ $? -ne 0 ]]; then
        echo "failed after $count times"
        break
    fi
done

