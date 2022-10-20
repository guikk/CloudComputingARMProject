#include "shell.h"
#include "kprintf.h"
#include "board.h"

#define CONSOLE_HEIGHT 40
#define CONSOLE_WIDTH 80

#define CR 13
#define BACKSPACE 8

void cursor_up(void) {
    uart_send(UART0, 27);
    uart_send(UART0, 91);
    uart_send(UART0, 65);
}

void cursor_down(void) {
    uart_send(UART0, 27);
    uart_send(UART0, 91);
    uart_send(UART0, 66);
}

void cursor_right(void) {
    uart_send(UART0, 27);
    uart_send(UART0, 91);
    uart_send(UART0, 67);
}

void cursor_left(void) {
    uart_send(UART0, 27);
    uart_send(UART0, 91);
    uart_send(UART0, 68);
}


void handle_char(char c) {
    switch(c) {
        case '\r':
            uart_send(UART0, '\n');
            break;
        default:
            uart_send(UART0, c);
    }

    // Log read ASCII value
    kprintf("ASCII(%d)\n", c);
}

void clear_screen(void) {
    for (int i = 0; i < 2*CONSOLE_HEIGHT; i++) {
        handle_char(CR);
    }

    for (int i = 0; i < CONSOLE_HEIGHT; i++) {
        cursor_up();
    }
}

