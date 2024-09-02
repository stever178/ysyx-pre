#!/bin/bash
#
#默认会将空白字符（空格、制表符、换行符）作为分隔符
find $1 -type f -name "*.html" | xargs -d '\n' tar -cvzf html.zip
