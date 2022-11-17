#include "shell.h"
#include "kprintf.h"
#include "board.h"
#include "console.h"
#include "shell_buffer.h"
#include <unistd.h>
#include "string.h"

typedef enum
{
    IDLE,
    ESC1,
    ESC2,
    ESC3
} ReadState;
static ReadState read_state = IDLE;

static void show_prompt(void);
static void parse_buffer(void);
static void refresh_buffer(void);
static void read_char(char c);
static void read_escape_sequence(char c);

void init_shell(void) {
	clear_screen();
    cursor_to_home();
    show_prompt();
    save_cursor();
}

static void show_prompt(void) {
    send_string("cloudshell> ");
}

static void refresh_buffer() {
    clear_line();
    send_char(CR);
    show_prompt();
    send_string(get_buffer());
}

static void parse_buffer(void) {
    unsigned char* buffer = get_buffer();
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

static void read_char(char c) {
    // Log read ASCII value
    kprintf("ASCII(%d)\t", c);

    switch(c) {
        case BACKSPACE: // When pressed Ctrl + Backspace
            while (!is_buffer_empty() && !is_buffer_offset_min()) {
                buffer_remove_left();
                backspace();
                if (!is_buffer_offset_min() && buffer_peek() == SPACE)
                    break;
            }
            save_cursor();
            refresh_buffer();
            load_cursor();
            break;
        case CR:
            parse_buffer();
            if (!is_buffer_empty()) {
                history_add_buffer();
                clear_buffer();
            }
            
            break;

        case ESC:
            read_state = ESC1;
            return;

        case MIN_READABLE_ASCII ... MAX_READABLE_ASCII:
            if (!is_buffer_full()) {
                buffer_add(c);
                save_cursor();
                refresh_buffer();
                load_cursor();
                send_char(c);
            }
            break;

        case DEL: // When pressed Backspace
            if (!is_buffer_empty() && !is_buffer_offset_min()) {
                buffer_remove_left();
                backspace();
                save_cursor();
                refresh_buffer();
                load_cursor();
            }
            break;
    }

    save_cursor();

    kprintf("Buffer= %s\n", get_buffer());
}

static void read_escape_sequence(char c) {
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
                kprintf("ARROW UP - ESC[A ");
                fetch_history_older();
                refresh_buffer();
                read_state = IDLE;
                break;
            case 'B':
                // ARROW DOWN
                kprintf("ARROW DOWN - ESC[B ");
                fetch_history_newer();
                refresh_buffer();
                read_state = IDLE;
                break;
            case 'C':
                // ARROW RIGHT
                kprintf("ARROW RIGHT - ESC[C ");
                if (!is_buffer_offset_max()) {
                    inc_buffer_offset();
                    move_cursor_right();
                }
                kprintf("\n");
                read_state = IDLE;
                break;
            case 'D':
                // ARROW LEFT
                kprintf("ARROW LEFT - ESC[D ");
                if (!is_buffer_offset_min()) {
                    dec_buffer_offset();
                    move_cursor_left();
                }
                kprintf("\n");
                read_state = IDLE;
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
                kprintf("DELETE - ESC[3~\t");
                if (!is_buffer_offset_max()) {
                    buffer_remove_right();
                    save_cursor();
                    refresh_buffer();
                    load_cursor();
                }
                kprintf("Buffer= %s\n", get_buffer());
                read_state = IDLE;
                break;
            default:
                read_state = IDLE;
                read_char(c);
        }
        return;
    }
}
