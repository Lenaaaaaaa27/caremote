cmake_minimum_required(VERSION 3.26)
project(caremote C)

set(CMAKE_C_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk+-3.0)

include_directories(${GTK4_INCLUDE_DIRS})
link_directories(${GTK4_LIBRARY_DIRS})

add_definitions(${GTK4_CFLAGS_OTHER})
add_executable(caremote main.c
        includes/define.h
        lib/sqlite-amalgamation-3440000/sqlite3.c
        src/GUI/gui.c
        src/bd_requests/config.c
        src/bd_requests/profile.c
        src/bd_requests/session.c
        src/Errors/fatal_errors.c
        includes/ui.h
        src/control/control.c
        src/connexion/connexion.c
        src/data/calculation.c
        src/Database.c
        src/files/config.c
        src/files/exports.c
        src/files/imports.c
)
target_link_libraries(caremote ${GTK4_LIBRARIES})


target_sources(caremote PRIVATE lib/sqlite-amalgamation-3440000/sqlite3.c)
target_include_directories(caremote PRIVATE lib/sqlite-amalgamation-3440000/)

set(CJSON_SOURCE_DIR lib/cJSON/)
add_library(cJSON STATIC
        ${CJSON_SOURCE_DIR}/cJSON.c
        ${CJSON_SOURCE_DIR}/cJSON_Utils.c
        # Ajoutez tous les autres fichiers source de cJSON ici
)
target_include_directories(cJSON PUBLIC ${CJSON_SOURCE_DIR})
target_link_libraries(caremote cJSON)


# Ajouter le support de Winsock
if(WIN32)
    target_link_libraries(caremote ws2_32)

    # Ajouter la bibliothèque XInput
    target_link_libraries(caremote -lXinput)
endif()
