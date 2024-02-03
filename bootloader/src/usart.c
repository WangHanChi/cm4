#include "usart.h"

static volatile struct {
    uint16_t tri, twi, tct;
    uint16_t rri, rwi, rct;
    uint8_t tbuf[USART3_TXB];
    uint8_t rbuf[USART3_RXB];
} Fifo3;

/* this function will overwrite the default in startup.c */
void USART3_IRQHandler(void)
{
    int i;

    if (USART3->SR & USART_SR_RXNE) {
        i = Fifo3.rct;
        if (i < USART3_RXB) {
            Fifo3.rct = ++i;
            i = Fifo3.rwi;
            Fifo3.rbuf[i] = USART3->DR;
            Fifo3.rwi = ++i % USART3_RXB;
        }
    }
    if (USART3->SR & USART_SR_TXE) {
        i = Fifo3.tct;
        if (i--) {
            Fifo3.tct = (uint16_t) i;
            i = Fifo3.tri;
            USART3->DR = Fifo3.tbuf[i];
            Fifo3.tri = ++i % USART3_TXB;
        } else {
            USART3->CR1 &= ~(USART_CR1_TXEIE);
        }
    }
}

int usart3_test(void)
{
    return Fifo3.rct;
}

uint8_t usart3_getc(void)
{
    uint8_t d;
    int i;

    /* Wait while Rx fifo is empty */
    while (!Fifo3.rct)
        ;

    i = Fifo3.rri; /* Get a byte from Rx fifo */
    d = Fifo3.rbuf[i];
    Fifo3.rri = ++i % USART3_RXB;
    __disable_irq();
    Fifo3.rct--;
    __enable_irq();

    return d;
}

void usart3_putc(uint8_t d)
{
    int i;

    /* Wait for Tx firo ready */
    while (Fifo3.tct >= USART3_TXB)
        ;

    i = Fifo3.twi; /* Put a byte into Tx byffer */
    Fifo3.tbuf[i] = d;
    Fifo3.twi = ++i % USART3_TXB;
    __disable_irq();
    Fifo3.tct++;
    USART3->CR1 |= USART_CR1_TXEIE;
    __enable_irq();
}

void usart3_init(uint32_t bps)
{
    NVIC_DisableIRQ(USART3_IRQn);

    /* Attach USART module to I/O pads */
    RCC->AHB1ENR |= (RCC_AHB1ENR_GPIODEN);
    RCC->APB1ENR |= (RCC_APB1ENR_USART3EN);

    /* USART3 Configuration, Rx->PD8, Tx->PD9 */
    GPIOD->MODER |= (GPIO_MODER_MODER8_1);
    GPIOD->MODER |= (GPIO_MODER_MODER9_1);
    GPIOD->AFR[1] |=
        GPIO_AFRH_AFSEL8_0 | GPIO_AFRH_AFSEL8_1 | GPIO_AFRH_AFSEL8_2;
    GPIOD->AFR[1] |=
        GPIO_AFRH_AFSEL9_0 | GPIO_AFRH_AFSEL9_1 | GPIO_AFRH_AFSEL9_2;

    USART3->CR1 |= (USART_CR1_RE);
    USART3->CR1 |= (USART_CR1_TE);

    /* enable usart3 interrupt */
    USART3->CR1 |= (USART_CR1_RXNEIE);

    USART3->CR1 |= (USART_CR1_UE);
    USART3->BRR = DEFAULT_F_CLK / BAUDRATE;

    /* Clear Tx/Rx fifo */
    Fifo3.tri = 0;
    Fifo3.twi = 0;
    Fifo3.tct = 0;
    Fifo3.rri = 0;
    Fifo3.rwi = 0;
    Fifo3.rct = 0;

    NVIC_EnableIRQ(USART3_IRQn);
}

void usart3_reset(void)
{
    RCC->APB1ENR &= ~(RCC_APB1ENR_USART3EN);
}
