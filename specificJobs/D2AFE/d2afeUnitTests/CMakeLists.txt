cmake_minimum_required(VERSION 3.15)
project(d2afeUnitTests)

set(CMAKE_CXX_STANDARD 14)

add_executable(d2afeUnitTests eeprom_write_block_test.cpp)