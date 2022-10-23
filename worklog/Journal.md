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

> End of step 1.

## October 17th

- Running the boot sequence with gdb

The periodic "Zzzz" output shows the user that the excecution is active, as the main program runs 10M loops and prints something repeatedly.

The stack top is defined in the end because the stack grows downward in memory. We know that the stack is working properly if function calls are being correctly handled.

If too many function calls were made, there would be a stack overflow. To verify that the program is running safely, you could test if the stack top is greater or equal to the end of the BSS section.

> End of step 2

## October 19th

- Added telnet serial line on address localhost:4321
- Linked kprintf to UART1

Compiler error on main.c: undefined reference to `kprintf'

## October 20th

- Added kprintf.o to Makefile OBJS variable (object file list)
- Log typed characters ASCII code directly to telnet serial line

| Key typed | Console action | ASCII code(s) |
| - | - | - |
| Regular keys | Character appears on console | Character ASCII |
| Arrow keys | Cursor moves to direction | 3 different ASCII codes |
| Backspace | Nothing | 127 |
| Delete | Nothing | 4 different ASCII codes |

When we type regular keys, the expected ASCII code is shown and the character is correctly printed in the console. 
In the other hand, the result to pressing arrow keys is a trigger of 3 different ASCII codes to each arrow, and the cursor moving on the console.
Pressing backspace and delete has no effect to the console.

(?) The behavior of the arrows is expected.

> End of step 3.

- Added a method to clear the console screen

## October 21st

- Added a method for deleting previous character

## October 23rd

- Added a shell prompt

Found a good resource for escape sequences [here](https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797).

- Added a buffer to read the line entered
- Added support to change behavior of escape sequences