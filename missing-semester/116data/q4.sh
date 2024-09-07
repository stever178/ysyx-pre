#!/bin/bash

OUT=starttime.txt
if [ ! -f $OUT ]; then
	bash getlog.sh 2>/dev/null > "$OUT"
fi

cat starttime.txt | grep "systemd\[1\]" | sed -E "s/.*=\ (.*)s\.$/\1/" 
echo
cat starttime.txt | grep "systemd\[1\]" | sed -E "s/.*=\ (.*)s\.$/\1/" | sed "s/min /*60.0+/" | bc -l | sort -n 
