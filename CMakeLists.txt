cmake_minimum_required(VERSION 3.12)

project(FreeRTOS_Project C ASM)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# cross compiler setting
set(CROSS_COMPILE "arm-none-eabi-")
set(CMAKE_C_COMPILER ${CROSS_COMPILE}gcc)
set(CMAKE_ASM_COMPILER ${CROSS_COMPILE}as)
set(CMAKE_SIZE ${CROSS_COMPILE}size)
set(CMAKE_OBJDUMP ${CROSS_COMPILE}objdump)
set(CMAKE_GDB ${CROSS_COMPILE}gdb)

# SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2")
# SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

########################### BOOTLOADER PART ############################
set(BOOTLOADER_SOURCE
    bootloader/src/loader.c
    bootloader/src/usart.c
    bootloader/src/startup.c
    bootloader/src/xprintf.c
)

set(BOOTLOADER_LDSCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/bootloader/src/loader.ld)
set(BOOTLOADER_TARGET loader)
set(CMSIS cmsis)
add_executable(${BOOTLOADER_TARGET} ${BOOTLOADER_SOURCE})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/bootloader/inc
    ${CMSIS}
)

set_target_properties(${BOOTLOADER_TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY bootloader
    LINK_FLAGS "-T${BOOTLOADER_LDSCRIPT}"
)

########################### KERNERL PART ###########################
set(TARGET main)
set(OUT kernel)

# set file path
set(USER src/user)
set(FREERTOS_CONFIG src/freertos_config)
set(PERIPHERAL src/peripheral)
set(STARTUP startup)
set(FREERTOS freertos)

# Add source files
set(SOURCE_FILE 
    ${USER}/main.c
    ${USER}/syscall.c
    ${USER}/xprintf.c
)

set(PERIPHERAL_FILE
    ${PERIPHERAL}/usart.c
    ${PERIPHERAL}/timer.c
    ${PERIPHERAL}/rtc.c
    ${PERIPHERAL}/exti.c
    ${PERIPHERAL}/dac.c
)

set(C_SOURCES
    ${STARTUP}/startup.c
    ${FREERTOS}/portable/GCC/ARM_CM4F/port.c
    ${FREERTOS}/portable/MemMang/heap_4.c
    ${FREERTOS}/croutine.c
    ${FREERTOS}/list.c
    ${FREERTOS}/queue.c
    ${FREERTOS}/stream_buffer.c
    ${FREERTOS}/tasks.c
    ${FREERTOS}/timers.c
    ${SOURCE_FILE}
    ${PERIPHERAL_FILE}
)

# Include directories
include_directories(
    ${PERIPHERAL}
    ${USER}
    ${CMSIS}
    ${FREERTOS}/portable/GCC/ARM_CM4F
    ${FREERTOS}/include
    ${FREERTOS_CONFIG}
)

# Kernel linker script
set(KERNEL_LDSCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/${STARTUP}/link.ld)

# compiler and linker options
set(MCU_FLAGS "-mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16")
set(CMAKE_C_FLAGS "${MCU_FLAGS} -fno-common -ffreestanding -O1 -gdwarf-2 -g3 -Wall -Werror -mcpu=cortex-m4 -mthumb -nostartfiles")
set(CMAKE_ASM_FLAGS "${MCU_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${MCU_FLAGS} -specs=nano.specs -Wl,-lc,-lm,-lnosys -nostartfiles")

# Create the executable
add_executable(${TARGET} ${C_SOURCES})

# Set the output directory
set_target_properties(${TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${OUT}
    LINK_FLAGS "-T${KERNEL_LDSCRIPT}"
)

# custom target for upload
add_custom_target(upload
    DEPENDS ${TARGET} ${BOOTLOADER_TARGET}
    COMMAND openocd -f interface/stlink.cfg -f board/st_nucleo_f4.cfg -c "program ${OUT}/${TARGET} exit"
    COMMAND openocd -f interface/stlink.cfg -f board/st_nucleo_f4.cfg -c "program bootloader/${BOOTLOADER_TARGET} exit reset"
    COMMENT "Upload image and bootloader to the board"
)

# custom target for screen
add_custom_target(screen
    # also can use "screen -L -Logfile temp.txt /dev/ttyACM0 115200" to save logfile
    # or you can in the screen and type Ctrl+A and input "H" to record !
    COMMAND gnome-terminal -- bash -c "screen /dev/ttyACM0 115200"
)

# custom target for format
add_custom_target(format
    COMMAND clang-format -i ${USER}/*.c ${USER}/*.h ${PERIPHERAL}/*.c bootloader/src/*.c
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# custom target for copy 
add_custom_target(copy
    COMMAND python3 script/data_process.py
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# custom target for plot 
add_custom_target(plot
    COMMAND python3 script/plot.py
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# custom target for debug
add_custom_target(debug
    COMMAND openocd -f board/st_nucleo_f4.cfg
)
