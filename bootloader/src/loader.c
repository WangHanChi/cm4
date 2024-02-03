#include <stdint.h>
#include <string.h>

#include "stm32f4xx.h"
#include "usart.h"
#include "xprintf.h"

#define PASSWORD_ADDR 0x800B000
#define FLASH_APP_ADDR 0x800C000
typedef void (*pFunction)(void);

static inline void set_MSP(uint32_t topOfMainStack);
static inline void NVIC_ClearEnableIRQs(void);
static inline void NVIC_ClearAllPendingIRQs(void);

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
        usart3_reset();
        NVIC_ClearEnableIRQs();
        NVIC_ClearAllPendingIRQs();
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


    char start[80] = {0};
    uint32_t password_len = 0;
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
        xprintf("Please enter \"PASSWORD + ENTER\" to start\n");
        xgets(start, sizeof(start));
        password_len = strlen(start);
        unsigned char *pptr = (unsigned char *) PASSWORD_ADDR;
        for (int i = 0; i < password_len; ++i) {
            if ((*(pptr)++) == start[i])
                continue;
            else {
                goto loginfail;
            }
        }

        go2APP();
        xprintf("Now jump to the os\n");
        DELAY(1000);

    loginfail:
        xprintf("You enter the wrong password!!\n\r");
    }
    return 0;
}

void loading_frame()
{
    xprintf("\n\rWelcome : )\nProgress Bar [\e[36;01m");
    for (volatile int i = 0; i < 60; ++i) {
        xprintf("#");
        DELAY(150);
    }
    xprintf("\e[0m]\n");
    DELAY(120);
}

int check(uint32_t *address)
{
    for (volatile int i = 0; i < 256; ++i) {
        if ((*(address + i * 4) & 0xFFFFFFFF) != 0xFFFFFFFF)
            return 1;
    }
    return 0;
}

static inline void set_MSP(uint32_t topOfMainStack)
{
    asm volatile("MSR msp, %0\n" : : "r"(topOfMainStack));
}

static inline void NVIC_ClearEnableIRQs(void)
{
    NVIC->ICER[0] = 0xFFFFFFFF;
    NVIC->ICER[1] = 0xFFFFFFFF;
    NVIC->ICER[2] = 0xFFFFFFFF;
    NVIC->ICER[3] = 0xFFFFFFFF;
    NVIC->ICER[4] = 0xFFFFFFFF;
    NVIC->ICER[5] = 0xFFFFFFFF;
    NVIC->ICER[6] = 0xFFFFFFFF;
    NVIC->ICER[7] = 0xFFFFFFFF;
}

static inline void NVIC_ClearAllPendingIRQs(void)
{
    NVIC->ICPR[0] = 0xFFFFFFFF;
    NVIC->ICPR[1] = 0xFFFFFFFF;
    NVIC->ICPR[2] = 0xFFFFFFFF;
    NVIC->ICPR[3] = 0xFFFFFFFF;
    NVIC->ICPR[4] = 0xFFFFFFFF;
    NVIC->ICPR[5] = 0xFFFFFFFF;
    NVIC->ICPR[6] = 0xFFFFFFFF;
    NVIC->ICPR[7] = 0xFFFFFFFF;
}
