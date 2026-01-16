#!/usr/bin/env bash

git log -1 README.md

git blame _config.yml | grep "collections" | cut -d' ' -f1 | xargs git show
