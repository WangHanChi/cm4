#include <stdint.h>
#include "stm32f4xx.h"

#define WEAK_ALIAS __attribute__((weak, alias("Default_Handler")))

#define HSE_STARTUP_TIMEOUT                            \
    ((uint16_t) 0x0500) /*!< Time out for HSE start up \
                         */

#define PLL_M 8
#define PLL_N 360
#define PLL_P 2
#define PLL_Q 7

uint32_t DEFAULT_F_CLK;
uint32_t BAUDRATE;

/* main program entry point */
extern void main(void);

/* start address for the initialization values of the .data section.
defined in linker script */
extern uint32_t _sidata;
/* start address for the .data section. defined in linker script */
extern uint32_t _sdata;
/* end address for the .data section. defined in linker script */
extern uint32_t _edata;
/* start address for the .bss section. defined in linker script */
extern uint32_t _sbss;
/* end address for the .bss section. defined in linker script */
extern uint32_t _ebss;
/* end address for the stack. defined in linker script */
extern uint32_t _estack;

void rcc_clock_init(void);
void PLL_Init(void);

void Reset_Handler(void)
{
    /* Copy the data segment initializers from flash to SRAM */
    uint32_t *idata_begin = &_sidata;
    uint32_t *data_begin = &_sdata;
    uint32_t *data_end = &_edata;
    while (data_begin < data_end)
        *data_begin++ = *idata_begin++;

    /* Zero fill the bss segment. */
    uint32_t *bss_begin = &_sbss;
    uint32_t *bss_end = &_ebss;
    while (bss_begin < bss_end)
        *bss_begin++ = 0;

    /* Clock system initialization */
    // rcc_clock_init();
    /* Use PLL (180 MHz) System clock */
    PLL_Init();

    main();
}

void Default_Handler(void)
{
    while (1)
        ;
}

void Reset_Handler(void);
void NMI_Handler(void) WEAK_ALIAS;
void HardFault_Handler(void) WEAK_ALIAS;
void MemManage_Handler(void) WEAK_ALIAS;
void BusFault_Handler(void) WEAK_ALIAS;
void UsageFault_Handler(void) WEAK_ALIAS;
void SVC_Handler(void) WEAK_ALIAS;
void DebugMon_Handler(void) WEAK_ALIAS;
void PendSV_Handler(void) WEAK_ALIAS;
void SysTick_Handler(void) WEAK_ALIAS;
void WWDG_IRQHandler(void) WEAK_ALIAS;
void PVD_IRQHandler(void) WEAK_ALIAS;
void TAMP_STAMP_IRQHandler(void) WEAK_ALIAS;
void RTC_WKUP_IRQHandler(void) WEAK_ALIAS;
void FLASH_IRQHandler(void) WEAK_ALIAS;
void RCC_IRQHandler(void) WEAK_ALIAS;
void EXTI0_IRQHandler(void) WEAK_ALIAS;
void EXTI1_IRQHandler(void) WEAK_ALIAS;
void EXTI2_IRQHandler(void) WEAK_ALIAS;
void EXTI3_IRQHandler(void) WEAK_ALIAS;
void EXTI4_IRQHandler(void) WEAK_ALIAS;
void DMA1_Stream0_IRQHandler(void) WEAK_ALIAS;
void DMA1_Stream1_IRQHandler(void) WEAK_ALIAS;
void DMA1_Stream2_IRQHandler(void) WEAK_ALIAS;
void DMA1_Stream3_IRQHandler(void) WEAK_ALIAS;
void DMA1_Stream4_IRQHandler(void) WEAK_ALIAS;
void DMA1_Stream5_IRQHandler(void) WEAK_ALIAS;
void DMA1_Stream6_IRQHandler(void) WEAK_ALIAS;
void ADC_IRQHandler(void) WEAK_ALIAS;
void CAN1_TX_IRQHandler(void) WEAK_ALIAS;
void CAN1_RX0_IRQHandler(void) WEAK_ALIAS;
void CAN1_RX1_IRQHandler(void) WEAK_ALIAS;
void CAN1_SCE_IRQHandler(void) WEAK_ALIAS;
void EXTI9_5_IRQHandler(void) WEAK_ALIAS;
void TIM1_BRK_TIM9_IRQHandler(void) WEAK_ALIAS;
void TIM1_UP_TIM10_IRQHandler(void) WEAK_ALIAS;
void TIM1_TRG_COM_TIM11_IRQHandler(void) WEAK_ALIAS;
void TIM1_CC_IRQHandler(void) WEAK_ALIAS;
void TIM2_IRQHandler(void) WEAK_ALIAS;
void TIM3_IRQHandler(void) WEAK_ALIAS;
void TIM4_IRQHandler(void) WEAK_ALIAS;
void I2C1_EV_IRQHandler(void) WEAK_ALIAS;
void I2C1_ER_IRQHandler(void) WEAK_ALIAS;
void I2C2_EV_IRQHandler(void) WEAK_ALIAS;
void I2C2_ER_IRQHandler(void) WEAK_ALIAS;
void SPI1_IRQHandler(void) WEAK_ALIAS;
void SPI2_IRQHandler(void) WEAK_ALIAS;
void USART1_IRQHandler(void) WEAK_ALIAS;
void USART2_IRQHandler(void) WEAK_ALIAS;
void USART3_IRQHandler(void) WEAK_ALIAS;
void EXTI15_10_IRQHandler(void) WEAK_ALIAS;
void RTC_Alarm_IRQHandler(void) WEAK_ALIAS;
void OTG_FS_WKUP_IRQHandler(void) WEAK_ALIAS;
void TIM8_BRK_TIM12_IRQHandler(void) WEAK_ALIAS;
void TIM8_UP_TIM13_IRQHandler(void) WEAK_ALIAS;
void TIM8_TRG_COM_TIM14_IRQHandler(void) WEAK_ALIAS;
void TIM8_CC_IRQHandler(void) WEAK_ALIAS;
void DMA1_Stream7_IRQHandler(void) WEAK_ALIAS;
void FMC_IRQHandler(void) WEAK_ALIAS;
void SDIO_IRQHandler(void) WEAK_ALIAS;
void TIM5_IRQHandler(void) WEAK_ALIAS;
void SPI3_IRQHandler(void) WEAK_ALIAS;
void UART4_IRQHandler(void) WEAK_ALIAS;
void UART5_IRQHandler(void) WEAK_ALIAS;
void TIM6_DAC_IRQHandler(void) WEAK_ALIAS;
void TIM7_IRQHandler(void) WEAK_ALIAS;
void DMA2_Stream0_IRQHandler(void) WEAK_ALIAS;
void DMA2_Stream1_IRQHandler(void) WEAK_ALIAS;
void DMA2_Stream2_IRQHandler(void) WEAK_ALIAS;
void DMA2_Stream3_IRQHandler(void) WEAK_ALIAS;
void DMA2_Stream4_IRQHandler(void) WEAK_ALIAS;
void ETH_IRQHandler(void) WEAK_ALIAS;
void ETH_WKUP_IRQHandler(void) WEAK_ALIAS;
void CAN2_TX_IRQHandler(void) WEAK_ALIAS;
void CAN2_RX0_IRQHandler(void) WEAK_ALIAS;
void CAN2_RX1_IRQHandler(void) WEAK_ALIAS;
void CAN2_SCE_IRQHandler(void) WEAK_ALIAS;
void OTG_FS_IRQHandler(void) WEAK_ALIAS;
void DMA2_Stream5_IRQHandler(void) WEAK_ALIAS;
void DMA2_Stream6_IRQHandler(void) WEAK_ALIAS;
void DMA2_Stream7_IRQHandler(void) WEAK_ALIAS;
void USART6_IRQHandler(void) WEAK_ALIAS;
void I2C3_EV_IRQHandler(void) WEAK_ALIAS;
void I2C3_ER_IRQHandler(void) WEAK_ALIAS;
void OTG_HS_EP1_OUT_IRQHandler(void) WEAK_ALIAS;
void OTG_HS_EP1_IN_IRQHandler(void) WEAK_ALIAS;
void OTG_HS_WKUP_IRQHandler(void) WEAK_ALIAS;
void OTG_HS_IRQHandler(void) WEAK_ALIAS;
void DCMI_IRQHandler(void) WEAK_ALIAS;
void HASH_RNG_IRQHandler(void) WEAK_ALIAS;
void FPU_IRQHandler(void) WEAK_ALIAS;
void UART7_IRQHandler(void) WEAK_ALIAS;
void UART8_IRQHandler(void) WEAK_ALIAS;
void SPI4_IRQHandler(void) WEAK_ALIAS;
void SPI5_IRQHandler(void) WEAK_ALIAS;
void SPI6_IRQHandler(void) WEAK_ALIAS;
void SAI1_IRQHandler(void) WEAK_ALIAS;
void LTDC_IRQHandler(void) WEAK_ALIAS;
void LTDC_ER_IRQHandler(void) WEAK_ALIAS;
void DMA2D_IRQHandler(void) WEAK_ALIAS;

__attribute__((section(".isr_vector"))) void *isr_vectors[] = {
    (void *) &_estack, /* stack pointer */
    Reset_Handler, NMI_Handler, HardFault_Handler, MemManage_Handler,
    BusFault_Handler, UsageFault_Handler, 0, 0, 0, 0, SVC_Handler,
    DebugMon_Handler, 0, PendSV_Handler, SysTick_Handler,
    /* External Interrupts          */
    WWDG_IRQHandler,          /* Window WatchDog              */
    PVD_IRQHandler,           /* PVD through EXTI Line detection */
    TAMP_STAMP_IRQHandler,    /* Tamper and TimeStamps through the EXTI line */
    RTC_WKUP_IRQHandler,      /* RTC Wakeup through the EXTI line */
    FLASH_IRQHandler,         /* FLASH                        */
    RCC_IRQHandler,           /* RCC                          */
    EXTI0_IRQHandler,         /* EXTI Line0                   */
    EXTI1_IRQHandler,         /* EXTI Line1                   */
    EXTI2_IRQHandler,         /* EXTI Line2                   */
    EXTI3_IRQHandler,         /* EXTI Line3                   */
    EXTI4_IRQHandler,         /* EXTI Line4                   */
    DMA1_Stream0_IRQHandler,  /* DMA1 Stream 0                */
    DMA1_Stream1_IRQHandler,  /* DMA1 Stream 1                */
    DMA1_Stream2_IRQHandler,  /* DMA1 Stream 2                */
    DMA1_Stream3_IRQHandler,  /* DMA1 Stream 3                */
    DMA1_Stream4_IRQHandler,  /* DMA1 Stream 4                */
    DMA1_Stream5_IRQHandler,  /* DMA1 Stream 5                */
    DMA1_Stream6_IRQHandler,  /* DMA1 Stream 6                */
    ADC_IRQHandler,           /* ADC1, ADC2 and ADC3s         */
    CAN1_TX_IRQHandler,       /* CAN1 TX                      */
    CAN1_RX0_IRQHandler,      /* CAN1 RX0                     */
    CAN1_RX1_IRQHandler,      /* CAN1 RX1                     */
    CAN1_SCE_IRQHandler,      /* CAN1 SCE                     */
    EXTI9_5_IRQHandler,       /* External Line[9:5]s          */
    TIM1_BRK_TIM9_IRQHandler, /* TIM1 Break and TIM9          */
    TIM1_UP_TIM10_IRQHandler, /* TIM1 Update and TIM10        */
    TIM1_TRG_COM_TIM11_IRQHandler, /* TIM1 Trigger and Commutation and TIM11 */
    TIM1_CC_IRQHandler,            /* TIM1 Capture Compare         */
    TIM2_IRQHandler,               /* TIM2                         */
    TIM3_IRQHandler,               /* TIM3                         */
    TIM4_IRQHandler,               /* TIM4                         */
    I2C1_EV_IRQHandler,            /* I2C1 Event                   */
    I2C1_ER_IRQHandler,            /* I2C1 Error                   */
    I2C2_EV_IRQHandler,            /* I2C2 Event                   */
    I2C2_ER_IRQHandler,            /* I2C2 Error                   */
    SPI1_IRQHandler,               /* SPI1                         */
    SPI2_IRQHandler,               /* SPI2                         */
    USART1_IRQHandler,             /* USART1                       */
    USART2_IRQHandler,             /* USART2                       */
    USART3_IRQHandler,             /* USART3                       */
    EXTI15_10_IRQHandler,          /* External Line[15:10]s        */
    RTC_Alarm_IRQHandler,          /* RTC Alarm (A and B) through EXTI Line */
    OTG_FS_WKUP_IRQHandler,        /* USB OTG FS Wakeup through EXTI line */
    TIM8_BRK_TIM12_IRQHandler,     /* TIM8 Break and TIM12         */
    TIM8_UP_TIM13_IRQHandler,      /* TIM8 Update and TIM13        */
    TIM8_TRG_COM_TIM14_IRQHandler, /* TIM8 Trigger and Commutation and TIM14 */
    TIM8_CC_IRQHandler,            /* TIM8 Capture Compare         */
    DMA1_Stream7_IRQHandler,       /* DMA1 Stream7                 */
    FMC_IRQHandler,                /* FMC                         */
    SDIO_IRQHandler,               /* SDIO                         */
    TIM5_IRQHandler,               /* TIM5                         */
    SPI3_IRQHandler,               /* SPI3                         */
    UART4_IRQHandler,              /* UART4                        */
    UART5_IRQHandler,              /* UART5                        */
    TIM6_DAC_IRQHandler,           /* TIM6 and DAC1&2 underrun errors */
    TIM7_IRQHandler,               /* TIM7                         */
    DMA2_Stream0_IRQHandler,       /* DMA2 Stream 0                */
    DMA2_Stream1_IRQHandler,       /* DMA2 Stream 1                */
    DMA2_Stream2_IRQHandler,       /* DMA2 Stream 2                */
    DMA2_Stream3_IRQHandler,       /* DMA2 Stream 3                */
    DMA2_Stream4_IRQHandler,       /* DMA2 Stream 4                */
    ETH_IRQHandler,                /* Ethernet                     */
    ETH_WKUP_IRQHandler,           /* Ethernet Wakeup through EXTI line */
    CAN2_TX_IRQHandler,            /* CAN2 TX                      */
    CAN2_RX0_IRQHandler,           /* CAN2 RX0                     */
    CAN2_RX1_IRQHandler,           /* CAN2 RX1                     */
    CAN2_SCE_IRQHandler,           /* CAN2 SCE                     */
    OTG_FS_IRQHandler,             /* USB OTG FS                   */
    DMA2_Stream5_IRQHandler,       /* DMA2 Stream 5                */
    DMA2_Stream6_IRQHandler,       /* DMA2 Stream 6                */
    DMA2_Stream7_IRQHandler,       /* DMA2 Stream 7                */
    USART6_IRQHandler,             /* USART6                       */
    I2C3_EV_IRQHandler,            /* I2C3 event                   */
    I2C3_ER_IRQHandler,            /* I2C3 error                   */
    OTG_HS_EP1_OUT_IRQHandler,     /* USB OTG HS End Point 1 Out   */
    OTG_HS_EP1_IN_IRQHandler,      /* USB OTG HS End Point 1 In    */
    OTG_HS_WKUP_IRQHandler,        /* USB OTG HS Wakeup through EXTI */
    OTG_HS_IRQHandler,             /* USB OTG HS                   */
    DCMI_IRQHandler,               /* DCMI                         */
    0,                             /* Reserved                     */
    HASH_RNG_IRQHandler,           /* Hash and Rng                 */
    FPU_IRQHandler,                /* FPU                          */
    UART7_IRQHandler,              /* UART7                        */
    UART8_IRQHandler,              /* UART8                        */
    SPI4_IRQHandler,               /* SPI4                         */
    SPI5_IRQHandler,               /* SPI5 						*/
    SPI6_IRQHandler,               /* SPI6						    */
    SAI1_IRQHandler,               /* SAI1						    */
    LTDC_IRQHandler,               /* LTDC_IRQHandler			    */
    LTDC_ER_IRQHandler,            /* LTDC_ER_IRQHandler			*/
    DMA2D_IRQHandler               /* DMA2D                        */
};

void rcc_clock_init(void)
{
    /* Reset the RCC clock configuration to the default reset state(for debug
     * purpose) */
    /* Set HSION bit */
    RCC->CR |= RCC_CR_HSION;

    /* Reset HSEON, CSSON and PLLON bits */
    RCC->CR &= (uint32_t) 0xFEF6FFFF;

    /* Reset HSEBYP bit */
    RCC->CR &= (uint32_t) 0xFFFBFFFF;

    /* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits */
    RCC->CFGR &= (uint32_t) 0x24003010;

    /* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */
    RCC->CFGR &= (uint32_t) 0x00000000;

    /* Disable all interrupts and clear pending bits  */
    RCC->CIR = (uint32_t) 0x00000000;

    /* Configure the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers */
    /* Configure the Flash Latency cycles and enable prefetch buffer */
    volatile uint32_t StartUpCounter = 0, HSEStatus = 0;

    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/
    /* Enable HSE */
    RCC->CR |= (uint32_t) RCC_CR_HSEON;

    /* Wait till HSE is ready and if Time out is reached exit */
    do {
        HSEStatus = RCC->CR & RCC_CR_HSERDY;
        StartUpCounter++;
    } while ((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CR & RCC_CR_HSERDY) != 0)
        HSEStatus = (uint32_t) 0x01;
    else
        HSEStatus = (uint32_t) 0x00;

    if (HSEStatus == (uint32_t) 0x01) {
        /* Enable Prefetch Buffer */
        FLASH->ACR |= FLASH_ACR_PRFTEN;

        /* Flash 0 wait state */
        FLASH->ACR &= (uint32_t) ((uint32_t) ~FLASH_ACR_LATENCY);

        FLASH->ACR |= (uint32_t) FLASH_ACR_LATENCY_0WS;

        /* HCLK = SYSCLK */
        RCC->CFGR |= (uint32_t) RCC_CFGR_HPRE_DIV1;

        /* PCLK2 = HCLK */
        RCC->CFGR |= (uint32_t) RCC_CFGR_PPRE2_DIV1;

        /* PCLK1 = HCLK */
        RCC->CFGR |= (uint32_t) RCC_CFGR_PPRE1_DIV1;

        /* Set System Clock 8Mhz, and APB1 clock is divides 1 */
        DEFAULT_F_CLK = 8000000 / 1;
        BAUDRATE = 115200;

        /* Select HSE as system clock source */
        RCC->CFGR &= (uint32_t) ((uint32_t) ~(RCC_CFGR_SW));
        RCC->CFGR |= (uint32_t) RCC_CFGR_SW_HSE;

        /* Wait till HSE is used as system clock source */
        while ((RCC->CFGR & (uint32_t) RCC_CFGR_SWS) !=
               (uint32_t) RCC_CFGR_SWS_HSE)
            ;
    } else {
        /* If HSE fails to start-up, the application will have wrong clock
        configuration. User can add here some code to deal with this error */
    }
}

void PLL_Init()
{
    /* Systrm Init */
    /* HSE ON */
    RCC->CR |= (uint32_t) RCC_CR_HSEON;

    while (!(RCC->CR & RCC_CR_HSERDY))
        ;

    /* SET PLL OFF  */
    RCC->CR &= ~(RCC_CR_PLLON);

    /* Reset PLLCFGR register */
    RCC->CFGR = 0x00000000;

    /* Reset PLLCFGR register */
    RCC->PLLCFGR = 0x24003010;

    /* Reset HESON, CSSON and PLLON bits */
    //*RCC_CR &= (uint32_t) 0xFEF6FFFF;

    /************* SetSysClock ************/

    RCC->PLLCFGR =
        PLL_M | (PLL_N << 6) | (((PLL_P >> 1) - 1) << 16) | (PLL_Q << 24);

    /* PLL clock Source HSI or HSE */
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;

    /* APB1 PWR Enable */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    /* Select regulator voltage output Scale 1 mode, System frequency up to 180
     * MHz */
    PWR->CR |= PWR_CR_VOS_0;

    /* AHB div 1 */
    RCC->CFGR &= ~(RCC_CFGR_HPRE);
    RCC->CFGR |= (RCC_CFGR_HPRE_DIV1);

    /* APB1 Div = 4  */
    RCC->CFGR &= ~(RCC_CFGR_PPRE1);
    RCC->CFGR |= (RCC_CFGR_PPRE1_DIV4);

    /* APB2 Div =  2*/
    RCC->CFGR &= ~(RCC_CFGR_PPRE2);
    RCC->CFGR |= (RCC_CFGR_PPRE2_DIV2);

    /* Set System clock 180Mhz, and APB1 Max Freq is 45Mhz */
    DEFAULT_F_CLK = (180000000 / 4);
    BAUDRATE = 115200U;

    /* SET PLL ON  */
    RCC->CR |= (RCC_CR_PLLON);

    /* Check PLL is ready */
    while (!((RCC->CR) & (RCC_CR_PLLRDY)))
        ;

    /* Enable the Over-drive to extend the clock frequency to 180 Mhz */
    PWR->CR |= (PWR_CR_ODEN);
    while (!((PWR->CSR) & (PWR_CSR_ODRDY)))
        ;

    PWR->CR |= (PWR_CR_ODSWEN);
    while (!((PWR->CSR) & (PWR_CSR_ODSWRDY)))
        ;

    FLASH->ACR |= (FLASH_ACR_PRFTEN);
    FLASH->ACR |= (FLASH_ACR_ICEN);
    FLASH->ACR |= (FLASH_ACR_DCEN);
    FLASH->ACR |= FLASH_ACR_LATENCY_5WS;

    /* Select the main PLL as system clock source */
    RCC->CFGR &= ~(RCC_CFGR_SW);
    RCC->CFGR |= (RCC_CFGR_SW_1);

    /* Wait till the main PLL is used as system clock source */
    while (((RCC->CFGR & RCC_CFGR_SWS)) != RCC_CFGR_SWS_PLL)
        ;
}