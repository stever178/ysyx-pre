#!/bin/bash
#

count=0
RECORD="buggy.log"
 > $RECORD
while true; do
	bash buggy.sh &>> "$RECORD"
	if [[ $? != 0 ]]; then
		cat "$RECORD"
		echo "failed after $count times"		
		exit
	fi
	((count++))
done
