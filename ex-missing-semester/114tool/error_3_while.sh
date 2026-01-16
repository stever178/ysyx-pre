#!/bin/bash

count=0
RECORD="buggy.log"
echo '' > $RECORD

while true; do
	bash buggy.sh &>> "$RECORD"
	if [[ $? -ne 0 ]]; then
		echo "failed after $count times"		
		exit
	fi
	((count++))
done

