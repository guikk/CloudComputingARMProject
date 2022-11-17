#include "shell_buffer.h"
#include "string.h"
#include "kprintf.h"

unsigned char buffer[BUFFER_CAPACITY];
static int buffer_len = 0;
static int buffer_index_offset = 0;

unsigned char buffer_history[HISTORY_CAPACITY][BUFFER_CAPACITY];
static int history_last = -1;
static int history_len = 0;
static int history_index = -1;

static void copy_history_to_buffer(void);

// Buffer
unsigned char* get_buffer(void) {
    return buffer;
}

void clear_buffer(void) {
    while (!is_buffer_empty()) {
        buffer_len--; 
        buffer[buffer_len] = 0;
    }
    reset_buffer_offset();
}

int is_buffer_empty(void) {
    return buffer_len == 0;
}

int is_buffer_full(void) {
    return buffer_len == BUFFER_CAPACITY;
}

void buffer_add(unsigned char c) {
    if (!is_buffer_offset_max()) {
        memmove(
            (void *) &buffer[buffer_len + buffer_index_offset + 1],
            (void *) &buffer[buffer_len + buffer_index_offset],
            -1 * buffer_index_offset
        );
    }
    buffer[buffer_len + buffer_index_offset] = c;
    buffer_len++;
}

void buffer_remove_left(void) {
    if (!is_buffer_offset_max()) {
        memmove(
            (void *) &buffer[buffer_len + buffer_index_offset - 1],
            (void *) &buffer[buffer_len + buffer_index_offset],
            -1 * buffer_index_offset
        );
    }
    buffer_len--;
    buffer[buffer_len] = 0;
}

void buffer_remove_right(void) {
    memmove(
        (void *) &buffer[buffer_len + buffer_index_offset],
        (void *) &buffer[buffer_len + buffer_index_offset + 1],
        -1 * buffer_index_offset
    );
    buffer_len--;
    buffer[buffer_len] = 0;
    inc_buffer_offset();
}

unsigned char buffer_peek(void) {
    return buffer[buffer_len - 1 + buffer_index_offset];
}

void reset_buffer_offset(void) {
    buffer_index_offset = 0;
    kprintf("New offset= %d ", buffer_index_offset);
}

int is_buffer_offset_max(void) {
    return buffer_index_offset >= 0;
}

int is_buffer_offset_min(void) {
    return buffer_len + buffer_index_offset <= 0;
}

void inc_buffer_offset(void) {
    buffer_index_offset++;
    kprintf("New offset= %d ", buffer_index_offset);
}

void dec_buffer_offset(void) {
    buffer_index_offset--;
    kprintf("New offset= %d ", buffer_index_offset);
}

// History
static void copy_history_to_buffer(void) {
    // Get #history_index last history entry
    int i = (history_last + HISTORY_CAPACITY - history_index) % HISTORY_CAPACITY;
    strcpy(buffer, buffer_history[i]);
    buffer_len = strlen(buffer);
    reset_buffer_offset();
}

void fetch_history_older(void) {
    if (history_index < history_len-1) {
        history_index++;
        copy_history_to_buffer();
        kprintf("\n\tFetching history %d ", history_index);
        kprintf("-> %s\n", buffer);
    }
}

void fetch_history_newer(void) {
    if (history_index > 0) {
        history_index--;
        copy_history_to_buffer();
        kprintf("\n\tFetching history %d ", history_index);
        kprintf("-> %s\n", buffer);
    } else if (history_index > -1) {
        history_index--;
        clear_buffer();
    }
}

void history_add_buffer(void) {
    if (history_len < HISTORY_CAPACITY)
        history_len++;
    history_last = (history_last + 1) % HISTORY_CAPACITY;
    strcpy(buffer_history[history_last], buffer);
    history_index = -1;
}