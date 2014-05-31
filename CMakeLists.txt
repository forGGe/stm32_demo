CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(stm32demos)

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_C_COMPILER arm-none-eabi-gcc)
SET(CMAKE_CXX_COMPILER arm-none-eabi-g++)
SET(CMAKE_C_LINKER arm-none-eabi-ld)
SET(CMAKE_C_LINK_EXECUTABLE "${CMAKE_C_LINKER} <OBJECTS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <LINK_LIBRARIES> -o <TARGET>")
SET(CMAKE_OBJCOPY arm-none-eabi-objcopy)

SET(CMAKE_FIND_ROOT_PATH /usr/arm-none-eabi)
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m4 -mthumb -fdata-sections -ffunction-sections -fno-common -g3 -O0")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=cortex-m4 -mthumb -fdata-sections -ffunction-sections -fno-common")
SET(CMAKE_EXE_LINKER_FLAGS "-T${CMAKE_CURRENT_SOURCE_DIR}/misc/stm32.ld -nostartfiles --gc-sections")
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

# Libraries and startup

ADD_SUBDIRECTORY(lib/stm32 stm32)
ADD_SUBDIRECTORY(misc/startup startup)

# Modules

ADD_SUBDIRECTORY(mod/simple_led simple_led)
ADD_SUBDIRECTORY(mod/dumb dumb)

# Custom targets for modules

ADD_CUSTOM_TARGET(simple_led.bin ALL
COMMAND ${CMAKE_OBJCOPY} --output-format=binary ${CMAKE_CURRENT_BINARY_DIR}/simple_led/simple_led simple_led.bin
DEPENDS simple_led
COMMENT "Making binary simple_led.bin"
)

ADD_CUSTOM_TARGET(dumb.bin ALL
COMMAND ${CMAKE_OBJCOPY} --output-format=binary ${CMAKE_CURRENT_BINARY_DIR}/dumb/dumb dumb.bin
DEPENDS dumb
COMMENT "Making binary dumb.bin"
)

SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES simple_led.bin)
SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES dumb.bin)
