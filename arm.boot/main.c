#include "main.h"
#include "kprintf.h"
#include "shell.h"
#include "vic.h"

/**
 * This is the C entry point, upcalled once the hardware has been setup properly
 * in assembly language, see the reset.s file.
 */

static int dummy1;
static int dummy2 = 0;
static int dummy3 = 13;

void _start() {
	int i = 0;
	int count = 0;
	uart_send_string(UART0, "\nQuit with \"C-a c\" and then type in \"quit\".\n");
	uart_send_string(UART0, "\nHello world!\n");
	for (int k = 0; k < 500000000; k++);
	init_shell();

	while (1) {
    	unsigned char c;
		while (0 == uart_receive(UART0, &c)) {
			// friendly reminder that you are polling and therefore spinning...
			// not good for the planet! But until we introduce interrupts,
			// there is nothing you can do about it... except comment out
			// this annoying code ;-)
			count++;
			if (count > 10000000) {
				count = 0;
		 	}
		}
		handle_char(c);
	}
	wfi();
}
