#include "shell.h"
#include "kprintf.h"
#include "board.h"
#include <unistd.h>
#include "string.h"

#define CONSOLE_HEIGHT 40
#define CONSOLE_WIDTH 80
#define BUFFER_CAPACITY 70
#define HISTORY_LEN 20

#define BACKSPACE 8
#define LF 10
#define CR 13
#define ESC 27
#define SPACE 32
#define MIN_READABLE_ASCII 32
#define MAX_READABLE_ASCII 126
#define DEL 127

unsigned char buffer[BUFFER_CAPACITY];
static int buffer_len = 0;
// unsigned char buffer_history[HISTORY_LEN][BUFFER_CAPACITY];
// static int history_start = 0;
// static int history_end = 0;


void show_prompt(void) {
    uart_send_string(UART0, "cloudshell> ");
}

void clear_screen(void) {
    uart_send(UART0, ESC);
    uart_send(UART0, '[');
    uart_send(UART0, '2');
    uart_send(UART0, 'J');
}

void cursor_to_home(void) {    
    uart_send(UART0, ESC);
    uart_send(UART0, '[');
    uart_send(UART0, 'H');
}

void save_cursor(void) {
    uart_send(UART0, ESC);
    uart_send(UART0, '7');

}

void init_shell(void) {
	clear_screen();
    cursor_to_home();
    show_prompt();
    save_cursor();
}

void load_cursor(void) {
    uart_send(UART0, ESC);
    uart_send(UART0, '8');
}

void cursor_up(void) {
    uart_send(UART0, ESC);
    uart_send(UART0, '[');
    uart_send(UART0, 'A');
}

void cursor_down(void) {
    uart_send(UART0, ESC);
    uart_send(UART0, '[');
    uart_send(UART0, 'B');
}

void cursor_right(void) {
    uart_send(UART0, ESC);
    uart_send(UART0, '[');
    uart_send(UART0, 'C');
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
    // TODO: Verify if there isn't a way to implement by callbacks
    uart_send(UART0, ESC);
    // Read ESC

    unsigned char c;
    uart_receive(UART0, &c);
    uart_send(UART0, c);
    if (c != '[') return;
    // Read [

    uart_receive(UART0, &c);
    switch (c) {
        case 'A':
            // ARROW UP
            kprintf("ARROW UP - ESC[A\n"); 
            uart_send(UART0, c);
            break;
        case 'B':
            // ARROW DOWN
            kprintf("ARROW DOWN - ESC[B\n");
            uart_send(UART0, c);
            break;
        case 'C':
            // ARROW RIGHT
            kprintf("ARROW RIGHT - ESC[C\n");
            uart_send(UART0, c);
            break;
        case 'D':
            // ARROW LEFT
            kprintf("ARROW LEFT - ESC[D\n");
            uart_send(UART0, c);
            break;
        case '3':
            uart_send(UART0, c);
            uart_receive(UART0, &c);
            if (c != '3') break;
            // DELETE
            kprintf("DELETE - ESC[3~\n");
            uart_send(UART0, c);
            break;
    }
}

void parse() {
    // reset
    if (strcmp(buffer, "reset") == 0) {
        clear_screen();
        cursor_to_home();
        show_prompt();
    } 
    // echo
    else if (strncmp(buffer, "echo ", 5) == 0) {
        uart_send(UART0, LF);
        uart_send_string(UART0, buffer+5);
        uart_send(UART0, LF);
        show_prompt();
    } 
    // anything else
    else {
        uart_send(UART0, LF);
        show_prompt();
    }
}

void handle_char(char c) {
    if (c == ESC) {
        read_escape_sequence();
        return;
    }

    load_cursor();

    switch(c) {
        case CR:
            parse(&buffer);
            // clear buffer
            while (buffer_len > 0) {
                buffer_len--; 
                buffer[buffer_len] = 0;
            }
            break;

        case MIN_READABLE_ASCII ... MAX_READABLE_ASCII:
            if (buffer_len < BUFFER_CAPACITY) {
                buffer[buffer_len] = c;
                buffer_len++;
                uart_send(UART0, c);
            }
            break;

        case DEL:
            if (buffer_len > 0) {
                buffer_len--;
                buffer[buffer_len] = 0;
                backspace();
            }
            break;
    }

    save_cursor();

    // Log read ASCII value
    kprintf("ASCII(%d)\t", c);
    kprintf("Buffer= %s\n", buffer);
}
