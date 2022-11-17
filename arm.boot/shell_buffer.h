#ifndef SHELL_BUFFER_H_
#define SHELL_BUFFER_H_

// Constants
#define BUFFER_CAPACITY 70
#define HISTORY_CAPACITY 20

// Buffer
unsigned char* get_buffer(void);
void clear_buffer(void);
int is_buffer_empty(void);
int is_buffer_full(void);
void buffer_add(unsigned char c);
void buffer_remove_left(void);
void buffer_remove_right(void);
unsigned char buffer_peek(void);
void reset_buffer_offset(void);
int is_buffer_offset_max(void);
int is_buffer_offset_min(void);
void inc_buffer_offset(void);
void dec_buffer_offset(void);

// History
void fetch_history_older(void);
void fetch_history_newer(void);
void history_add_buffer(void);

#endif /* SHELL_BUFFER_H_ */