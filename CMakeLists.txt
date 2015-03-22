cmake_minimum_required(VERSION 2.8)

# verbose mode
# set(CMAKE_VERBOSE_MAKEFILE on)

project(stm32demos)

# include compiler definitions
include(compiler/compiler.cmake)

# set our custom list containing targets to be built
set(TargetsList)

# startup code (built as library to link with)
add_subdirectory(misc/startup)

# declare that we're using STM3240 processor
add_definitions("-DSTM32F40_41xxx")
# STM32 peripherial library (built as library to link with)
add_subdirectory(lib/stm32_periph)

# modules itself
add_subdirectory(mod/delay)
add_subdirectory(mod/simple_led)
add_subdirectory(mod/uart_echo)

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
