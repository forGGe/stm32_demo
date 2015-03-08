cmake_minimum_required(VERSION 2.8)

# include as system helps avoid warnings in headers
include_directories(SYSTEM ${STM32_INCLUDE_DIRS})

add_executable(uart_echo main.c)
target_link_libraries(uart_echo stm32_periph startup)

list(APPEND TargetsList uart_echo)
set(TargetsList "${TargetsList}" PARENT_SCOPE)
