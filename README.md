# stmos

stmos is an attempt to implement a general-purpose operating system on ARM
microcontrollers. The name "stmos" is due to the fact that this was first
implemented on an STM32L476RG MCU (ARM Cortex-M4F), and the operating system
still includes many operations/values that are specific to this processor
(currently limiting the portability of this OS).

Eventually, this OS aims to support at least all Cortex-M MCUs.

Features:  
* Advanced heap that does its best to keep large chunks of memory available
* Round-robin multitasking that keeps tasks in userspace
* A virtual filesystem implementation with stdio/initrd built into the kernel
* Ability to execute ELF files loaded from the filesystem
* A nearly complete C standard library implementation available to loaded ELFs
(using [PDCLib](https://github.com/DevSolar/pdclib))
* libgpio: The first userspace library; makes GPIO available to loaded ELFs
  
Currently supported processors:
* STM32L476RG (board: NUCLEO-L476RG)
  
### building  
To build stmos, you'll need the following programs:  
* The arm-stmos toolchain (see the `arm-stmos` folder)
* openocd
* gdb (with multiarch support, `gdb-multiarch` on Debian)
  
To compile stmos simply run `make` (-j supported). To upload, use `run.sh` to
launch OpenOCD and GDB; then, do `lo` to load stmos, and `c` to begin execution.  
  
### notes
Currently, the only available filesystem driver is for the initrd. Files in the
folder `src/initrd/files` are put into the initrd. C source files in
`src/initrd` are compiled (with libgpio available), placing the programs in
`src/initrd/files`
  
What's next for stmos?  
* Processor independence
* SD card support?
* Display support? (ILI....)
* Support for running a popular program (e.g. a shell like sash, maybe lua)
* Growth of features available to loaded ELFs (better execve, signals?, etc.)

