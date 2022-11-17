#include "main.h"
#include "cb.h"
#include "uart.h"
#include "board.h"

static struct cb rxcb; 
static struct cb txcb; 

void init_uart(void) {
	cb_init(&rxcb);
	cb_init(&txcb);
}

/**
 * Receive a character from the given uart, this is a non-blocking call.
 * Returns 0 if there are no character available.
 * Returns 1 if a character was read.
 */
int uart_receive(int uart, unsigned char *s) {
/*
 * The function "uart_receive" will pull bytes from a circular 
 * buffer, called "rxcb", that the handler of the RX interrupt
 * will fill with the available bytes from the RX FIFO.
*/
	return cb_get(&rxcb, s) ? 0 : 1;
}

/**
 * Sends a character through the given uart, this is a blocking call.
 * The code spins until there is room in the UART TX FIFO queue to send
 * the character.
 */
void uart_send(int uart, unsigned char s) {
/*
 * The function "uart_send" will push bytes in another circula buffer,
 * called "txcb", that the handler of the TX interrupt will 
 * empty, writing the bytes to the TX FIFO, when there is room to do so.
 */
	cb_put(&txcb, s);
}

/**
 * This is a wrapper function, provided for simplicity,
 * it sends a C string through the given uart.
 */
void uart_send_string(int uart, const unsigned char *s) {
	while (*s != '\0') {
		uart_send(uart, *s);
		s++;
	}
}

void rx_callback(void) {
    // unsigned char c;
    // int res = uart_receive(UART0, &c);
    // while(res!=0 && !cb_full(rx_cb)){
    //     cb_put(rx_cb, c);
    //     res = uart_receive(UART0, &c);
    // }

	int uart = UART0;
	unsigned char *s;

    unsigned short* uart_fr = (unsigned short*) (uart + UART_FR);
    unsigned short* uart_dr = (unsigned short*) (uart + UART_DR);
    while (!(*uart_fr & UART_RXFE) && !cb_full(&rxcb)) {
		*s = (*uart_dr & 0xff);
		cb_put(&rxcb, *s);
	}
}

void tx_callback(void) {
    // unsigned char c;
    // unsigned short* uart_fr = (unsigned short*) (UART0+UART_FR);
    // while(!(*uart_fr & UART_TXFF) && !cb_empty(tx_cb)){
    //     cb_get(tx_cb, &c);
    //     uart_send(UART0, c);
    // }
	
	int uart = UART0;
	unsigned char s;

    unsigned short* uart_fr = (unsigned short*) (uart + UART_FR);
    unsigned short* uart_dr = (unsigned short*) (uart + UART_DR);
    while (!cb_empty(&txcb) && !(*uart_fr & UART_TXFF)) {
		cb_get(&txcb, &s);
		*uart_dr = s;

	}
}

void rt_callback(void) {
	int uart = UART0;
	unsigned char *s;

    unsigned short* uart_fr = (unsigned short*) (uart + UART_FR);
    unsigned short* uart_dr = (unsigned short*) (uart + UART_DR);
    while (!(*uart_fr & UART_RXFE) && !cb_full(&rxcb)) {
		*s = (*uart_dr & 0xff);
		cb_put(&rxcb, *s);
	}
}
