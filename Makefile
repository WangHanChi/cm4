# cross compiler setting
CROSS_COMPILE ?= arm-none-eabi-
CC		:= $(CROSS_COMPILE)gcc
AS		:= $(CROSS_COMPILE)as
SZ		:= $(CROSS_COMPILE)size
DUMP	:= $(CROSS_COMPILE)objdump
GDB		:= $(CROSS_COMPILE)gdb

# import common.mk
include mk/common.mk

TARGET = os
OUT = build
BOOTLOADER = bootloader/build/loader

# C source files
C_SOURCE = 			\
src/os.c			\
src/command.c		\
src/shell.c			\
src/string.c		\
src/math.c			\
src/usart.c			\
src/startup.c		\
src/handler.c

C_INCLUDES = -Iinc

# linker script files
LDSCRIPT = src/os.ld

VPATH = $(dir $(C_SOURCES))

# name the object files
OBJS = $(patsubst %.c, $(OUT)/%.o, $(notdir $(C_SOURCE)))

VPATH = src
vpath %.c

# toolchain options
MCU = -mcpu=cortex-m4 -mfloat-abi=softfp -mfpu=fpv4-sp-d16
CFLAGS = -fno-common -ffreestanding -O0 -gdwarf-2 -g3 $(MCU) \
-Wall -Werror -mcpu=cortex-m4 -mthumb -nostartfiles $(C_INCLUDES)

LDFLAGS = $(MCU) -specs=nano.specs -Wl,-T$(LDSCRIPT) -lc -lm -lnosys -nostartfiles

.PHONY: upload clean help os

help:
	@echo ""
	@echo "+------------------- command manual ------------------------+"
	@echo "|   command   |                description                  |"
	@echo "|-------------+---------------------------------------------|"
	@echo "| help        | show command manual                         |"
	@echo "| os          | compile, link and assemble the OS files     |"
	@echo "| loader      | compile, link and assemble the Bootloader   |"
	@echo "| upload      | upload image and bootloader to the board    |"
	@echo "| debug       | use openocd to enter debug mode             |"
	@echo "| clean       | remove the intermediate objects and image   |"
	@echo "| format      | use clang format to make code tidy          |"
	@echo "| screen      | use screen to communicate with the board    |"
	@echo "+-----------------------------------------------------------+"
	@echo ""

os: clean $(OUT)/$(TARGET)

# compile
$(OUT)/%.o: %.c
	$(VECHO) "  [CC]\t$@\n"
	$(Q)$(CC) -c $(CFLAGS) $< -o $@

# link
$(OUT)/$(TARGET): $(OUT) $(OBJS)
	$(VECHO) "  [LD]\t$@\n"
	$(Q)$(CC) $(OBJS) $(LDFLAGS) -o $@
	$(Q)echo "===================================================="
	$(Q)$(SZ) $@
	$(Q)echo "===================================================="
	$(Q)$(call pass)

# mkdir dir
$(OUT):
	mkdir $@

upload: os loader
	openocd -f interface/stlink.cfg -f board/st_nucleo_f4.cfg -c " program $(OUT)/$(TARGET) exit"
	openocd -f interface/stlink.cfg -f board/st_nucleo_f4.cfg -c " program $(BOOTLOADER) exit reset"

loader:
	make -C bootloader/

debug:
	openocd -f board/st_nucleo_f4.cfg

screen:
	gnome-terminal -- bash -c "screen /dev/ttyACM0 115200"

format:
	clang-format -i $(C_SOURCE)

clean:
	$(RM) $(OUT)/*.o build/$(TARGET) bootloader/build/*