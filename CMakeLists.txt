cmake_minimum_required(VERSION 2.8)

# verbose mode
# set(CMAKE_VERBOSE_MAKEFILE on)

project(stm32demos)

# this one is important
set(CMAKE_SYSTEM_NAME Generic)
#this one not so much
set(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_C_LINKER arm-none-eabi-ld)

# where is the target environment
set(CMAKE_FIND_ROOT_PATH /usr/arm-none-eabi)

# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# avoid using any additional flags when linking with shared libraries
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

# set various flags for C\C++ compiler and linker
# warn flags goes here
set(CC_WARN_FLAGS "-Wall -Wextra -Wpedantic")
set(CXX_WARN_FLAGS "${CC_WARN_FLAGS} -Weffc++")

# optimization flags
set(CC_OPT_FLAGS "-O2")
set(CXX_OPT_FLAGS "${CC_OPT_FLAGS}")

# common flags for current platform
set(CC_PLATFORM_FLAGS "-mcpu=cortex-m4 -mthumb -fdata-sections -ffunction-sections -fno-common")
set(CXX_PLATFORM_FLAGS "${CC_PLATFORM_FLAGS}")

# extra flags
set(CC_EXTRA_FLAGS "-std=c99 -g3")
set(CXX_EXTRA_FLAGS "-std=c++11 -g3")

set(CMAKE_C_FLAGS
"${CMAKE_C_FLAGS} ${CC_PLATFORM_FLAGS} ${CC_OPT_FLAGS} ${CC_WARN_FLAGS} ${CC_EXTRA_FLAGS}")
set(CMAKE_CXX_FLAGS
"${CMAKE_CXX_FLAGS} ${CXX_PLATFORM_FLAGS} ${CXX_OPT_FLAGS} ${CC_WARN_FLAGS} ${CXX_EXTRA_FLAGS}")
set(CMAKE_C_LINK_FLAGS
"-T${CMAKE_CURRENT_SOURCE_DIR}/misc/stm32.ld -nostartfiles --gc-sections")
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_C_LINK_EXECUTABLE
"${CMAKE_C_LINKER} <OBJECTS> <CMAKE_C_LINK_FLAGS> <LINK_LIBRARIES> -o <TARGET>")

# set our custom list containing targets to be built
set(TargetsList)

# startup code (built as library to link with)
add_subdirectory(misc/startup)

# STM32 peripherial library (built as library to link with)
add_subdirectory(lib/stm32)

# modules itself
add_subdirectory(mod/delay)
add_subdirectory(mod/simple_led)

# generate custom binary targets for each module built y
foreach(Target ${TargetsList})
message("Configuring binary target for ${Target}")
    add_custom_target(${Target}.bin ALL
    COMMAND ${CMAKE_OBJCOPY} --output-format=binary ${CMAKE_CURRENT_BINARY_DIR}/mod/${Target}/${Target} ${Target}.bin
    DEPENDS ${Target}
    COMMENT "Making binary ${Target}"
    )

    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${Target}.bin)
endforeach(Target)
