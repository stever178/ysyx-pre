#!/bin/sh

git log --oneline --since="yesterday 08:00" --until="today 02:00" --numstat | awk '
BEGIN { add = 0; del = 0 }
/^[0-9-]+[[:space:]]+[0-9-]+[[:space:]]+/ {
    add += $1
    del += $2
}
END { printf "%d insertions(+), %d deletions(-), %d total\n", add, del, add + del }
'
