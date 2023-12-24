#include "rtc.h"

int rtc_initialize()
{
    /* enable the PWR clock */
    RCC->APB1ENR |= (RCC_APB1ENR_PWREN);

    /* allow access to RTC */
    PWR->CR |= (PWR_CR_DBP);

    /* enable LSI clock for RTC and wait for ready flag */
    RCC->CSR |= (RCC_CSR_LSION);
    while (!(RCC->CSR & RCC_CSR_LSIRDY))
        ;

    /* select LSI as clock source */
    RCC->BDCR |= (RCC_BDCR_RTCSEL_1);

    /* enable RTC */
    RCC->BDCR |= (RCC_BDCR_RTCEN);

    /* enter the "key" to unlock write protection
     * Reference : RM90 (stm32f2429zit6_reference_manual) Page.803
     * First  -> write "0xCA" to RTC_WPR
     * Second -> write "0x53" to RTC_WPR
     */
    RTC->WPR |= (0xCA);
    RTC->WPR |= (0x53);

    /* set init bit and wait for ready flag */
    RTC->ISR |= (RTC_ISR_INIT);
    while (!(RTC->ISR & RTC_ISR_INITF))
        ;

    /* adjust prescaler values for RTC ti obtain 1 Hz */
    RTC->PRER |= (0xF9 << RTC_PRER_PREDIV_S_Pos);
    RTC->PRER |= (0x7F << RTC_PRER_PREDIV_A_Pos);

    /* write time and date values */
    RTC->TR = 0x135950;  // set hour as 13:57:00
    RTC->DR = 0x23D202;  // set DATE as 23/12/02

    /* set BYPSHAD bit
     * Reference : RM90 (stm32f2429zit6_reference_manual) Page.816
     * System reset 0x0000 0000 when BYPSHAD = 0.
     * Not affected when BYPSHAD = 1
     */
    RTC->CR |= (RTC_CR_BYPSHAD);

    /* clear init bit */
    RTC->ISR &= ~(RTC_ISR_INIT);

    /* disable access to RTC registers */
    PWR->CR &= ~(PWR_CR_DBP);

    return 1;
}

int rtc_deinit()
{
    /* disable RTC */
    RCC->BDCR &= ~(RCC_BDCR_RTCEN);

    return 1;
}

int rtc_gettime(RTCTIME *rtc)
{
    /* get second */
    rtc->sec = ((RTC->TR & 0x7f) >> 4) * 10 + (RTC->TR & 0xf);
    /* get minute */
    rtc->min = ((RTC->TR & 0x7f00) >> 8);
    rtc->min = (((rtc->min & 0x7f) >> 4) * 10) + ((rtc->min & 0xf));
    /* get hour */
    rtc->hour = ((RTC->TR & 0x7f0000) >> 16);
    rtc->hour = (((rtc->hour & 0x7f) >> 4) * 10) + (rtc->hour & 0xf);

    /* get year */
    rtc->year = ((RTC->DR >> 20) * 10) + ((RTC->DR >> 16) & 0xf);
    /* get month */
    rtc->month = ((RTC->DR >> 12) & 1) * 10 + ((RTC->DR >> 8) & 0xf);
    /* get day */
    rtc->mday = ((RTC->DR >> 4) & 3) + (RTC->DR & 0xf);

    return 1;
}

int rtc_settime(const RTCTIME *rtc)
{
    /* enable the PWR clock */
    RCC->APB1ENR |= (RCC_APB1ENR_PWREN);

    /* allow access to RTC */
    PWR->CR |= (PWR_CR_DBP);

    /* enable LSI clock for RTC and wait for ready flag */
    RCC->CSR |= (RCC_CSR_LSION);
    while (!(RCC->CSR & RCC_CSR_LSIRDY))
        ;

    /* select LSI as clock source */
    RCC->BDCR |= (RCC_BDCR_RTCSEL_1);

    /* enable RTC */
    RCC->BDCR |= (RCC_BDCR_RTCEN);

    /* enter the "key" to unlock write protection
     * Reference : RM90 (stm32f2429zit6_reference_manual) Page.803
     * First  -> write "0xCA" to RTC_WPR
     * Second -> write "0x53" to RTC_WPR
     */
    RTC->WPR |= (0xCA);
    RTC->WPR |= (0x53);

    /* set init bit and wait for ready flag */
    RTC->ISR |= (RTC_ISR_INIT);
    while (!(RTC->ISR & RTC_ISR_INITF))
        ;

    /* adjust prescaler values for RTC ti obtain 1 Hz */
    RTC->PRER |= (0xF9 << RTC_PRER_PREDIV_S_Pos);
    RTC->PRER |= (0x7F << RTC_PRER_PREDIV_A_Pos);

    /* clear RTC_TR */
    RTC->TR = 0;
    /* set HT bit */
    uint8_t bit = rtc->hour / 10;
    RTC->TR |= (bit << RTC_TR_HT_Pos);
    /* set HU bit */
    bit = rtc->hour % 10;
    RTC->TR |= (bit << RTC_TR_HU_Pos);
    /* set MNT bit */
    bit = rtc->min / 10;
    RTC->TR |= (bit << RTC_TR_MNT_Pos);
    /* set MNU bit */
    bit = rtc->min % 10;
    RTC->TR |= (bit << RTC_TR_MNU_Pos);
    /* set ST bit */
    bit = rtc->sec / 10;
    RTC->TR |= (bit << RTC_TR_ST_Pos);
    /* set SU bit */
    bit = rtc->sec % 10;
    RTC->TR |= (bit << RTC_TR_SU_Pos);

    /* cleat RTC_DR */
    RTC->DR = 0;
    /* set YT bit */
    bit = rtc->year / 10;
    RTC->DR |= (bit << RTC_DR_YT_Pos);
    /* set YU bit */
    bit = rtc->year % 10;
    RTC->DR |= (bit << RTC_DR_YU_Pos);
    /* set WDU bit */
    RTC->DR |= (rtc->wday << RTC_DR_WDU_Pos);
    /* set MT bit */
    bit = rtc->month / 10;
    RTC->DR |= (bit << RTC_DR_MT_Pos);
    /* set MU bit */
    bit = rtc->month % 10;
    RTC->DR |= (bit << RTC_DR_MU_Pos);
    /* set DT bit */
    bit = rtc->mday / 10;
    RTC->DR |= (bit << RTC_DR_DT_Pos);
    /* set DU bit */
    bit = rtc->mday % 10;
    RTC->DR |= (bit << RTC_DR_DU_Pos);

    /* set BYPSHAD bit
     * Reference : RM90 (stm32f2429zit6_reference_manual) Page.816
     * System reset 0x0000 0000 when BYPSHAD = 0.
     * Not affected when BYPSHAD = 1
     */
    RTC->CR |= (RTC_CR_BYPSHAD);

    /* clear init bit */
    RTC->ISR &= ~(RTC_ISR_INIT);

    /* disable access to RTC registers */
    PWR->CR &= ~(PWR_CR_DBP);

    return 1;
}
