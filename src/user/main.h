#pragma once

#include "dac.h"
#include "exti.h"
#include "gpio.h"
#include "pi.h"
#include "rtc.h"
#include "timer.h"
#include "usart.h"
#include "xprintf.h"

#include "FreeRTOSConfig.h"

#define MAX_VOLT (33)
#define MAX_RPM (1050)

// clang-format off
const char banner[] = " \e[36;01m ===============================================================\e[0m\n"
                      "\r\e[33;01m  ###   #   #     #           ###   #   #  #####  #      #\n"
                      "\r #   #  #   #    ##          #   #  #   #  #      #      #\n"
                      "\r #      ## ##   # #          #      #   #  #      #      #\n"
                      "\r #      # # #  #  #   #####   ###   #####  ####   #      #\n"
                      "\r #      #   #  #####             #  #   #  #      #      #\n"
                      "\r #   #  #   #     #          #   #  #   #  #      #      #\n"
                      "\r  ###   #   #     #           ###   #   #  #####  #####  #####\n"
                      "\r \e[36;01m ==============================================================="
                      "\r\n\n"
                      "\r\e[32;01mThis is a mini shell for stm32f4. It is imitating UNIX like!\e[0m\n\n"
                      "\r\n";
// clang-format on

// Queue handle
QueueHandle_t command_queue = NULL;
QueueHandle_t uart_write_queue = NULL;

// task handles
TaskHandle_t xTaskHandleShell = NULL;
TaskHandle_t xTaskHandleCmd = NULL;
TaskHandle_t xTaskHandleWrite = NULL;
TaskHandle_t xspeedHandle = NULL;
TaskHandle_t xCtrlAlgoHandle = NULL;
TaskHandle_t xDACOutputHandle = NULL;

// check the time which control algo take
TickType_t xLastWakeTime = 0;

// for xTaskNotifyFromISR
BaseType_t xHigherPriorityTaskWoken = pdFALSE;

void exti_config();