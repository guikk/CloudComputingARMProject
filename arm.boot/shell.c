#include "shell.h"
#include "kprintf.h"
#include "board.h"
#include "console.h"
#include <unistd.h>
#include "string.h"

// Constants
#define BUFFER_CAPACITY 70
// #define HISTORY_LEN 20

unsigned char buffer[BUFFER_CAPACITY];
static int buffer_len = 0;
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

void init_shell(void);
void show_prompt(void);

void parse_buffer(void);
void handle_char(char c);
void read_char(char c);
void read_escape_sequence(char c);

void show_prompt(void) {
    send_string("cloudshell> ");
}

void init_shell(void) {
	clear_screen();
    cursor_to_home();
    show_prompt();
    save_cursor();
}

void clear_buffer(void) {
    while (buffer_len > 0) {
        buffer_len--; 
        buffer[buffer_len] = 0;
    }
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
        send_char(LF);
        send_string(buffer+5);
        send_char(LF);
        show_prompt();
    } 
    // anything else
    else {
        send_char(LF);
        show_prompt();
    }
}

void handle_char(char c) {
    if (read_state != IDLE) {
        read_escape_sequence(c);
        return;
    }

    read_char(c);
}

void read_char(char c) {

    switch(c) {
        case BACKSPACE: // When pressed Ctrl + Backspace
            load_cursor();
            while (buffer_len > 0) {
                buffer_len--;
                buffer[buffer_len] = 0;
                backspace();
                if (buffer[buffer_len-1] == SPACE) break;
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
                send_char(c);
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
