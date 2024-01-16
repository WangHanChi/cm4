#pragma once

#include "stm32f429xx.h"

void DAC_config();
void DAC_SetValue(uint32_t value);
void DAC_Reset();