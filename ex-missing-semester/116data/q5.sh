
OUT="last3start.txt"
journalctl -b -2 -b -1 -b 0 > "$OUT"
cut -d' ' -f5- "$OUT" > tmp.txt

#cat last3start.txt | sed -E "s/.*pi\ (.*)/\1/" | sort | uniq -c | sort | awk '$1!=3  { print }'
