#!/usr/bin/env bash

# Change this to the directory containing your ti install.
# This was the defualt on linux I think.
TI_INSTALL_DIR=$HOME/ti

cd $TI_INSTALL_DIR/gcc/emulation/common/uscif
./gdb_agent_console -f MSP432 xds110_msp432_jtag.dat
cd - > /dev/null
