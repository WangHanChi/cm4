#pragma once

#include "stm32f429xx.h"

#define _RTC_TDIF	+9.0	/* TW = UTC+8.0 */

typedef struct {
	uint16_t	year;	/* 1970..2106 */
	uint8_t		month;	/* 1..12 */
	uint8_t		mday;	/* 1..31 */
	uint8_t		hour;	/* 0..23 */
	uint8_t		min;	/* 0..59 */
	uint8_t		sec;	/* 0..59 */
	uint8_t		wday;	/* 0..6 (Sun..Sat) */
} RTCTIME;

int rtc_initialize (void);				/* Initialize RTC */
int rtc_deinit(void);                   /* DeInitialize RTC */
int rtc_gettime (RTCTIME* rtc);			/* Get time */
int rtc_settime (const RTCTIME* rtc);	/* Set time */