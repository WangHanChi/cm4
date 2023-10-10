#pragma once

#include "reg.h"

/* USART TXE Flag
 * This flag is cleared when data is written to USARTx_DR and
 * set when that data is transferred to the TDR
 */
#define USART_FLAG_TXE ((uint16_t) 0x0080)

/* USART RXNE Flag
 * This flag is set to 1 means "Received data is ready to be read."
 * Set to 0 means "Data is not received"
 */
#define USART_FLAG_RXNE ((uint16_t) 0x0020)

extern uint32_t DEFAULT_F_CLK;
extern uint32_t BAUDRATE;

void print_char(char c);
void print_str(const char *str);
void scan_str(char *str);
void usart_init(void);