# stmos

stmos is an operating system for STM microprocessors. It aims to support (or be able to support) all STM Cortex-M processors.  
  
Features:  
* Unprivileged code execution
* ELF loading and execution (loads an init from the initrd)
* Near-full C standard library support for loaded ELFs
* libgpio: GPIO access for loaded ELFs
  
Currently supported processors:
* STM32L476RG (board: NUCLEO-L476RG)
  
### building  
To build stmos, you'll need the following programs:  
* The arm-stmos toolchain (see the ```arm-stmos``` folder)
* openocd
* gdb (with multiarch support)
  
To compile stmos simply run ```make``` (-j option supported). To upload, use ```run.sh``` to start openocd/gdb; do ```lo``` to load stmos, and ```c``` to run it.
