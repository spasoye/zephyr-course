cmake_minimum_required(VERSION 3.20.0)

list(APPEND BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(ZEPHYR_EXTRA_MODULES ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/spas_driver)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(l07_shell)

target_sources(app PRIVATE src/main.cpp)
