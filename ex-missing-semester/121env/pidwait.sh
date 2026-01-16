#!/usr/bin/env bash
while kill -0 $1; do
	sleep 1
done

