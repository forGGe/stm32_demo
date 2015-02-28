cmake_minimum_required(VERSION 2.8)

add_executable(dumb main.c)
target_link_libraries(dumb startup)

list(APPEND TargetsList dumb)
set(TargetsList "${TargetsList}" PARENT_SCOPE)

