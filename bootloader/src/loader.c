#include <stdint.h>

#include "stm32f4xx.h"
#include "usart.h"
#include "xprintf.h"

#define LOCATE_FUNC __attribute__((section(".mysection")))
#define FLASH_APP_ADDR 0x800C000
typedef void (*pFunction)(void);

static inline void set_MSP(uint32_t topOfMainStack)
{
    asm volatile("MSR msp, %0\n" : : "r"(topOfMainStack));
}

void loading_frame();
int check(uint32_t *address);

void go2APP(void)
{
    uint32_t JumpAddress;
    pFunction Jump_to_Application;
    xprintf("BOOTLOADER Start \n\r");

    /* check if there is something "installed" in the app FLASH region */
    if (check((uint32_t *) FLASH_APP_ADDR)) {
        loading_frame();
        xprintf("\33[2J\n");
        DELAY(500);
        /* Jump to the application */
        JumpAddress = *(uint32_t *) (FLASH_APP_ADDR + 4);
        Jump_to_Application = (pFunction) JumpAddress;
        /* initialize application's stack pointer */
        SCB->VTOR = (uint32_t) FLASH_APP_ADDR;
        set_MSP(*(uint32_t *) FLASH_APP_ADDR);
        // usart_reset();
        Jump_to_Application();
    } else {
        /* there si no application installed */
        xprintf("No APP found\n\r");
    }
}

int main(void)
{
    /* Enable UART1 and attach it to xprintf module for console */
    usart3_init(115200);
    xdev_out(usart3_putc);
    xdev_in(usart3_getc);


    char start[2] = {0};
    DELAY(3000);
    // clang-format off
    xprintf("\r\n");
    xprintf("                              ##\n");
    xprintf(" ######                       ##\n");
    xprintf(" ##   ##                      ##\n");
    xprintf(" ##   ##   #####    #####   ######\n");
    xprintf(" ######   ##   ##  ##   ##    ##\n");
    xprintf(" ##   ##  ##   ##  ##   ##    ##\n");
    xprintf(" ##   ##  ##   ##  ##   ##    ##\n");
    xprintf(" ######    #####    #####      ###\n");
    xprintf("\r\n\n");
    xprintf(
        "\rThis is a mini bootload for stm32f4\n");
    xprintf("\r\n");
    // clang-format on

    while (1) {
        xprintf("Now jump to the os\n");
        xprintf("Please enter \"ANY KEY + ENTER\" to start\n");

        while ((start[0] == 0)) {
            xgets(start, sizeof(start));
        }

        go2APP();
        DELAY(10000);
    }
    return 0;
}

void loading_frame()
{
    xprintf("\n\rWelcome : )\nProgress Bar [\e[36;01m");
    for (int i = 0; i < 60; ++i) {
        xprintf("#");
        DELAY(100);
    }
    xprintf("\e[0m]\n");
    DELAY(120);
}

int check(uint32_t *address)
{
    for (int i = 0; i < 256; ++i) {
        if (*(address + i * 4) & 0xFFFFFFFF)
            return 1;
    }
    return 0;
}