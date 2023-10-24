# cross compiler setting
CROSS_COMPILE ?= arm-none-eabi-
CC		:= $(CROSS_COMPILE)gcc
AS		:= $(CROSS_COMPILE)as
SZ		:= $(CROSS_COMPILE)size
DUMP	:= $(CROSS_COMPILE)objdump
GDB		:= $(CROSS_COMPILE)gdb

# import common.mk
include ../mk/common.mk

TARGET = loader
OUT = build

# C source files
C_SOURCE = 			\
src/loader.c		\
src/usart.c			\
src/startup.c		\
src/string.c		\
src/math.c

C_INCLUDES = -Iinc

# linker script files
LDSCRIPT = src/loader.ld

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

.PHONY: loader clean

loader: clean $(OUT)/$(TARGET)

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

clean:
	$(RM) build/*
