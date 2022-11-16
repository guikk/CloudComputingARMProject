#include "shell.h"
#include "kprintf.h"
#include "board.h"
#include <unistd.h>
#include "string.h"

// Constants
#define CONSOLE_HEIGHT 40
#define CONSOLE_WIDTH 80
#define BUFFER_CAPACITY 70
// #define HISTORY_LEN 20

// ASCII Codes
#define BACKSPACE 8
#define LF 10
#define CR 13
#define ESC 27
#define SPACE 32
#define MIN_READABLE_ASCII 32
#define MAX_READABLE_ASCII 126
#define DEL 127

// TODO: use circular buffer on cb.h
unsigned char buffer[BUFFER_CAPACITY];
static int buffer_len = 0;

void clear_buffer(void) {
    while (buffer_len > 0) {
        buffer_len--; 
        buffer[buffer_len] = 0;
    }
}

typedef enum
{
    IDLE,
    ESC1,
    ESC2,
    ESC3
} ReadState;
static ReadState read_state = IDLE;

// TODO: implement history with a circular buffer
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
    uart_send(UART0, ESC);
    uart_send(UART0, '[');
    uart_send(UART0, 'D');
}

void backspace(void) {
    uart_send(UART0, BACKSPACE);
    uart_send(UART0, SPACE);
    uart_send(UART0, BACKSPACE);
}

void parse_buffer(void) {
    // reset
    if (strcmp(buffer, "reset") == 0) {
        clear_screen();
        cursor_to_home();
        show_prompt();
    } 
    // echo
    else if (strncmp(buffer, "echo ", 5) == 0) {
        load_cursor();
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

void read_char(char c) {

    switch(c) {
        case BACKSPACE: // When pressed Ctrl + Backspace
            load_cursor();
            while (buffer_len > 0) {
                buffer_len--;
                buffer[buffer_len] = 0;
                backspace();
                if (buffer[buffer_len-1] == SPACE)
            }
            break;
        case CR:
            parse_buffer();
            clear_buffer();
            
            break;

        case ESC:
            read_state = ESC1;
            return;

        case MIN_READABLE_ASCII ... MAX_READABLE_ASCII:
            load_cursor();
            if (buffer_len < BUFFER_CAPACITY) {
                buffer[buffer_len] = c;
                buffer_len++;
                uart_send(UART0, c);
            }
            break;

        case DEL: // When pressed Backspace
            load_cursor();
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

void read_escape_sequence(char c) {
    // TODO: Verify if there isn't a way to implement by callbacks

    // Escape sequence 1st byte
    if (read_state == ESC1) {
        switch(c) {
            case '[': 
                read_state = ESC2;
                break;
            default:
                read_state = IDLE;
                read_char(c);
        }
        return;
    }

    // Escape sequence 2nd byte
    if (read_state == ESC2) {
        switch(c) {
            case 'A':
                // ARROW UP
                // TODO: bind escape sequence with looking up in command history
                kprintf("ARROW UP - ESC[A\n");
                cursor_up();
                break;
            case 'B':
                // ARROW DOWN
                // TODO: bind escape sequence with looking down in command history
                kprintf("ARROW DOWN - ESC[B\n");
                cursor_down();
                break;
            case 'C':
                // ARROW RIGHT
                // TODO: bind escape sequence with editing line buffer
                kprintf("ARROW RIGHT - ESC[C\n");
                cursor_right();
                break;
            case 'D':
                // ARROW LEFT
                // TODO: bind escape sequence with editing line buffer
                kprintf("ARROW LEFT - ESC[D\n");
                cursor_left();
                break;
            case '3':
                read_state = ESC3;
                break;
            default:
                read_state = IDLE;
                read_char(c);
        }
        return;
    }

    // Escape sequence 3rd byte
    if (read_state == ESC3) {
        switch(c) {
            case '~': 
                // DELETE
                // TODO: bind escape sequence with editing line buffer
                kprintf("DELETE - ESC[3~\n");
                read_state = IDLE;
                break;
            default:
                read_state = IDLE;
                read_char(c);
        }
        return;
    }
}

void handle_char(char c) {
    if (read_state != IDLE) {
        read_escape_sequence(c);
        return;
    }

    read_char(c);
}
