#include "console.h"
#include "board.h"
#include <stdarg.h>

// Send input
void send_char(unsigned char c) {
    uart_send(UART0, c);
}

void send_chars(int n, ...) {
    va_list args;
    va_start(args, n);
 
    for (int i=0; i < n; i++) {
        unsigned char c = va_arg(args, int);
        uart_send(UART0, c);
    }
    va_end(args);
}

void send_string(const unsigned char *s) {
    uart_send_string(UART0, s);
}

// Console actions
void clear_screen(void) {
    send_chars(4, ESC,'[','2','J');
}

void clear_line(void) {
    send_chars(4, ESC,'[','2','K');
}

void cursor_to_home(void) {    
    send_chars(3, ESC,'[','H');
}

void save_cursor(void) {
    send_chars(2, ESC,'7');
}

void load_cursor(void) {
    send_chars(2, ESC,'8');
}

void move_cursor_up(void) {
    send_chars(3, ESC,'[','A');
}

void move_cursor_down(void) {
    send_chars(3, ESC,'[','B');
}

void move_cursor_right(void) {
    send_chars(3, ESC,'[','C');
}

void move_cursor_left(void) {
    send_chars(3, ESC,'[','D');
}

void backspace(void) {
    send_chars(3, BACKSPACE, SPACE, BACKSPACE);
}