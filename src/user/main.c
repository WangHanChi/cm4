/* C runtime library */
#include <math.h>
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
float pulse_A = 0;
float pre_pulse_A = 0;
float speed_A = 0;
uint8_t display_speed = 0;
int count = 0;
static uint32_t DAC_Output_value = 0;
static uint32_t target_speed = 0;
static long integral = 0;

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

void __attribute__((interrupt)) TIM2_IRQHandler(void)
{
    // get tick count
    xLastWakeTime = xTaskGetTickCount();
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xTaskNotifyFromISR(xspeedHandle, 0, eNoAction, &xHigherPriorityTaskWoken);

    /* Clear UIF update interrupt flag */
    TIM2->SR &= ~(TIM_SR_UIF);
}

static void vspeedTask(void *params)
{
    char *ptransmit = NULL;
    while (1) {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
        speed_A = 60 * (pulse_A - pre_pulse_A) * (20.0) / 1000.0 / (1.785);
        if (display_speed) {
            xsprintf(message, "|%d| -> |%d| RPM\n\r", count++, (int) speed_A);
            ptransmit = message;
            xQueueSend(uart_write_queue, &ptransmit, portMAX_DELAY);
        }
        pre_pulse_A = pulse_A;
        xTaskNotify(xCtrlAlgoHandle, 0, eNoAction);
    }
}

static void vCtrlAlgoTask(void *params)
{
    while (1) {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

        // Control algorithm begin
        // Calculate the error (difference between target and current speed)
        float error = target_speed - speed_A;
        xprintf("err:%d\n\r", (int)error);
        // Update the integral term (simple accumulation)
        integral += error;
        
        // if(integral > 500){
        //     integral /= 2;
        // }

        float new_speed = KP * error + KI * integral + speed_A;

        if(new_speed > 1000)
            new_speed = 1000;
        
        /*
         *  Voltage to Speed table
         * --------------------------------------
         * DAC    ->  Driver   ->   Speed
         * 3.4 V      8 V           1000 (RPM)
         * 3.0 V      6 V           800  (RPM)
         * 2.6 V      4 V           533  (RPM)
         * 2.1 V      1.5 V         Cannot spin
         * --------------------------------------
         */
        float driver_volt = (new_speed / 3200) * 24;
        // xprintf("1 -> driver_volt:%d\n\r", (int)driver_volt);
        driver_volt = 0.225 * driver_volt + 1.7;
        // xprintf("2 -> driver_volt:%d\n\r", (int)driver_volt);
        DAC_Output_value = ((driver_volt * 4096)) / 3.2;

        // // Limit the output to a reasonable range
        if (DAC_Output_value > 4096) {
            DAC_Output_value = 4096;
        } else if (DAC_Output_value < 2600) {
            DAC_Output_value = 2600;
        }

        // Control algorithm end
        xTaskNotify(xDACOutputHandle, 0, eNoAction);
    }
}

static void vDACOutputTask(void *params)
{
    char *ptransmit = NULL;

    while (1) {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
        DAC_SetValue((uint32_t) DAC_Output_value);

        /* check task can be done in limited time */
        TickType_t checktime = xTaskGetTickCount();

        /* Uncommon this line to assert the total task time */
        // configASSERT(((checktime - xLastWakeTime) < CTRL_PERIOD_MS));

        /* Uncommon this line to watch get the total time */
        // xprintf("The total speed : %u\n\r", checktime - xLastWakeTime);

        /* The current method : To Warn User */
        do {
            if (((checktime - xLastWakeTime) < CTRL_PERIOD_MS))
                break;
            else {
                xsprintf(message,
                         "\e[31;01mWarning :  The calculation takes too long: "
                         "%u ticks\e[0m\n\r",
                         (checktime - xLastWakeTime));
                ptransmit = message;
                xQueueSend(uart_write_queue, &ptransmit, portMAX_DELAY);
            }
        } while (0);
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

        xTaskCreate(vspeedTask, "TASK-GetSpeed", 500, NULL, 1, &xspeedHandle);

        xTaskCreate(vCtrlAlgoTask, "TASK-CalculateSpeed", 500, NULL, 1,
                    &xCtrlAlgoHandle);

        xTaskCreate(vDACOutputTask, "TASK-DACOutput", 500, NULL, 1,
                    &xDACOutputHandle);

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
    int len = MESSAGE_LEN > sizeof(ret) ? sizeof(ret) : MESSAGE_LEN;

    strncpy(message, ret, len);
}

void command_hello(char message[])
{
    char ret[] = "Hello world!\n\r";
    int len = MESSAGE_LEN > sizeof(ret) ? sizeof(ret) : MESSAGE_LEN;

    strncpy(message, ret, len);
}

void command_not_found(char message[])
{
    char ret[] = "Error: command not found, try <help>\n\r";
    int len = MESSAGE_LEN > sizeof(ret) ? sizeof(ret) : MESSAGE_LEN;

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

    int len = MESSAGE_LEN > sizeof(ret) ? sizeof(ret) : MESSAGE_LEN;

    strncpy(message, ret, len);
}

void command_clear(char message[])
{
    char ret[] = "\r\n\33[2J";

    int len = MESSAGE_LEN > sizeof(ret) ? sizeof(ret) : MESSAGE_LEN;

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
    while (1) {
        xprintf("Please enter the speed you want ? (0 ~ 1050 RPM)\n\r> ");
        memset(input, 0, MAX_BUFFER_LENGTH);
        xgets(input, MAX_BUFFER_LENGTH);
        char *tmp = input;
        int ret =
            xatoi(&tmp, (long *) &target_speed); /* 0:Failed, 1:Successful */

        if ((ret != 1) || (target_speed > MAX_RPM) || (target_speed < 0)) {
            xprintf("Invaliad input, speed should be in 0 ~ %d\n\r", MAX_RPM);
        } else
            break;
    }


    xprintf("!!! Remember Enter < Ctrl+A and H > to record !!! \n\r");

    do {
        xprintf("Do you want to display the speed per ticks ? (y/n)\n\r> ");
        memset(input, 0, MAX_BUFFER_LENGTH);
        xgets(input, MAX_BUFFER_LENGTH);
        if (!strncmp(input, "y", 1) && strlen(input) == 1)
            display_speed = 1;
    } while (0);

    xprintf("Set the speed to be %d\n\r", target_speed);
    xprintf(
        "If want to stop control motor, reset the speed, enter < quit "
        ">\n\r");

    /* Enable GPIO D port */
    (RCC->AHB1ENR) |= (1 << 3);
    /* Set intput for PD3 */
    (GPIOD->MODER) &= ~(0b11 << 6);
    /* Set intput for PD4 */
    (GPIOD->MODER) &= ~(0b11 << 8);
    /* set pull-up */
    (GPIOD->PUPDR) |= (0b10 << 6);
    /* set pull-up */
    (GPIOD->PUPDR) |= (0b10 << 8);

    /* Enable GPIO B port */
    (RCC->AHB1ENR) |= (1 << 1);
    /* Set General purpose output for PB7, PB14 */
    (GPIOB->MODER) |= (1 << 14) | (1 << 28);
    /* Light up PB7 */
    (GPIOB->BSRR) |= (1 << 7);

    count = 0;

    xprintf("--------------------------------------DataStart\n\r");
    exti_config();
    Tim2_config();
    DAC_config();
    Tim2_Start();

    float target_volt = (target_speed + 1200) / 666;
    uint32_t DAC_Output_value = ((4096 * target_volt) * 10) / MAX_VOLT;
    DAC_SetValue((uint32_t) DAC_Output_value);

    while (1) {
        memset(input, 0, MAX_BUFFER_LENGTH);
        while (xgets(input, MAX_BUFFER_LENGTH)) {
            /* turn off pmdc control */
            if (!strncmp(input, "quit", 4)) {
                // deinit DAC GPIOB GPIOD
                DAC_Reset();
                RCC->APB1ENR &= ~(RCC_APB1ENR_TIM2EN);
                RCC->AHB1ENR &= ~(1 << 3);
                RCC->APB1ENR &= ~(1 << 29);
                xprintf("Stop to control motor ...\n\r");
                return;
            }

            /* change the speed */
            char *pInput = input;
            long change_speed = 0;
            if (xatoi(&pInput, (long *) &change_speed)) {
                if ((float) (change_speed > MAX_RPM) ||
                    (float) (change_speed < 0))
                    xprintf("Invaliad input, speed should be in 0 ~ %d\n\r",
                            MAX_RPM);
                else {
                    target_speed = change_speed;
                }
            }
        }
    }
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