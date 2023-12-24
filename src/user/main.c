/* C runtime library */
#include <stdio.h>
#include <string.h>

/* freeRTOS library */
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"

/* user library */
#include "main.h"
#include "shell.h"



static void vShellTask(void *pvParameters)
{
    char *ptransmit;
    char input[MAX_BUFFER_LENGTH] = {0};

    while (1) {
        ptransmit = USER_INPUT;
        xQueueSend(uart_write_queue, &ptransmit, portMAX_DELAY);
        xgets(input, MAX_BUFFER_LENGTH);
        ptransmit = input;
        xQueueSend(uart_write_queue, &ptransmit, portMAX_DELAY);
        memset(input, 0, MAX_BUFFER_LENGTH);
        xTaskNotify(xTaskHandleCmd, 0, eNoAction);
        // lets wait here until someone notifies.
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
    }
}

static void vCmdTask(void *pvParameters)
{
    while (1) {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
        xTaskNotify(xTaskHandleShell, 0, eNoAction);
    }
}

static void vWrtieTask(void *params)
{
    char *pData = NULL;
    while (1) {
        xQueueReceive(uart_write_queue, &pData, portMAX_DELAY);
        xprintf("%s", pData);
    }
}


int main(void)
{
    /* initial peripheral */
    rtc_initialize();
    /* Enable UART1 and attach it to xprintf module for console */
    usart3_init(115200);
    xdev_out(usart3_putc);
    xdev_in(usart3_getc);

    DELAY(10000);
    xprintf("%s", banner);

    // create the write queue
    uart_write_queue = xQueueCreate(10, sizeof(char*));
    // create command queue
    command_queue = xQueueCreate(10, sizeof(char*));

    // check that the queue is created successfully
    if ((command_queue != NULL) && (uart_write_queue != NULL)) {
        // lets create task-1
        xTaskCreate(vShellTask, "TASK-Shell", 500, NULL, 1, &xTaskHandleShell);

        xTaskCreate(vCmdTask, "TASK-Cmd", 500, NULL, 1, &xTaskHandleCmd);

        xTaskCreate(vWrtieTask, "TASK-Write", 500, NULL, 1, &xTaskHandleWrite);


        // freeRTOS start
        vTaskStartScheduler();
    } else {
        xprintf("Queue creation failed!\n");
    }


    for (;;)
        ;
    // should be never get here

    return 0;
}
