#include <stdint.h>
#include "reg.h"
#include "shell.h"
#include "usart.h"

int main(void)
{
    usart_init();

    print_str(
        "\r===============================================================\n");
    print_str("\r  ###   #   #     #           ###   #   #  #####  #      #\n");
    print_str("\r #   #  #   #    ##          #   #  #   #  #      #      #\n");
    print_str("\r #      ## ##   # #          #      #   #  #      #      #\n");
    print_str("\r #      # # #  #  #   #####   ###   #####  ####   #      #\n");
    print_str("\r #      #   #  #####             #  #   #  #      #      #\n");
    print_str("\r #   #  #   #     #          #   #  #   #  #      #      #\n");
    print_str(
        "\r   ###   #   #     #           ###   #   #  #####  #####  #####\n");
    print_str(
        "\r===============================================================\n");
    print_str("\r\n");
    print_str(
        "\rThis is a mini shell for stm32f4. It is imitating UNIX like!\n");
    print_str("\r\n");


    shell_start();

    while (1)
        ;
    return 0;
}