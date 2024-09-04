
cat starttime.txt | grep "systemd\[1\]" | sed -E "s/.*=\ (.*)s\.$/\1/" | sed "s/min /*60.0+/" | bc -l | sort -n
