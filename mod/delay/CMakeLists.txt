cmake_minimum_required(VERSION 2.8)

# include as system helps avoid warnings in headers
include_directories(SYSTEM ${STM32_INCLUDE_DIR})

add_executable(delay main.c)
target_link_libraries(delay startup stm32)

list(APPEND TargetsList delay)
set(TargetsList "${TargetsList}" PARENT_SCOPE)

