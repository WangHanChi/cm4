# About this

These are simple practices about ARM-Cortex M4 processor.  
Most of them are learned on udemy and do some change.

## usart2tty
This project is to build the bridge between MCU and users.  
We can type in the console in STM32CubeIDE and send the message to
MCU by USART.

## hello-bare-metal  
This project is to do the same thing with usart2tty but using bare metal  
We can use `putty` to communicate with serial port.  
I also implement the `putty.py` which is the same function with `putty`.  
Here is the [learning note](https://hackmd.io/@wanghanchi/cm4-cm4hbm)  

## context-switch
This project is to do context switch and using usart to send which task is  
working. Here is the [learning note](https://hackmd.io/QHc9-LluQEqTrlHtnm2P1Q)  

