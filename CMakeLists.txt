cmake_minimum_required(VERSION 3.26)
project(caremote C)

set(CMAKE_C_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

include_directories(${GTK4_INCLUDE_DIRS})
link_directories(${GTK4_LIBRARY_DIRS})

add_definitions(${GTK4_CFLAGS_OTHER})

add_executable(caremote
                src/control/control.c
                main.c)
target_link_libraries(caremote ${GTK4_LIBRARIES})

target_sources(caremote PRIVATE lib/sqlite-amalgamation-3440000/sqlite3.c)
target_include_directories(caremote PRIVATE lib/sqlite-amalgamation-3440000/)

# Ajouter le support de Winsock
if(WIN32)
    target_link_libraries(caremote ws2_32)
endif()
