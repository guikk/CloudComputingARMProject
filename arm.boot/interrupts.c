#include "uart-irqs.h"
#include "board.h"
#include "vic.h"
#include "uart.h"
#include "uart-irqs.h"
#include <stdint.h>

void interrupts_callback(void* cookie);

void init_interrupts(void) {
    vic_setup();
	// vic_enable();

    void *cookie;
    vic_irq_enable(UART0_IRQ, interrupts_callback, cookie);

	// Enable FIFO queues, both rx-queue and tx-queue.
	uint16_t lcr = *(uint16_t*) (UART0 + CUARTLCR_H);
	lcr |= CUARTLCR_H_FEN;
	*(uint16_t*) (UART0 + CUARTLCR_H) = lcr;

	// Set receive, transmit, and receive timeouts interrupt masks.
	uint16_t imsc = *(uint16_t*) (UART0 + UART_IMSC);
	imsc |= UART_IMSC_RXIM | UART_IMSC_TXIM | UART_IMSC_RTIM;
	*(uint16_t*) (UART0 + UART_IMSC) = imsc;
}

void interrupts_callback(void* cookie) {

    // Filter interrupt triggered
    if (UART0 + UART_MIS & UART_MIS_RXMIS) {
        rx_callback();
    }
    if (UART0 + UART_MIS & UART_MIS_TXMIS) {
        tx_callback();
    }
    if (UART0 + UART_MIS & UART_MIS_RTMIS) {
        rt_callback();
    }

    // Clear interrupts
	uint16_t icr = *(uint16_t*) (UART0 + UART_ICR);
	icr |= UART_ICR_RTIC | UART_ICR_TXIC | UART_ICR_RXIC;
	*(uint16_t*) (UART0 + UART_ICR) = icr;
}