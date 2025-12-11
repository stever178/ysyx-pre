#!/bin/sh
# usage: cd ex-dc; ../scripts/yosys-eval.sh ex?

top=$1

mkdir -p ./build
make -C $PRJ/yosys-sta sta \
   DESIGN=$top SDC_FILE=$(pwd)/../scripts/$top.sdc \
   CLK_FREQ_MHZ=100 CLK_PORT_NAME=clk O=$(pwd)/build/result_$top \
   RTL_FILES="$(pwd)/vsrc/$top.v"
