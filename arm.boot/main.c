#include "main.h"
#include "kprintf.h"
#include "shell.h"
#include "vic.h"
#include "interrupts.h"

/**
 * This is the C entry point, upcalled once the hardware has been setup properly
 * in assembly language, see the reset.s file.
 */

void _start() {
	init_uart();

	init_interrupts();

	uart_send_string(UART0, "\nQuit with \"Ctrl+A X\".\n");

	init_shell();

	while (1) {
    	unsigned char c;
		// wait for char
		while (0 == uart_receive(UART0, &c)) {
			wfi();
		}
		handle_char(c);
	}
}
