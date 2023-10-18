#pragma once

#include "reg.h"

#define systick_init()          \
    do {                        \
        *SYSTICK_RVR = 8000000; \
        *SYSTICK_CVR = 0;       \
        *SYSTICK_CSR = 0x07;    \
    } while (0)
