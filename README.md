# Sample MSP432 Project

This is a sample setup for a project with an MSP432 microcontroller that can be built
with standard gcc tools.

## Prerequisites for building

You will need to install the gcc arm compiler for the microcontroller from you package manager.
On Ubunutu, you can install with

```
sudo apt install gcc-arm-none-eabi gdb-arm-none-eabi
```

Then run `make` to build your project to the output directory.

To actually run and debug the project, you will need some more things.

## Running and debugging
