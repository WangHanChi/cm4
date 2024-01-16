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

char message[MESSAGE_LEN] = {0};
char input[MAX_BUFFER_LENGTH] = {0};


static void vShellTask(void *pvParameters)
{
    char *ptransmit;

    while (1) {
        memset(input, 0, MAX_BUFFER_LENGTH);
        ptransmit = USER_INPUT;
        xQueueSend(uart_write_queue, &ptransmit, portMAX_DELAY);
        xgets(input, MAX_BUFFER_LENGTH);
        ptransmit = input;
        xQueueSend(command_queue, &ptransmit, portMAX_DELAY);
        xTaskNotify(xTaskHandleCmd, 0, eNoAction);
        // lets wait here until someone notifies.
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
    }
}

static void vCmdTask(void *pvParameters)
{
    char *pData = NULL;
    char *ptransmit = NULL;
    while (1) {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
        xQueueReceive(command_queue, &pData, portMAX_DELAY);
        select_cmd(pData, message);
        ptransmit = message;
        xQueueSend(uart_write_queue, &ptransmit, portMAX_DELAY);
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
    uart_write_queue = xQueueCreate(10, sizeof(char *));
    // create command queue
    command_queue = xQueueCreate(10, sizeof(char *));

    // check that the queue is created successfully
    if ((command_queue != NULL) && (uart_write_queue != NULL)) {
        xTaskCreate(vShellTask, "TASK-Shell", 500, NULL, 1, &xTaskHandleShell);

        xTaskCreate(vCmdTask, "TASK-Cmd", 500, NULL, 1, &xTaskHandleCmd);

        xTaskCreate(vWrtieTask, "TASK-Write", 500, NULL, 1, &xTaskHandleWrite);

        // freeRTOS start
        vTaskStartScheduler();
    } else {
        xprintf("Queue creation failed!\n");
    }

    // should be never get here
    return 0;
}

void command_help(char message[])
{
    // clang-format off
    char ret[] = "\n"
    "+------------------- command manual ------------------------+\n\r"
    "|   command   |                description                  |\n\r"
    "|-------------+---------------------------------------------|\n\r"
    "| help        | show command manual                         |\n\r"
    "| hello       | print (Hello world!) to the screen          |\n\r"
    "| reboot      | restart the board thought SCB_AIRCR         |\n\r"
    "| lscpu       | show the information for the board          |\n\r"
    "| date        | show the information for the time           |\n\r"
    "| pmdc        | start to control PMDC                       |\n\r"
    "+-----------------------------------------------------------+\n\r"
    "\n\r";
    // clang-format on
    int len = MESSAGE_LEN > strlen(ret) ? strlen(ret) : MESSAGE_LEN;

    strncpy(message, ret, len);
}

void command_hello(char message[])
{
    char ret[] = "Hello world!\n\r";
    int len = MESSAGE_LEN > strlen(ret) ? strlen(ret) : MESSAGE_LEN;

    strncpy(message, ret, len);
}

void command_not_found(char message[])
{
    char ret[] = "Error: command not found, try <help>\n\r";
    int len = MESSAGE_LEN > strlen(ret) ? strlen(ret) : MESSAGE_LEN;

    strncpy(message, ret, len);
}

void command_reboot(char message[])
{
    DELAY(30);
    SCB->AIRCR = 0x05FA0004;
}

void command_lscpu(char message[])
{
    char ret[] =
        "Architecture:\t\tARMv7-M\n\r"
        "  CPU op-mode(s):\t32-bit\n\r"
        "  Address sizes:\t32 bits physical\n\r"
        "  Byte Order:\t\tLittle Endian\n\r"
        "Vendor ID:\t\tSTMicroelectronics\n\r"
        "  Model name:\t\tNUCLEO-F429ZI (Cortex-M4)\n\r"
        "\n\r";

    int len = MESSAGE_LEN > strlen(ret) ? strlen(ret) : MESSAGE_LEN;

    strncpy(message, ret, len);
}

void command_clear(char message[])
{
    char ret[] = "\r\n\33[2J";

    int len = MESSAGE_LEN > strlen(ret) ? strlen(ret) : MESSAGE_LEN;

    strncpy(message, ret, len);
}

void command_date(char message[])
{
    RTCTIME t;
    rtc_gettime(&t);

    sprintf(message, "%d-%d-%d | %d-%d-%d\n\r", t.year, t.month, t.mday, t.hour,
            t.min, t.sec);
}

void command_pmdc(char message[])
{
    // WIP
    while (1) {
        memset(input, 0, MAX_BUFFER_LENGTH);
        xgets(input, MAX_BUFFER_LENGTH);
        if (!strncmp(input, "quit", 4))
            break;
    }
    xprintf("Stop to control motor ...\n\r");
}

/*
 * commands list
 * (command, code)
 */
#define CMD_LIST            \
    _(NOT_FOUND, not_found) \
    _(HELP, help)           \
    _(HELLO, hello)         \
    _(REBOOT, reboot)       \
    _(LSCPU, lscpu)         \
    _(CLEAR, clear)         \
    _(DATE, date)           \
    _(PMDC, pmdc)

typedef enum {
#define _(CMD, cmd) CMD_##CMD,
    CMD_LIST
#undef _
        CMD_MAX
} cmd_code_list;

const char *cmd_str[] = {
#define _(CMD, cmd) #CMD,
    CMD_LIST
#undef _
};

const func func_table[] = {
#define _(CMD, cmd) command_##cmd,
    CMD_LIST
#undef _
};

int get_cmd_index(const char *cmd)
{
    for (int i = 0; i < CMD_MAX; ++i) {
        if (strcasecmp((cmd_str[i]), cmd) == 0) {
            return i;
        }
    }

    return -1;  // means doesn't find the correspond index
}

void select_cmd(char *input, char message[])
{
    memset(message, 0, MESSAGE_LEN);
    int cmd_index = get_cmd_index(input);

    if (cmd_index == -1) {
        cmd_index = 0;
    }

    func_table[cmd_index](message);
}