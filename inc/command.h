#pragma once
#include <stdint.h>

void input_buffer_overflow_message(char str[]);
void command_help();
void command_hello();
void command_not_found(char *);
void command_reboot();
void command_sudo_su();
void command_lscpu();
void command_exit();
void command_clear();