#pragma once

#include "gpio.h"
#include "rtc.h"
#include "timer.h"
#include "usart.h"
#include "xprintf.h"

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

BaseType_t xHigherPriorityTaskWoken = pdFALSE;
