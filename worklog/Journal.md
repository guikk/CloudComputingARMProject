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

Compiler error on main.c: undefined reference to `kprintf`

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

## October 24th

- Most of the imports do not work, string and memory methods are not recognized and labeled with "undefined"

- Learned that I can't just import standard C libraries, because they have to be available in the toolkit instead of my operating system.

## October 27th

- Imported some `<string.h>` functions to be able to compare strings

- Now the parsing works and the reset and echo methods too

- Added week 2 base source

> *Fall Break*

## November 13th

 - Went through the new provided files
 - Add call to `wfi` on main program

## November 14th

- Fix bug on reading escape sequences
- Refactoring on shell code

- Call to wfi wasn't being reached, fixed
- Added irqs.o vic.o uart-irqs.o cb.o as compiled target objects

Compilation error:
~~~
cc  -g   -c -o irqs.o irqs.S
irqs.S: Assembler messages:
irqs.S:29: Error: no such instruction: `mrs r0,cpsr'
irqs.S:31: Error: no such instruction: `bic r1,r0,'
irqs.S:32: Error: no such instruction: `orr r1,r1,'
irqs.S:33: Error: no such instruction: `msr cpsr,r1'
irqs.S:35: Error: no such instruction: `ldr sp,=irq_stack_top'
irqs.S:39: Error: no such instruction: `msr cpsr,r0'
irqs.S:40: Error: too many memory references for `mov'
irqs.S:51: Error: no such instruction: `mrs r0,cpsr'
irqs.S:56: Error: no such instruction: `bic r0,r0,'
irqs.S:58: Error: no such instruction: `msr cpsr,r0'
irqs.S:59: Error: too many memory references for `mov'
irqs.S:74: Error: no such instruction: `mrs r0,cpsr'
irqs.S:76: Error: no such instruction: `orr r0,r0,'
irqs.S:78: Error: no such instruction: `msr cpsr,r0'
irqs.S:79: Error: too many memory references for `mov'
make: *** [<builtin>: irqs.o] Error 1
~~~

## November 15th

- Searched for similar errors on online forums

[Assembler Messages: no such instruction when Compiling C++](https://stackoverflow.com/questions/38403611/assembler-messages-no-such-instruction-when-compiling-c)

[Error on make: no such instruction](https://embdev.net/topic/129526)

- Hypotheses:
    - [Not the case] The toolchain assembler is not up to date 
    - There's an error on the toolchain installation (The instructions are being fetched in the wrong place)
    - The system's architecture does not support ?b instructions 

- Updating the toolchain did not solve the problem

Compiling with the verbose flag shows the following lines:
~~~
gcc version 9.4.0 (Ubuntu 9.4.0-1ubuntu1~20.04.1)
COLLECT_GCC_OPTIONS='-g' '-v' '-c' '-o' 'irqs.o' '-mtune=generic' '-march=x86-64'
 /usr/lib/gcc/x86_64-linux-gnu/9/cc1 -E -lang-asm -quiet -v -imultiarch x86_64-linux-gnu irqs.S -mtune=generic -march=x86-64 -g -fworking-directory -fasynchronous-unwind-tables -fstack-protector-strong -Wformat -Wformat-security -fstack-clash-protection -fcf-protection -fno-directives-only -o /tmp/ccTki5V5.s
ignoring nonexistent directory "/usr/local/include/x86_64-linux-gnu"
ignoring nonexistent directory "/usr/lib/gcc/x86_64-linux-gnu/9/include-fixed"
ignoring nonexistent directory "/usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include"
#include "..." search starts here:
#include <...> search starts here:
 /usr/lib/gcc/x86_64-linux-gnu/9/include
 /usr/local/include
 /usr/include/x86_64-linux-gnu
 /usr/include
~~~

This error makes it impossible to advance on the 2nd part of the project.

## November 16th

- Revisiting Part1 of the project, to refactor the code and tie up loose ends
- Started and almost ended implementing the buffer history
- Still haven't found a way to continue Part2

## November 17th

- Finished implementing history
- Implemented editing mid-line with delete/backspace and adding characters