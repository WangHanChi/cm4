#include "command.h"
#include "reg.h"
#include "usart.h"

/* define in command.c */
extern uint32_t HOUR, MIN, SEC;
/* define in command.c */
extern uint8_t IsIRQ;

void __attribute__((interrupt)) systick_handler(void)
{
    ++SEC;
    if (SEC == 60) {
        ++MIN;
        SEC = 0;
    }
    if (MIN == 60) {
        ++HOUR;
        MIN = 0;
    }
    if (HOUR == 24) {
        print_str("One day passed !\n");
        SEC = 0;
        MIN = 0;
        HOUR = 0;
    }
    if (IsIRQ)
        print_str("Interrupt from System Timer\n");
}