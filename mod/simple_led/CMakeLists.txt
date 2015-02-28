cmake_minimum_required(VERSION 2.8)

include_directories(${STM32_INCLUDE_DIR})

add_executable(simple_led main.c)
target_link_libraries(simple_led stm32 startup)

list(APPEND TargetsList simple_led)
set(TargetsList "${TargetsList}" PARENT_SCOPE)
