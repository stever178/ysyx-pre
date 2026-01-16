#!/bin/bash

OUT=starttime.txt
if [ ! -f $OUT ]; then
	bash getlog.sh 2>/dev/null > "$OUT"
fi

cat starttime.txt | grep "systemd\[1\]" | sed -E "s/.*=\ (.*)s\.$/\1/" 

echo 'Average: '
cat starttime.txt | grep "systemd\[1\]" | sed -E "s/.*=\ (.*)s\.$/\1/" | sed "s/min /*60.0+/" | paste -sd+ | bc -l | awk '{print $1 / 10}'

echo 'midian: '
cat starttime.txt | grep "systemd\[1\]" | sed -E "s/.*=\ (.*)s\.$/\1/" | sed "s/min /*60.0+/" | bc -l | sort -n | paste -sd\  | awk '{print ($5+$6)/2}'
# or
cat starttime.txt | grep "systemd\[1\]" | sed -E "s/.*=\ (.*)s\.$/\1/" | sed "s/min /*60.0+/" | bc -l | sort -n | xargs | awk '{print ($5+$6)/2}'

