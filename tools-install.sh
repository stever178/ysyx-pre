#!/bin/bash

# verilator prerequisites
sudo apt-get install -y git help2man perl python3 make autoconf g++ flex bison ccache
sudo apt-get install -y libgoogle-perftools-dev numactl perl-doc
sudo apt-get install -y libfl2  # Ubuntu only (ignore if gives error)
sudo apt-get install -y libfl-dev  # Ubuntu only (ignore if gives error)
sudo apt-get install -y zlibc zlib1g zlib1g-dev  # Ubuntu only (ignore if gives error)

unset VERILATOR_ROOT  # For bash

#
sudo apt-get install -y gtkwave
