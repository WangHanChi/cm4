#pragma once

#include "stm32f429xx.h"

extern uint32_t DEFAULT_F_CLK;
extern uint32_t BAUDRATE;

#define USART3_RXB	128		/* Size of Rx buffer */
#define USART3_TXB	128		/* Size of Tx buffer */

#define DELAY(x)                            \
    do {                                    \
        for(int i = 0 ; i < 5000*x; i++)    \
            ;                               \
    } while(0)

void usart3_init (uint32_t bps);
int usart3_test (void);
void usart3_putc (uint8_t);
uint8_t usart3_getc (void);