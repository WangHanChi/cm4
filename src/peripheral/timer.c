#include "timer.h"
#include "stm32f429xx.h"

#include "xprintf.h"

extern float pulse_A;
extern float pre_pulse_A;
extern float speed_A;
extern uint8_t display_speed;
int count = 0;


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

    NVIC_SetPriority(TIM2_IRQn, 0x81);
    NVIC_EnableIRQ(TIM2_IRQn);

    /* clear Update interrupt flag */
    TIM2->SR &= ~(TIM_SR_UIF);

    /* Set interrupt flag update generation*/
    TIM2->EGR |= (TIM_EGR_UG);

    /* Disable timer, from now */
    TIM2->CR1 &= ~(TIM_CR1_CEN);
}

void Tim2_Start()
{
    /* Enable timer */
    TIM2->CR1 |= (TIM_CR1_CEN);
}

void __attribute__((interrupt)) TIM2_IRQHandler(void)
{
    speed_A = 60 * (pulse_A - pre_pulse_A) * (200.0) / 1000.0;
    if (display_speed)
        xprintf("%d ->%d \n\r", count++, (int) speed_A);
    pre_pulse_A = pulse_A;
    /* Clear UIF update interrupt flag */
    TIM2->SR &= ~(TIM_SR_UIF);
}