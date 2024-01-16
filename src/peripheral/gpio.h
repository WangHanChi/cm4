#pragma once

#include "stm32f429xx.h"

typedef enum {
    GPIO_A = 0,
    GPIO_B,
    GPIO_C,
    GPIO_D,
    GPIO_E,
    GPIO_F,
    GPIO_G,
    GPIO_H,
    GPIO_I,
    GPIO_J,
    GPIO_K,
} GPIO_PORT;

typedef enum {
    GPIO_PIN_NO_0 = 0,
    GPIO_PIN_NO_1,
    GPIO_PIN_NO_2,
    GPIO_PIN_NO_3,
    GPIO_PIN_NO_4,
    GPIO_PIN_NO_5,
    GPIO_PIN_NO_6,
    GPIO_PIN_NO_7,
    GPIO_PIN_NO_8,
    GPIO_PIN_NO_9,
    GPIO_PIN_NO_10,
    GPIO_PIN_NO_11,
    GPIO_PIN_NO_12,
    GPIO_PIN_NO_13,
    GPIO_PIN_NO_14,
    GPIO_PIN_NO_15,
} GPIO_PIN_NO;

typedef enum {
    INPUT = 0,
    OUTPUT,
    AF,
    ANALOG,
} MODER;

typedef enum {
    PUSH_PULL = 0,
    OPEN_DRAIN,
} OTYPER;

typedef enum {
    LOW_SPEED = 0,
    MEDIUM_SPEED,
    HIGH_SPEED,
    VERY_HIGH_SPEED,
} OSPEEDR;

typedef enum {
    NO_PP_PD = 0,
    PULL_UP,
    PULL_DOWN,
} PUPDR;

typedef enum {
    GPIO_AF0 = 0,
    GPIO_AF1,
    GPIO_AF2,
    GPIO_AF3,
    GPIO_AF4,
    GPIO_AF5,
    GPIO_AF6,
    GPIO_AF7,
    GPIO_AF8,
    GPIO_AF9,
    GPIO_AF10,
    GPIO_AF11,
    GPIO_AF12,
    GPIO_AF13,
    GPIO_AF14,
    GPIO_AF15,
} GPIO_AF;

typedef enum {
    GREEN = 0,
    BLUE = 7,
    RED = 14,
} USER_LED;

typedef enum {
    LED_OFF = 0,
    LED_ON,
    LED_BLINK,
} LED_STATUS;


/* Configuration structure for a GPIO pin */
typedef struct {
    GPIO_PORT port;
    GPIO_PIN_NO PinNumber; /* possible values from @GPIO_PIN_NUMBER   */
    MODER PinMode;         /* possible values from @GPIO_PIN_MODES    */
    OSPEEDR PinSpeed;      /* possible values from @GPIO_PIN_SPEED    */
    PUPDR PinPUPD;         /* possible values from @GPIO_PIN_PUPD_CFG */
    OTYPER PinOType;       /* possible values from @GPIO_PIN_OTYPE    */
    GPIO_AF PinAltFun;     /* possible values from @GPIO_PIN_ALTFUNC  */
} GPIO_PinCFG_t;


/* Handle structure for a GPIO pin */
typedef struct {
    GPIO_TypeDef *pGPIOx;
    GPIO_PinCFG_t GPIO_PINCFG; /* This holds GPIO pin configuration setting */
} GPIO_Handle_t;

#define USER_LED_CONTROL(COLOR, STATUS)         \
    do {                                        \
        RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOBEN);  \
        GPIOB->MODER &= ~(0x03 << (COLOR * 2)); \
        GPIOB->MODER |= (0x01 << (COLOR * 2));  \
        if (STATUS == LED_OFF)                  \
            GPIOB->BSRR |= (1 << (COLOR + 16)); \
        else if (STATUS == LED_ON)              \
            GPIOB->BSRR |= (1 << (COLOR));      \
        else {                                  \
            GPIOB->BSRR |= (1 << (COLOR));      \
            for (int i = 0; i < 5000000; i++)   \
                ;                               \
            GPIOB->BSRR |= (1 << (COLOR + 16)); \
            for (int i = 0; i < 5000000; i++)   \
                ;                               \
        }                                       \
    } while (0)

#define GPIO_Init(pGPIOHandle)                                             \
    do {                                                                   \
        RCC->AHB1ENR |= (1 << (pGPIOHandle)->GPIO_PINCFG.port);            \
        (pGPIOHandle)->pGPIOx->MODER |=                                    \
            ((pGPIOHandle)->GPIO_PINCFG.PinMode                            \
             << ((pGPIOHandle)->GPIO_PINCFG.PinNumber * 2));               \
        (pGPIOHandle)->pGPIOx->OSPEEDR |=                                  \
            ((pGPIOHandle)->GPIO_PINCFG.PinSpeed                           \
             << ((pGPIOHandle)->GPIO_PINCFG.PinNumber * 2));               \
        (pGPIOHandle)->pGPIOx->OTYPER |=                                   \
            ((pGPIOHandle)->GPIO_PINCFG.PinOType                           \
             << (pGPIOHandle)->GPIO_PINCFG.PinNumber);                     \
        (pGPIOHandle)->pGPIOx->PUPDR |=                                    \
            ((pGPIOHandle)->GPIO_PINCFG.PinPUPD                            \
             << ((pGPIOHandle)->GPIO_PINCFG.PinNumber * 2));               \
        if ((pGPIOHandle)->GPIO_PINCFG.PinMode == AF) {                    \
            (pGPIOHandle)                                                  \
                ->pGPIOx->AFR[(pGPIOHandle)->GPIO_PINCFG.PinAltFun / 8] |= \
                ((pGPIOHandle)->GPIO_PINCFG.PinAltFun                      \
                 << ((pGPIOHandle)->GPIO_PINCFG.PinNumber % 8) * 4);       \
        }                                                                  \
    } while (0)


#define GPIO_DeInit(pGPIOHandle)                               \
    do {                                                       \
        RCC->AHB1ENR &= ~(1 << ((pGPIOHandle)->GPIO_PINCFG.port)); \
    } while (0)

#define GPIO_ReadPin(pGPIOx, PinNumber) (((pGPIOx)->IDR) & (1 << (PinNumber)))

#define GPIO_WritePin(pGPIOx, PinNumber, Enable)     \
    do {                                             \
        if (Enable)                                  \
            pGPIOx->BSRR |= (1 << PinNumber);        \
        else                                         \
            pGPIOx->BSRR |= (1 << (PinNumber + 16)); \
    } while (0)

#define GPIO_TogglePin(pGPIOx, PinNumber)  \
    do {                                   \
        (pGPIOx->ODR ^= (1 << PinNumber)); \
    } while (0)

#define USRT_BUTTON 13

#define USER_BUTTON_PRESS() (((GPIOC)->IDR) & (1 << (USRT_BUTTON)))

#define USER_BUTTON_Init()                                           \
    do {                                                             \
        RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOCEN);                       \
        GPIOC->MODER &= ~(GPIO_MODER_MODER13);                       \
        RCC->APB2ENR |= (RCC_APB2ENR_SYSCFGEN);                      \
        SYSCFG->EXTICR[3] |=                                         \
            (SYSCFG_EXTICR4_EXTI12_PC << SYSCFG_EXTICR4_EXTI13_Pos); \
        EXTI->IMR |= (EXTI_IMR_IM13);                                \
        EXTI->RTSR |= (EXTI_RTSR_TR13);                              \
        EXTI->FTSR &= ~(EXTI_FTSR_TR13);                             \
    } while (0)
