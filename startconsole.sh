#!/bin/bash

cd ../ti/gcc/emulation/common/uscif
./gdb_agent_console -f MSP432 xds110_msp432_jtag.dat
cd - > /dev/null
