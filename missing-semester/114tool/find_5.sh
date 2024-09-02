#!/bin/bash
#

find . -type f -mmin -60 | xargs -d '\n' ls -lt | head -3
