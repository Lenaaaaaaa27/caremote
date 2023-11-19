cmake_minimum_required(VERSION 3.26)
project(caremote C)

set(CMAKE_C_STANDARD 17)

add_executable(caremote main.c)

target_sources(caremote PRIVATE lib/sqlite-amalgamation-3440000/sqlite3.c)
target_include_directories(caremote PRIVATE lib/sqlite-amalgamation-3440000/)