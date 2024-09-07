#!/bin/bash
#
set -e  # 脚本遇到错误会立即退出

RECORD="test.log"
echo "test output is recorded at $RECORD"

./ex17 db.dat c
./ex17 db.dat s 1 zed zed@zedshaw.com
./ex17 db.dat s 2 frank frank@zedshaw.com
./ex17 db.dat s 3 joe joe@zedshaw.com
./ex17 db.dat l > "$RECORD"

EXPECTED="1 zed zed@zedshaw.com
2 frank frank@zedshaw.com
3 joe joe@zedshaw.com"

if [ "$(cat $RECORD)" = "$EXPECTED" ]; then
    echo "匹配成功"
else
    echo "匹配失败"
	exit 1
fi

./ex17 db.dat d 3
./ex17 db.dat d 1
./ex17 db.dat l > "$RECORD"

EXPECTED="2 frank frank@zedshaw.com"

if [ "$(cat $RECORD)" = "$EXPECTED" ]; then
    echo "匹配成功"
else
    echo "匹配失败"
	exit 1
fi


