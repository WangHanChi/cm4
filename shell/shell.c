#include "shell.h"
#include "command.h"
#include "string.h"
#include "usart.h"

extern uint8_t IsRoot;

void shell_start()
{
    char input[MAX_BUFFER_LENGTH];
    memset(input, 0, MAX_BUFFER_LENGTH);
    IsRoot = 0;
    // read input
    while (1) {
        // new line head
        if (IsRoot)
            print_str("root@stm32f4~# ");
        else
            print_str("hank@stm32f4~$ ");

        scan_str(input);
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
    else
        command_not_found(str);
}
