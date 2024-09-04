#!/bin/bash
for i in {0..9}; do
   journalctl -b-$i | grep "Startup finished in"
   echo 
done
