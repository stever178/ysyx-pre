#!/bin/bash

grep 'a.*a.*a.*' /usr/share/dict/words | grep -v "'s$" | wc -l
grep -E '(a.*){3}' /usr/share/dict/words | grep -v "'s$" | wc -l
grep -E '([^a]*a){3,}' /usr/share/dict/words | grep -v "'s$" | wc -l

echo

grep 'a.*a.*a.*' /usr/share/dict/words | grep -v "'s$" | sed -E "s/.*([a-z]{2})/\1/" | sort | uniq -c | sort -n | tail -n3 

ALL="all.txt"
> "$ALL"
for i in {a..z};do
 for j in {a..z};do
    echo "$i$j" >> "$ALL"
 done
done

OUT="out.txt"
grep 'a.*a.*a.*' /usr/share/dict/words | grep -v "'s$" | sed -E "s/.*([a-z]{2})/\1/" | sort | uniq -c | sort -n > "$OUT"

#diff "$ALL" "$OUT" > result.txt
diff <(cat "$ALL") <(cat "$OUT") > result.txt
#diff --unchanged-group-format='' "$ALL" "$OUT" > result.txt
