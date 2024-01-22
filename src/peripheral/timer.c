#include "timer.h"
#include "stm32f429xx.h"

#include "xprintf.h"


void Tim2_config()
{
    /* Enable RCC APB1 TIM2EN */
    RCC->APB1ENR |= (RCC_APB1ENR_TIM2EN);

    /* Set PSC */
    TIM2->PSC = 1000 - 1;

    /* Set the timer tick 20 times per second */
    TIM2->ARR = 4500 - 1;

    /* Enable Timer interrupt generation */
    TIM2->DIER |= (TIM_DIER_UIE);

    /* clear Update interrupt flag */
    TIM2->SR &= ~(TIM_SR_UIF);

    /* Set interrupt flag update generation*/
    TIM2->EGR |= (TIM_EGR_UG);

    /* Disable timer, from now */
    TIM2->CR1 &= ~(TIM_CR1_CEN);
}

void Tim2_Start()
{
    NVIC_SetPriority(TIM2_IRQn, 5);
    NVIC_EnableIRQ(TIM2_IRQn);
    /* Enable timer */
    TIM2->CR1 |= (TIM_CR1_CEN);
}
