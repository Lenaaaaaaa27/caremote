//
// Created by Lena on 22/11/2023.
// CRUD for configurations
//
#include<sqlite3.h>
#include"../../includes/define.h"

int create_configuration(int id_profile){

    sqlite3 *db;
    sqlite3_stmt *stmt;

    if(sqlite3_open("../caremote_db", &db) != SQLITE_OK)
        error_content(101);

    if (sqlite3_prepare_v2(db, "INSERT INTO configuration (name,"
                               "move_forward,"
                               "move_backward,"
                               "move_left,"
                               "move_right,"
                               "max_speed_first_step,"
                               "max_speed_second_step,"
                               "change_step_button,"
                               "id_profile)"
                               "VALUES (?,?,?,?,?,?,?,?,?)",
                               -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_bind_text(stmt, 1, "Nouvelle configuration", -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, "z", -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, "s", -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, "q", -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, "d", -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 6, 3.0);
    sqlite3_bind_double(stmt, 7, 6.0);
    sqlite3_bind_text(stmt, 8, "&", -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 9, id_profile);


    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

/**
@brief Cette fonction permet de get toutes les configurations.
Une allocation dynamique est faite il faut donc free la mémoire allouer
@return Retourne un tableau dynamique de type Configuration
@endcode
*/
Configuration * get_configurations(int id_profile){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    Configuration *array;
    int size = 0;

    if(sqlite3_open("../caremote_db", &db) != SQLITE_OK)
        error_content(101);

    char *req = "SELECT id, name, move_forward, move_backward, move_left, move_right, max_speed_first_step, max_speed_second_step, change_step_button, id_profile FROM configuration WHERE id_profile = ?";
    if (sqlite3_prepare_v2(db, req, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_bind_int(stmt, 1, id_profile);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        ++size;
    }

    array = malloc ((size+1) * sizeof(Configuration));
    size = 0;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        array[size].id = sqlite3_column_int(stmt, 0);
        strcpy(array[size].name, (const char *)sqlite3_column_text(stmt, 1));
        array[size].move_forward = (char) sqlite3_column_text(stmt, 2)[0];
        array[size].move_backward = (char) sqlite3_column_text(stmt, 3)[0];
        array[size].move_left = (char) sqlite3_column_text(stmt, 4)[0];
        array[size].move_right = (char)sqlite3_column_text(stmt, 5)[0];
        array[size].max_speed_first_step = sqlite3_column_double(stmt, 6);
        array[size].max_speed_second_step = sqlite3_column_double(stmt, 7);
        array[size].change_step_button = (char) sqlite3_column_text(stmt, 8)[0];
        array[size].id_profile = sqlite3_column_int(stmt, 9);
        ++size;
    }

    array[size].id = -1;
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return array;
}

Configuration get_configuration(int id_configuration){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    Configuration array;

    if(sqlite3_open("../caremote_db", &db) != SQLITE_OK)
        error_content(101);

    char *req = "SELECT id, name, move_forward, move_backward, move_left, move_right, max_speed_first_step, max_speed_second_step, change_step_button, id_profile FROM configuration WHERE id = ?";
    if (sqlite3_prepare_v2(db, req, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_bind_int(stmt, 1, id_configuration);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        array.id = sqlite3_column_int(stmt, 0);
        strcpy(array.name, (const char *) sqlite3_column_text(stmt, 1));
        array.move_forward = (char) sqlite3_column_text(stmt, 2)[0];
        array.move_backward = (char) sqlite3_column_text(stmt, 3)[0];
        array.move_left = (char) sqlite3_column_text(stmt, 4)[0];
        array.move_right = (char)sqlite3_column_text(stmt, 5)[0];
        array.max_speed_first_step = sqlite3_column_double(stmt, 6);
        array.max_speed_second_step = sqlite3_column_double(stmt, 7);
        array.change_step_button = (char) sqlite3_column_text(stmt, 8)[0];
        array.id_profile = sqlite3_column_int(stmt, 9);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return array;
}

int update_configuration(Configuration configuration) {

    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *sql = "UPDATE configuration SET name = ?, move_forward = ?, move_backward = ?, move_left = ?,"
                "move_right = ?, max_speed_first_step = ?, max_speed_second_step = ?, change_step_button = ? "
                "WHERE id = ?";

    if(sqlite3_open("../caremote_db", &db) != SQLITE_OK) {
        error_content(101);
        sqlite3_free(sql);
    }

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        error_content(101);
        sqlite3_free(sql);
        sqlite3_close(db);
        return 1;
    }

    sqlite3_bind_text(stmt, 1, configuration.name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, &configuration.move_forward, 1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, &configuration.move_backward, 1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, &configuration.move_left, 1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, &configuration.move_right, 1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 6, configuration.max_speed_first_step);
    sqlite3_bind_double(stmt, 7, configuration.max_speed_second_step);
    sqlite3_bind_text(stmt, 8, &configuration.change_step_button, 1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 9, configuration.id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}

int delete_configuration(int id_configuration) {
    sqlite3 *db;
    sqlite3_stmt *stmt;

    if(sqlite3_open("../caremote_db", &db) != SQLITE_OK)
        return 1;

    if (sqlite3_prepare_v2(db, "DELETE FROM configuration WHERE id = ?", -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_bind_int(stmt, 1, id_configuration);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}

