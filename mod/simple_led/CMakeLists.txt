cmake_minimum_required(VERSION 2.8)

# include as system helps avoid warnings in headers
include_directories(SYSTEM ${STM32_INCLUDE_DIRS})

add_executable(simple_led main.c)
target_link_libraries(simple_led stm32_periph startup)

list(APPEND TargetsList simple_led)
set(TargetsList "${TargetsList}" PARENT_SCOPE)
