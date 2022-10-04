cmake_minimum_required(VERSION 3.0)
project(ps4j C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories("/usr/lib/jvm/java-18-openjdk/include/")
include_directories("/usr/lib/jvm/java-18-openjdk/include/linux/")
include_directories("/usr/local/include/include/")

add_library(ps4j SHARED ps4j.c)
target_link_libraries(ps4j "/usr/local/lib/libpocketsphinx.a")
