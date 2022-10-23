#include "shell.h"
#include "kprintf.h"
#include "board.h"
#include <unistd.h>

#define CONSOLE_HEIGHT 40
#define CONSOLE_WIDTH 80
#define BUFFER_SIZE 50

#define BACKSPACE 8
#define LF 10
#define CR 13
#define ESC 27
#define SPACE 32
#define MIN_READABLE_ASCII 32
#define MAX_READABLE_ASCII 126
#define DEL 127

// CIRCBUF_DEF(uint8_t, buffer, BUFFER_SIZE);

void cursor_up(void) {
    uart_send(UART0, ESC);
    uart_send(UART0, 91);
    uart_send(UART0, 65);
}

void cursor_down(void) {
    uart_send(UART0, ESC);
    uart_send(UART0, 91);
    uart_send(UART0, 66);
}

void cursor_right(void) {
    uart_send(UART0, ESC);
    uart_send(UART0, 91);
    uart_send(UART0, 67);
}

void cursor_left(void) {
    uart_send(UART0, BACKSPACE);
}

void backspace(void) {
    cursor_left();
    uart_send(UART0, SPACE);
    cursor_left();
}

void read_escape_sequence(void) {
    uart_send(UART0, ESC)
}

void handle_char(char c) {
    switch(c) {
        case CR:
            uart_send(UART0, LF);
            show_prompt();
            break;
        case ESC:
            read_escape_sequence();
            break;
        case DEL:
            backspace();
            break;
        default:
            uart_send(UART0, c);
    }

    // Log read ASCII value
    kprintf("ASCII(%d)\n", c);
}

void clear_screen(void) {
    for (int i = 0; i < 2*CONSOLE_HEIGHT; i++) {
        uart_send(UART0, LF);
    }

    for (int i = 0; i < CONSOLE_HEIGHT; i++) {
        cursor_up();
    }
}

void show_prompt(void) {
    uart_send_string(UART0, "cloudshell> ");
}

