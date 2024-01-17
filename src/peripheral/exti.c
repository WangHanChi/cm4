#include "gpio.h"
#include "xprintf.h"

extern float pulse_A;

void exti_config()
{
    /* enable the system  clock */
    (RCC->APB2ENR) |= (RCC_APB2ENR_SYSCFGEN);

    /* config EXTI3 line for PD0 */
    SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI0_PD << SYSCFG_EXTICR1_EXTI3_Pos);

    /* disable Mask on EXTI3 */
    (EXTI->IMR) |= (EXTI_IMR_MR3_Msk);

    /* select rising trigger for EXTI3 */
    (EXTI->RTSR) |= (EXTI_RTSR_TR3);

    (EXTI->FTSR) |= (EXTI_FTSR_TR3);

    /* set priority for EXTI0_IRQ_NO */
    NVIC_SetPriority(EXTI3_IRQn, 0x79);
    // set_irqs_pending_bit(EXTI0_IRQ_NO);
    NVIC_EnableIRQ(EXTI3_IRQn);
}

void __attribute__((interrupt)) EXTI3_IRQHandler(void)
{
    if ((EXTI->PR) & (EXTI_PR_PR3)) {
        // xprintf("In EXTI\n\r");
        uint8_t status_A = GPIO_ReadPin(GPIOD, 3);
        uint8_t status_B = GPIO_ReadPin(GPIOD, 4);

        (status_A == status_B) ? --pulse_A : ++pulse_A;
        (EXTI->PR) |= (EXTI_PR_PR3);
    }
}