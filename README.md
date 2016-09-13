# Sample MSP432 Project

This is a sample setup for a project with an MSP432 microcontroller that can be built
with standard opensource gcc tools.

If you're not interested in the project setup, the unique c source is in src/main.c.

## Prerequisites for building

You will need to install the gcc arm compiler for the microcontroller from you package manager.
On Ubunutu, you can install with

```
sudo apt install gcc-arm-none-eabi gdb-arm-none-eabi
```

Then run `make` to build your project to the output directory. This produces a binary [something].out in
the `output` folder.

To actually run and debug the project, you will need some more things. The xds110 debugger from TI
works the best right now, but I would like to get openocd working. There are numerous snippets for openocd
support for the msp432p401r around the internet, but none of them worked for me with the most recent version
of openocd.

## Running and debugging

Follow the directions in the official TI instructions for the GCC toolchain.

[http://www.ti.com/lit/ug/slau688/slau688.pdf](http://www.ti.com/lit/ug/slau688/slau688.pdf)

## Using this template for your own projects

Simply clone this directory, and add your source c and header files
into the src directory.

