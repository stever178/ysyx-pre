#!/usr/bin/env bash

set -e  # 脚本遇到错误会立即退出

if [ ! -f ./ex17 ]; then
    echo "==== Now compiling ex17 ===="
    make ex17
fi

echo "==== Now running ex17 ===="
TEST_LOG="test.log"
echo "test output is recorded at ${TEST_LOG}"

./ex17 db.dat c
./ex17 db.dat s 1 zed zed@zedshaw.com
./ex17 db.dat s 2 frank frank@zedshaw.com
./ex17 db.dat s 3 joe joe@zedshaw.com
./ex17 db.dat l > "${TEST_LOG}"

EXPECTED="1 zed zed@zedshaw.com
2 frank frank@zedshaw.com
3 joe joe@zedshaw.com"

if [ "$(cat ${TEST_LOG})" = "$EXPECTED" ]; then
    echo "匹配成功"
else
    echo "匹配失败"
	exit 1
fi

./ex17 db.dat d 3
./ex17 db.dat d 1
./ex17 db.dat l > "${TEST_LOG}"

EXPECTED="2 frank frank@zedshaw.com"

if [ "$(cat ${TEST_LOG})" = "$EXPECTED" ]; then
    echo "匹配成功"
else
    echo "匹配失败"
	exit 1
fi

echo "==== All tests passed ===="

echo "==== Now cleaning up ===="
rm ex17 ${TEST_LOG}
