# Week 1 Work

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

## October 12th
- ARM toolkit installation
- Edit the toolkit path on the Makefile in order to succesfully compile the project
- Add arm toolkit folder to `.gitignore`

## October 14th
- Successful compile and run with VPB board
    - Base memory: 33554432 (B?)
- Successful compile and run with VAB board
    - Base memory: 1048576 (B?)

VPB has roughly 30x more memory, the processors are different, from configuration on Makefile

The BSS section contains uninitialized static data that must have no value, hence it needs to be reset. It is 16-bits aligned to fit in registers.
[Data segment - Wikipedia](https://en.wikipedia.org/wiki/Data_segment)