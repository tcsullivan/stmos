# The arm-stmos toolchain

A special binutils/gcc toolchain is necessary to build stmos and associated
programs. For convenience, a bash script has been written to attempt to automate
the majority of the compilation/installation process. This process is based of
off various tutorials from [osdev.org](https://wiki.osdev.org). 
  
Some programs are required for the build process. A list can be found [here](https://wiki.osdev.org/GCC_Cross-Compiler#Installing_Dependencies). Once
installed, just run the script:  
```
./create-toolchain.sh
```  
It should be safe to re-run the script, if necessary.  
  
After successful installation, be sure to add the toolchain to your path ($HOME
/arm-stmos/bin). Then, follow these steps to build pdclib and the crt0: 
* Cd to src/pdclib
* Make sure the Makefile does not have the ```-g``` flag set under CFLAGS
* Build pdclib (make)
* Copy pdclib.a to ~/arm-stmos/lib/gcc/arm-stmos/8.2.0/libc.a
* Add the ```-g``` flag to CFLAGS in the Makefile; Rebuild pdclib
* Copy pdclib.a to ~/arm-stmos/lib/gcc/arm-stmos/8.2.0/libg.a
* Cd to the stmos root directory; run ```make crt```
* Copy crt0.o ~/arm-stmos/lib/gcc/arm-stmos/8.2.0/crt0.o
  
Now everything should be ready. Build stmos, run it, hope it works.

