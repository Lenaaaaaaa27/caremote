//
// Created by Lena on 22/11/2023.
// CRUD for configurations
//
#include"../../includes/define.h"

int create_configuration(Configuration *configuration){

    sqlite3 *db;
    sqlite3_stmt *stmt;

    if(sqlite3_open("caremote_db", &db) != SQLITE_OK)
        error_content(101);

    if (sqlite3_prepare_v2(db, "INSERT INTO configuration (name,"
                               "move_forward,"
                               "move_backward,"
                               "move_left,"
                               "move_right,"
                               "speed_step,"
                               "change_step_button,"
                               "id_profile)"
                               "VALUES (?,?,?,?,?,?,?,?)",
                               -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_bind_text(stmt, 1, configuration->name, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, configuration->move_forward);
    sqlite3_bind_int(stmt, 3, configuration->move_backward);
    sqlite3_bind_int(stmt, 4, configuration->move_left);
    sqlite3_bind_int(stmt, 5, configuration->move_right);
    sqlite3_bind_int(stmt, 6, configuration->speed_step);
    sqlite3_bind_int(stmt, 7, configuration->change_step_button);
    sqlite3_bind_int(stmt, 8, configuration->id_profile);


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

    if(sqlite3_open("caremote_db", &db) != SQLITE_OK)
        error_content(101);

    char *req = "SELECT id, name, move_forward, move_backward, move_left, move_right, speed_step, change_step_button, id_profile FROM configuration WHERE id_profile = ? ORDER BY id DESC";
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
        array[size].move_forward = (char) sqlite3_column_int(stmt, 2);
        array[size].move_backward = (char) sqlite3_column_int(stmt, 3);
        array[size].move_left = (char) sqlite3_column_int(stmt, 4);
        array[size].move_right = (char) sqlite3_column_int(stmt, 5);
        array[size].speed_step = sqlite3_column_int(stmt, 6);
        array[size].change_step_button = (char) sqlite3_column_int(stmt, 7);
        array[size].id_profile = sqlite3_column_int(stmt, 8);
        ++size;
    }

    array[size].id = -1;
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return array;
}

boolean does_configuration_exist_with_id(int id_configuration) {
    sqlite3 *db;
    sqlite3_stmt *stmt;

    if (sqlite3_open("caremote_db", &db) != SQLITE_OK)
        error_content(101);

    char *req = "SELECT 1 FROM configuration WHERE id = ?";
    if (sqlite3_prepare_v2(db, req, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_bind_int(stmt, 1, id_configuration);

    int result = sqlite3_step(stmt);

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return (result == SQLITE_ROW);
}

Configuration get_configuration(int id_configuration){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    Configuration array;

    if(sqlite3_open("caremote_db", &db) != SQLITE_OK)
        error_content(101);

    char *req = "SELECT id, name, move_forward, move_backward, move_left, move_right, speed_step, change_step_button, id_profile FROM configuration WHERE id = ?";
    if (sqlite3_prepare_v2(db, req, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_bind_int(stmt, 1, id_configuration);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        array.id = sqlite3_column_int(stmt, 0);
        strcpy(array.name, (const char *) sqlite3_column_text(stmt, 1));
        array.move_forward = (char) sqlite3_column_int(stmt, 2);
        array.move_backward = (char) sqlite3_column_int(stmt, 3);
        array.move_left = (char) sqlite3_column_int(stmt, 4);
        array.move_right = (char)sqlite3_column_int(stmt, 5);
        array.speed_step = sqlite3_column_int(stmt, 6);
        array.change_step_button = (char) sqlite3_column_int(stmt, 7);
        array.id_profile = sqlite3_column_int(stmt, 8);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return array;
}

int update_configuration(Configuration configuration) {

    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *sql = "UPDATE configuration SET name = ?, move_forward = ?, move_backward = ?, move_left = ?,"
                "move_right = ?, speed_step = ?, change_step_button = ? "
                "WHERE id = ?";

    if(sqlite3_open("caremote_db", &db) != SQLITE_OK) {
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
    sqlite3_bind_int(stmt, 2, configuration.move_forward);
    sqlite3_bind_int(stmt, 3, configuration.move_backward);
    sqlite3_bind_int(stmt, 4, configuration.move_left);
    sqlite3_bind_int(stmt, 5, configuration.move_right);
    sqlite3_bind_int(stmt, 6, configuration.speed_step);
    sqlite3_bind_int(stmt, 7, configuration.change_step_button);
    sqlite3_bind_int(stmt, 8, configuration.id);

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

    if(sqlite3_open("caremote_db", &db) != SQLITE_OK)
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

