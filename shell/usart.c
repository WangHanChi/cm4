#include "usart.h"
#include "reg.h"

void print_char(char c)
{
    while (!(*USART3_SR & USART_FLAG_TXE))
        ;
    *USART3_DR = c;
    while (!(*USART3_SR & 0x40))
        ;
}

/* use print_char to warpper puts */
void print_str(const char *str)
{
    while (*str) {
        print_char(*str);
        if (*str == '\n')
            print_char('\r');
        str++;
    }
}

/* scan the stdin for user keyin */
void scan_str(char *str)
{
    char received_char;
    int index = 0;

    /* Clear input buffer */
    while ((*USART3_SR & USART_FLAG_RXNE)) {
        received_char = *USART3_DR & 0xFF;
    }

    while (1) {
        /* Wait until a character is received (USART_RXNE flag is set) */
        while (!(*USART3_SR & USART_FLAG_RXNE))
            ;

        /* Read the received character from USART3_DR */
        received_char = *USART3_DR & 0xFF;

        print_char(received_char);

        /* Clear RXNE by set 0 */
        *USART3_SR &= ~(USART_FLAG_RXNE);

        /* Check if it is the end of line (Enter key) */
        if (received_char == '\r') {
            /* Null-terminate the string and exit the loop */
            str[index] = '\0';
            print_char('\n');
            print_char('\r');
            break;
        }

        /* Store the received character in the buffer */
        str[index] = received_char;

        /* Increment the index for the next character */
        index++;
    }
}

void usart_init(void)
{
    *(RCC_AHB1ENR) |= (uint32_t) (0x01 << 3);
    *(RCC_APB1ENR) |= (uint32_t) (0x01 << 18);

    /* USART3 Configuration, Rx->PD8, Tx->PD9 */
    *(GPIOD) &= (uint32_t) ~(0x0F << 16);
    *(GPIOD) |= (uint32_t) (0x02 << 16);
    *(GPIOD) |= (uint32_t) (0x02 << 18);
    *(GPIOD_AFRH) &= (uint32_t) ~(0xFF << 0);
    *(GPIOD_AFRH) |= (uint32_t) (0x77 << 0);

    *(USART3_CR1) |= (0x01 << 2);
    *(USART3_CR1) |= (0x01 << 3);
    *(USART3_CR1) |= (0x01 << 13);
    *(USART3_BRR) = DEFAULT_F_CLK / BAUDRATE;
}