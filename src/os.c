#include <stdint.h>
#include "reg.h"
#include "shell.h"
#include "usart.h"

int main(void)
{
    usart_init();

    DELAY(50);
    // clang-format off
    print_str(
        " \e[36;01m ===============================================================\e[0m\n");
    print_str("\r\e[33;01m  ###   #   #     #           ###   #   #  #####  #      #\n");
    print_str("\r #   #  #   #    ##          #   #  #   #  #      #      #\n");
    print_str("\r #      ## ##   # #          #      #   #  #      #      #\n");
    print_str("\r #      # # #  #  #   #####   ###   #####  ####   #      #\n");
    print_str("\r #      #   #  #####             #  #   #  #      #      #\n");
    print_str("\r #   #  #   #     #          #   #  #   #  #      #      #\n");
    print_str(
        "\r   ###   #   #     #           ###   #   #  #####  #####  #####\n");
    print_str(
        "\r \e[36;01m ===============================================================");
    print_str("\r\n\n");
    print_str(
        "\r\e[32;01mThis is a mini shell for stm32f4. It is imitating UNIX like!\e[0m\n\n");
    print_str("\r\n");
    // clang-format on

    shell_start();

    while (1)
        ;
    return 0;
}
