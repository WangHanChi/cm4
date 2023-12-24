#pragma once

typedef void (*func)(char[]);

#define MAX_BUFFER_LENGTH 80
#define MESSAGE_LEN 1024

char USER_INPUT[] = "\e[32;01mhank@cm4\e[0m:\e[34;01m~$\e[0m ";
char ROOT_INPUT[] = "root@cm4~# ";

void select_cmd(char *input, char message[]);
