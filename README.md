# Cloud Computing Shell Project

## Branches
There were 3 branches in the repository throughout the project:

- `master`: Where is the most updated finished work.
- `week1`: Where I committed changes from the first part of the project, but after finished it was merged to master.
- `week2`: Where I committed changes from the second part of the project, and since it wasn't completely finished, it is still separate from master.

## Tags

The git tags used were `week1.step1`, `week1.step2`, `week1.step3`,`week1.step4` and `week2`. There are unfortunately some untagged commits, which I forgot to add the corresponding tag to.

## How to compile/run the code

### Pre-requisites
- Install the `qemu-system-arm` package from your package manager.
- Install the arm toolchain:
    - Download from the [ARM website](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads?fbclid=IwAR1-3kYtKPIBNgKY82lsbsjt5SSmDMKNsyb45Y_9l9BS5imcjMswRVEPHPc) and follow the [installation instructions](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads?fbclid=IwAR1-3kYtKPIBNgKY82lsbsjt5SSmDMKNsyb45Y_9l9BS5imcjMswRVEPHPc#).
    - Add the directory `/bin/` from the toolchain to the PATH.


### Commands
Inside the `arm.boot` directory, you can run any of these commands:

| Usage | Effect |
| - | - |
| `make` | Compile the project |
| `make clean` | Delete compiled sources |
| `make run` | Compile and run |
| `make debug` | Compile and run on debug mode |
| `telnet localhost 4321` | Access log serial line |
| `gdb-multiarch kernel.elf` | Start debugger (use `target remote :1234` on gdb to link program) |

## Shell Features
Explain also what works and what does not and why it does not in your opinion. If the archive is too big, it probable means that you didn't setup the proper .gitignore, something to remember in the future. You can use a file server and send me the URL to download your archive. Pr. Olivier Gruber.

After running the program, the shell prompt will be shown.
You can type an input as you like, then press `ENTER` to send it.
If the input is a supported instruction, the instruction will be processed. Otherwise, the input will be ignored.
After the system reads the input, the prompt will be shown again.

### Instructions
There is support for 2 instructions:
| Intruction | Effect |
| - | - |
|`echo` *word* | Displays *word* to the console. |
| `reset` | Clears the screen and resets the shell. |

### Buffer history
By pressing the `UP` and `DOWN` arrows you can navigate through the last 20 entered inputs.

### Line editting
Like any regular shell, you can navigate through the typed input with the `LEFT` and `RIGHT` arrows, and use `BACKSPACE` or `DELETE` to delete characters to the left and to the right, respectively.

To make it even easier to edit the current line, support to the `Ctrl + BACKSPACE` command was added, to delete all the characters to the left until the last space.

## Work in progress
The project was also intended to deliver interrupt support, but even with the addition of initialization and callbacks to handle the interrupt behaviours, they did not work as expected. The program hangs after the first call to *wait for interrupts* and doesn't receive any further messages. The problem could be in some lacking setup instruction which helped the system handling the input signals.

The work for this part is available in the `week2` branch.

## Journal
Notes from the development progression are available [here](worklog/Journal.md).