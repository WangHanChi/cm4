#include "dac.h"

void DAC_config()
{
    // GPIO Set
    /*GPIOA RCC Enable*/
    RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN);

    /*GPIOA Mode Set*/
    GPIOA->MODER &= ~(GPIO_MODER_MODER4);
    GPIOA->MODER |= (GPIO_MODER_MODER4);

    // DAC Set
    RCC->APB1ENR |= (RCC_APB1ENR_DACEN);

    // DAC CH1 Enable
    DAC->CR |= (DAC_CR_EN1);
}

void DAC_SetValue(uint32_t value)
{
    if (value >= 4095)
        value = 4095;
    DAC->DHR12R1 = (value);
}

void DAC_Reset()
{
    DAC->DHR12R1 &= ~(DAC_DHR12R1_DACC1DHR);
}