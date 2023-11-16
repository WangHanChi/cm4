#include <stdint.h>
#include <stdarg.h>
#include "usart.h"
#include "reg.h"
#include "string.h"

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

/* display the integer to screen */
void print_int(int c, int len, int base)
{
    char buf[30];
    switch (base)
    {
    case 10:
        itoa(c, buf, len);
        break;
    case 16:
        itohex(c, sizeof(int), buf);
        break;
    default:
        print_str("No support type in output integer\n");
        break;
    }

    print_str(buf);
}

/* display the float to screen */
void print_float(float f, int len)
{
    char buf[30];
    ftoa(f, buf, 3);
    print_str(buf);
    print_char('\r');
}

/* internal function for print */
int isdigit( int c )
{
    if ( c <= '9' && c >= '0' )
        return 1;
    else
        return 0;
}

/* print something to screen in general format */
void print(const char *format, ...)
{
    const char *temp = format;
    int len;
    int d;
    int c;
    char *s;
    float f;

    va_list arg;
    va_start(arg, format);

    do {
        if(*temp == '%'){
            temp++;
            len = 0;

            if(isdigit(*temp)){
                len = *temp;
                while(isdigit(*temp)){
                    len *= 10;
                    len += *temp;
                    temp++;
                }
            }

            /* format in different types */
            switch (*temp)
            {
            case 'd':
                d = va_arg(arg, int);
                print_int(d, len, 10);
                break;
            case 's':
                s = va_arg(arg, char *);
                print_str(s);
                break;
            case 'c':
                c = (unsigned char)va_arg(arg, int);
                print_char(c);
                break;
            case 'x':
                d = va_arg(arg, int);
                print_int(d, len, 16);
                break;
            case 'f':
                f = va_arg(arg, double);
                print_float(f, len);
                break;
            default:
                print_str("No support type in print\n");
                break;
            }
        } else {
            /* string */
            print_char(*temp);
            if(*temp == '\n')
                print_char('\r');
        }
        temp++;
        
    } while (*temp != '\0');
    
}

/* scan the stdin for user keyin */
int scan_str(char *str)
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

        /* Clear RXNE by set 0 */
        *USART3_SR &= ~(USART_FLAG_RXNE);

        /* Check if it is the end of line (Enter key) */
        if (received_char == '\r') {
            /* Null-terminate the string and exit the loop */
            str[index] = '\0';
            print_char('\n');
            print_char('\r');
            break;
        } else if (received_char == 127) {
            print_str("\33[1D");
            print_str("\33[K");
            index--;
            continue;
        } else
            print_char(received_char);
        /* Store the received character in the buffer */
        str[index] = received_char;

        /* Increment the index for the next character */
        index++;
    }
    return index;
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

void usart_reset(void)
{
    *(RCC_APB1RSTR) |= 1 << 18;
    *(RCC_APB1RSTR) &= ~(1 << 18);
}