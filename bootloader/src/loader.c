#include <stdint.h>
#include "reg.h"
#include "usart.h"

#define LOCATE_FUNC __attribute__((section(".mysection")))
#define FLASH_APP_ADDR 0x8008000
typedef void (*pFunction)(void);

static inline void __set_MSP(uint32_t topOfMainStack)
{
  asm volatile ("MSR msp, %0\n" : : "r" (topOfMainStack));
}

void loading_frame();
int check(uint32_t *address);

void go2APP(void)
{
    uint32_t JumpAddress;
    pFunction Jump_to_Application;
    print_str("BOOTLOADER Start \n\r");

    /* check if there is something "installed" in the app FLASH region */
    if(check((uint32_t *)FLASH_APP_ADDR)){
        loading_frame();
        print_str("\33[2J\n");
        DELAY(500);
        /* Jump to the application */
        JumpAddress = *(uint32_t *) (FLASH_APP_ADDR + 4);
        Jump_to_Application = (pFunction) JumpAddress;
        /* initialize application's stack pointer */
        *(SCB_VTOR) = (uint32_t) FLASH_APP_ADDR;
        __set_MSP(*(uint32_t *) FLASH_APP_ADDR);
        usart_reset();
        Jump_to_Application();
    } else{
        /* there si no application installed */
        print_str("No APP found\n\r");
    }   
}

int main(void)
{
    usart_init();
    char start[1] = {0};
    DELAY(1000);
    // clang-format off
    print_str("\r\n");
    print_str("                              ##\n");
    print_str(" ######                       ##\n");
    print_str(" ##   ##                      ##\n");
    print_str(" ##   ##   #####    #####   ######\n");
    print_str(" ######   ##   ##  ##   ##    ##\n");
    print_str(" ##   ##  ##   ##  ##   ##    ##\n");
    print_str(" ##   ##  ##   ##  ##   ##    ##\n");
    print_str(" ######    #####    #####      ###\n");
    print_str("\r\n\n");
    print_str(
        "\rThis is a mini bootload for stm32f4\n");
    print_str("\r\n");
    // clang-format on

    while(1)
    {   
        print_str("Now jump to the os\n");
        print_str("Please enter \"ANY KEY + ENTER\" to start\n");

        while((start[0] == 0)){
            scan_str(start);    
        }
        
        go2APP();
        DELAY(10000);
    }
	return 0;
}

void loading_frame()
{
    print_str("\n\rWelcome : )\nProgress Bar [\e[36;01m");
    for(int i = 0 ; i < 60; ++i){
        print_char('#');
        DELAY(10);
    }
    print_str("\e[0m]\n");
    DELAY(120);
}

int check(uint32_t *address)
{
    for(int i = 0 ; i < 256; ++i){
        if(*(address + i*4) & 0xFFFFFFFF)
            return 1;
    }
    return 0;
}