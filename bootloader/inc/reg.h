#ifndef __REG_H_
#define __REG_H_

#define __REG_TYPE volatile uint32_t
#define __REG __REG_TYPE *

/* RCC Memory Map */
#define RCC             ((__REG_TYPE) 0x40023800)
#define RCC_CR          ((__REG) (RCC + 0x00))
#define RCC_PLLCFGR     ((__REG) (RCC + 0x04))
#define RCC_CFGR        ((__REG) (RCC + 0x08))
#define RCC_CIR         ((__REG) (RCC + 0x0C))
#define RCC_AHB1RSTR    ((__REG) (RCC + 0x10))
#define RCC_AHB2RSTR    ((__REG) (RCC + 0x14))
#define RCC_APB1RSTR    ((__REG) (RCC + 0x20))
#define RCC_APB2RSTR    ((__REG) (RCC + 0x24))
#define RCC_AHB1ENR     ((__REG) (RCC + 0x30))
#define RCC_AHB2ENR     ((__REG) (RCC + 0x34))
#define RCC_APB1ENR     ((__REG) (RCC + 0x40))
#define RCC_APB2ENR     ((__REG) (RCC + 0x44))
#define RCC_BDCR        ((__REG) (RCC + 0x70))
#define RCC_CSR         ((__REG) (RCC + 0x74))

/* Flash Memory Map */
#define FLASH           ((__REG_TYPE) 0x40023C00)
#define FLASH_ACR       ((__REG) (FLASH + 0x00))

/* GPIO Memory Map */

// GPIOA
#define GPIOA_BASE      ((__REG_TYPE) 0x40020000)
#define GPIOA           ((__REG) (GPIOA_BASE + 0x00))
#define GPIOA_IDR       ((__REG) (GPIOA_BASE + 0x10))
#define GPIOA_ODR       ((__REG) (GPIOA_BASE + 0x14))
#define GPIOA_BSRR      ((__REG) (GPIOA_BASE + 0x18))
#define GPIOA_LCKR      ((__REG) (GPIOA_BASE + 0x1C))
#define GPIOA_AFRL      ((__REG) (GPIOA_BASE + 0x20))
#define GPIOA_AFRH      ((__REG) (GPIOA_BASE + 0x24))

// GPIOB
#define GPIOB_BASE      ((__REG_TYPE) 0x40020400)
#define GPIOB           ((__REG) (GPIOB_BASE + 0x00))
#define GPIOB_OTYPER    ((__REG) (GPIOB_BASE + 0x04))
#define GPIOB_OSPEEDR   ((__REG) (GPIOB_BASE + 0x08))
#define GPIOB_PUPDR     ((__REG) (GPIOB_BASE + 0x0C))
#define GPIOB_IDR       ((__REG) (GPIOB_BASE + 0x10))
#define GPIOB_ODR       ((__REG) (GPIOB_BASE + 0x14))
#define GPIOB_BSRR      ((__REG) (GPIOB_BASE + 0x18))
#define GPIOB_LCKR      ((__REG) (GPIOB_BASE + 0x1C))
#define GPIOB_AFRL      ((__REG) (GPIOB_BASE + 0x20))
#define GPIOB_AFRH      ((__REG) (GPIOB_BASE + 0x24))

// GPIOD
#define GPIOD_BASE      ((__REG_TYPE) 0x40020C00)
#define GPIOD           ((__REG) (GPIOD_BASE + 0x00))
#define GPIOD_IDR       ((__REG) (GPIOD_BASE + 0x10))
#define GPIOD_ODR       ((__REG) (GPIOD_BASE + 0x14))
#define GPIOD_BSRR      ((__REG) (GPIOD_BASE + 0x18))
#define GPIOD_LCKR      ((__REG) (GPIOD_BASE + 0x1C))
#define GPIOD_AFRL      ((__REG) (GPIOD_BASE + 0x20))
#define GPIOD_AFRH      ((__REG) (GPIOD_BASE + 0x24))

/* USART3 Memory Map */
#define USART3          ((__REG_TYPE) 0x40004800)
#define USART3_SR       ((__REG) (USART3 + 0x00))
#define USART3_DR       ((__REG) (USART3 + 0x04))
#define USART3_BRR      ((__REG) (USART3 + 0x08))
#define USART3_CR1      ((__REG) (USART3 + 0x0C))
#define USART3_CR2      ((__REG) (USART3 + 0x10))
#define USART3_CR3      ((__REG) (USART3 + 0x14))
#define USART3_GTPR     ((__REG) (USART3 + 0x18))

/* ADC Memory Map */
#define ADC             ((__REG_TYPE) 0x40012000)
#define ADC1_SR         ((__REG) (ADC + 0x00))
#define ADC1_CR1        ((__REG) (ADC + 0x04))
#define ADC1_CR2        ((__REG) (ADC + 0x08))
#define ADC1_SMPR1      ((__REG) (ADC + 0x0C))
#define ADC1_SMPR2      ((__REG) (ADC + 0x10))
#define ADC1_JOFR1      ((__REG) (ADC + 0x14))
#define ADC1_JOFR2      ((__REG) (ADC + 0x18))
#define ADC1_JOFR3      ((__REG) (ADC + 0x1C))
#define ADC1_JOFR4      ((__REG) (ADC + 0x20))
#define ADC1_HTR        ((__REG) (ADC + 0x24))
#define ADC1_LTR        ((__REG) (ADC + 0x28))
#define ADC1_SQR1       ((__REG) (ADC + 0x2C))
#define ADC1_SQR2       ((__REG) (ADC + 0x30))
#define ADC1_SQR3       ((__REG) (ADC + 0x34))
#define ADC1_JSQR       ((__REG) (ADC + 0x38))
#define ADC1_JDR1       ((__REG) (ADC + 0x3C))
#define ADC1_JDR2       ((__REG) (ADC + 0x40))
#define ADC1_JDR3       ((__REG) (ADC + 0x44))
#define ADC1_JDR4       ((__REG) (ADC + 0x48))
#define ADC1_DR         ((__REG) (ADC + 0x4C))

#define ADC_CSR         ((__REG) (ADC + 0x00 + 0x300))
#define ADC_CCR         ((__REG) (ADC + 0x04 + 0x300))
#define ADC_CDER        ((__REG) (ADC + 0x08 + 0x300))

/* DAC Memory Map */
#define DAC            ((__REG_TYPE) 0x40007400)
#define DAC_CR         ((__REG) (DAC + 0x00))
#define DAC_DHR12R1    ((__REG) (DAC + 0x08))
#define DAC_DOR1       ((__REG) (DAC + 0x2C))
#define DAC_SR         ((__REG) (DAC + 0x34))

/* System Control block */
#define SCB             ((__REG_TYPE) 0xE000ED00)
#define SCB_CPUID	    ((__REG) (SCB + 0x00))
#define SCB_ICSR	    ((__REG) (SCB + 0x04))
#define SCB_VTOR	    ((__REG) (SCB + 0x08))
#define SCB_AIRCR	    ((__REG) (SCB + 0x0C))
#define SCB_SCR 	    ((__REG) (SCB + 0x10))
#define SCB_CCR 	    ((__REG) (SCB + 0x14))
#define SCB_ICSR_PENDSVSET  (uint32_t) (1 << 28)

/* Power Control Register */
#define PWR		        ((__REG_TYPE) 0x40007000)
#define PWR_CR	        ((__REG) (PWR + 0x00))
#define PWR_CSR	        ((__REG) (PWR + 0x04))

/* I2C1 Memory Map */
#define I2C1		    ((__REG_TYPE) 0x40005400)
#define I2C1_CR1	    ((__REG) (I2C1 + 0x00))
#define I2C1_CR2	    ((__REG) (I2C1 + 0x04))
#define I2C1_OAR1       ((__REG) (I2C1 + 0x08))
#define I2C1_OAR2	    ((__REG) (I2C1 + 0x0C))
#define I2C1_DR	        ((__REG) (I2C1 + 0x10))
#define I2C1_SR1	    ((__REG) (I2C1 + 0x14))
#define I2C1_SR2	    ((__REG) (I2C1 + 0x18))
#define I2C1_CCR	    ((__REG) (I2C1 + 0x1C))
#define I2C1_TRISE	    ((__REG) (I2C1 + 0x20))
#define I2C1_FLTR	    ((__REG) (I2C1 + 0x24))

#endif