#ifndef SHELL_H_
#define SHELL_H_

#include <stddef.h>
#include <stdint.h>

void clear_screen(void);
void handle_char(char c);
void show_prompt(void);

#endif /* SHELL_H_ */
