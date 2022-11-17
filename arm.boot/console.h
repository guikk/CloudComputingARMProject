#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <stdint.h>

// ASCII Codes
#define BACKSPACE 8
#define LF 10
#define CR 13
#define ESC 27
#define SPACE 32
#define MIN_READABLE_ASCII 32
#define MAX_READABLE_ASCII 126
#define DEL 127

// Constants
#define CONSOLE_HEIGHT 40
#define CONSOLE_WIDTH 80

// Send input
void send_char(unsigned char c);
void send_chars(int n, ...);
void send_string(const unsigned char *s);

// Console actions
void clear_screen(void);
void clear_line(void);
void cursor_to_home(void);
void save_cursor(void);
void load_cursor(void);
void move_cursor_up(void);
void move_cursor_down(void);
void move_cursor_right(void);
void move_cursor_left(void);
void backspace(void);

#endif /* CONSOLE_H_ */