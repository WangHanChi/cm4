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

#define USE_MUTEX

#ifdef USE_MUTEX
/* Declare a variable of type xSemaphoreHandle.  This is used to reference the
mutex type semaphore that is used to ensure mutual exclusive access to UART. */
xSemaphoreHandle xMutex;
#endif

char message[250] = {0};

/* The function that uses a mutex to control access to standard out. */
static void prvNewPrintString(const portCHAR *pcString)
{
#ifdef USE_MUTEX
    /* The semaphore is created before the scheduler is started so already
    exists by the time this task executes.

    Attempt to take the semaphore, blocking indefinitely if the mutex is not
    available immediately.  The call to xSemaphoreTake() will only return when
    the semaphore has been successfully obtained so there is no need to check
    the return value.  If any other delay period was used then the code must
    check that xSemaphoreTake() returns pdTRUE before accessing the resource (in
    this case standard out. */
    xSemaphoreTake(xMutex, portMAX_DELAY);
    {
#endif
        /* The following line will only execute once the semaphore has been
        successfully obtained - so standard out can be accessed freely. */
        xprintf("%s\n", pcString);
#ifdef USE_MUTEX
    }
    xSemaphoreGive(xMutex);
#endif
}

/* The task to be created.  Two instances of this task are created. */
static void prvPrintTask(void *pvParameters)
{
    for (;;) {
        /* Print out the string using the newly defined function. */
        prvNewPrintString((char *) pvParameters);

        /* Wait a pseudo random time.  Note that rand() is not necessarily
        re-entrant, but in this case it does not really matter as the code does
        not care what value is returned.  In a more secure application a version
        of rand() that is known to be re-entrant should be used - or calls to
        rand() should be protected using a critical section. */
        vTaskDelay(100 & 0XF);
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

    DELAY(1000);
    xprintf("\rThis is freeRTOS mutex example\n");

#ifdef USE_MUTEX
    /* Before a semaphore is used it must be explicitly created.  In this
    example a mutex type semaphore is created. */
    xMutex = xSemaphoreCreateMutex();
#endif

#ifdef USE_MUTEX
    /* Check the mutex was created successfully. */
    if ((xMutex != NULL)) {
#endif

        /* Create two instances of the tasks that attempt to write stdout.  The
        string they attempt to write is passed in as the task parameter.  The
        tasks are created at different priorities so some pre-emption will
        occur. */
        xTaskCreate(prvPrintTask, "Print1", 240,
                    "Task 1 ******************************************\r\n", 1,
                    NULL);
        xTaskCreate(prvPrintTask, "Print2", 240,
                    "Task 2 ------------------------------------------\r\n", 2,
                    NULL);

        /* Start the scheduler so the created tasks start executing. */
        vTaskStartScheduler();
#ifdef USE_MUTEX
    } else
#endif
        for (;;)
            ; /* Should never get here */
    return 0;
}
