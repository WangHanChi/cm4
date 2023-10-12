#include "command.h"
#include "reg.h"
#include "string.h"
#include "usart.h"

/* use this to check is root or not */
uint8_t IsRoot;

void input_buffer_overflow_message(char str[])
{
    print_str("Follow command: \"");
    print_str(str);
    print_str("\"... is too long to process.\n");

    print_str("The maximum length of input is 64.");
}

void command_help()
{
    print_str("\n");
    print_str(
        "+------------------- command manual ------------------------+\n");
    print_str(
        "|   command   |                description                  |\n");
    print_str(
        "|-------------+---------------------------------------------|\n");
    print_str(
        "| help        | show command manual                         |\n");
    print_str(
        "| hello       | print (Hello world!) to the screen          |\n");
    print_str(
        "| reboot      | restart the board thought SCB_AIRCR         |\n");
    print_str(
        "| sudo su     | change to the root (PASSWORD is root)       |\n");
    print_str(
        "| exit        | change to the user                          |\n");
    print_str(
        "| lscpu       | show the information for the board          |\n");
    print_str(
        "+-----------------------------------------------------------+\n");
    print_str("\n");
}

void command_hello()
{
    print_str("Hello world!\n");
}

void command_not_found(char *str)
{
    print_str("Error: command ");
    print_str(str);
    print_str(" not found, try <help>\n");
}

void command_reboot()
{
    if (IsRoot) {
        print_str("Rebooting ...\n\n\n\n");
        (*SCB_AIRCR) = 0x05FA0004;
    } else {
        print_str("Error: Permission denied\n");
        print_str("Try again in root\n");
    }
}

void command_sudo_su()
{
    char password[80];
    for (int i = 0; i < 3; ++i) {
        print_str("[sudo] password for hank:\n");
        scan_str(password);
        if (!strcmp(password, "root")) {
            IsRoot = 1;
            return;
        } else
            print_str("Sorry, try again.\n");
    }
    print_str("You have entered the wrong password three times,\n");
    print_str("please try again later\n");
}

void command_lscpu()
{
    print_str("Architecture:\t\tARMv7-M\n");
    print_str("  CPU op-mode(s):\t32-bit\n");
    print_str("  Address sizes:\t32 bits physical\n");
    print_str("  Byte Order:\t\tLittle Endian\n");
    print_str("Vendor ID:\t\tSTMicroelectronics\n");
    print_str("  Model name:\t\tNUCLEO-F429ZI (Cortex-M4)\n");
    print_str("\n");
}

void command_exit()
{
    if (IsRoot) {
        print_str("exit\n");
        IsRoot = 0;
    } else
        print_str("You are already not root\n");
}

void command_clear()
{
    print_str("\33[2J\n");
}