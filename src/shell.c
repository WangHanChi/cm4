#include "shell.h"
#include "command.h"
#include "string.h"
#include "usart.h"
#include "systick.h"

/* define in command.c */
extern uint8_t IsRoot;
/* define in command.c */
extern uint32_t HOUR, MIN, SEC;
/* define in command.c */
extern uint8_t IsIRQ;

void shell_start()
{
    char input[MAX_BUFFER_LENGTH];
    memset(input, 0, MAX_BUFFER_LENGTH);
    IsRoot = IsIRQ = HOUR = MIN = SEC = 0;
    /* enable the systick for reload value 8 MHz */
    systick_init();
    // read input
    while (1) {
        // new line head
        if (IsRoot)
            print_str("root@cm4~# ");
        else
            print_str("\e[32;01mhank@cm4\e[0m:\e[34;01m~$\e[0m ");

        int num = scan_str(input);
        strtrim(input, num);
        parse(input);
    }
}

void parse(char str[])
{
    if (!strcmp(str, "help"))
        command_help();
    else if (!strcmp(str, "hello"))
        command_hello();
    else if (!strcmp(str, "reboot"))
        command_reboot();
    else if (!strcmp(str, "sudo su"))
        command_sudo_su();
    else if (!strcmp(str, "lscpu"))
        command_lscpu();
    else if (!strcmp(str, "exit"))
        command_exit();
    else if (!strcmp(str, "clear"))
        command_clear();
    else if (!strcmp(str, "date"))
        command_date();
    else if (!strcmp(str, "irq"))
        command_irq();
    else if (!strcmp(str, "dirq"))
        command_dirq();
    else
        command_not_found(str);
}
