## October 11th
Setup for workspace on a Windows 11 / WSL2:
- Installation of QEMU and GDB on WSL2
- Initialization of local git repository

Boards VAP and VPB -> typo on work specification, stand for the two type of machine options to run QEMU, `versatileab` and `versatilepb`

First attempt: Error on Makefile
~~~
arm-none-eabi-as  -g -o vector.o vector.s
make: arm-none-eabi-as: Command not found
make: *** [Makefile:69: vector.o] Error 127
~~~