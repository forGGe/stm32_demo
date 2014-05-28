#!/bin/bash

arm-none-eabi-gcc -I. -c -fno-common -O0 -g -mcpu=cortex-m3 -mthumb main.c
arm-none-eabi-ld -Tstm32.ld -nostartfiles -o main.elf main.o
arm-none-eabi-objcopy -Obinary main.elf main.bin