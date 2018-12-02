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
  
### notes
Only the initrd works right now in regards to filesystem access. Files in the folder ```src/initrd/files``` are put into the initrd. C source files in ```src/initrd``` are compiled with libgpio, placing the program in ```src/initrd/files```.  
  
What's next for stmos?  
* SD card support
* Display support (ILI....)
* Script program support (a shell like sash, maybe lua)
* More ELF work (better execve, signals?, etc.)
