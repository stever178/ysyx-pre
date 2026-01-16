#!/usr/bin/env bash

#默认会将空白字符（空格、制表符、换行符）作为分隔符
find . -type f -name "*.html" | xargs -d '\n' tar -cvzf html_1.zip

find . -type f -name "*.html" -print0 | xargs -0 tar -cvzf html_2.zip

