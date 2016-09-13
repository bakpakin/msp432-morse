#!/bin/bash

# Change this to the directory containing your ti install's gdb_agent_console
cd ../ti/gcc/emulation/common/uscif

./gdb_agent_console -f MSP432 xds110_msp432_jtag.dat
cd - > /dev/null
