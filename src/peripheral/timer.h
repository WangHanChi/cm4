#pragma once

void Tim2_config();
void Tim2_Start();

#define CTRL_TICK_PER_SECOND    (20)
#define CTRL_PERIOD_MS          (1000 / CTRL_TICK_PER_SECOND)
#define CTRL_PERIOD_SEC         (1 / CTRL_TICK_PER_SECOND)