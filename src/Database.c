//
// Created by Léna on 19/11/2023.
//

#include "../../includes/define.h"

// To create the database
int create_database(Setting *settings) {
    sqlite3 *db;
    Configuration config;
    char *error_message = 0;

    if (sqlite3_open("../caremote_db", &db) != SQLITE_OK) {
        error_content(101);
    }

    const char *create_table_sql =
            "CREATE TABLE profile("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "username VARCHAR(20)"
            ");";

    if (sqlite3_exec(db, create_table_sql, 0, 0, &error_message) != SQLITE_OK) {
        sqlite3_free(error_message);
        sqlite3_close(db);
        error_content(101);
    }

    const char *create_table_2_sql =
            "CREATE TABLE configuration("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "name VARCHAR(30),"
            "move_forward CHAR,"
            "move_backward CHAR,"
            "move_left CHAR,"
            "move_right CHAR,"
            "speed_step INTEGER,"
            "change_step_button CHAR,"
            "id_profile INTEGER,"
            "FOREIGN KEY (id_profile) REFERENCES profile(id) ON DELETE CASCADE"
            ");";

    if (sqlite3_exec(db, create_table_2_sql, 0, 0, &error_message) != SQLITE_OK) {
        sqlite3_free(error_message);
        sqlite3_close(db);
        error_content(101);
    }

    const char *create_table_3_sql =
            "CREATE TABLE session ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "name VARCHAR(40),"
            "duration INTEGER,"
            "distance INTEGER,"
            "average_speed DOUBLE,"
            "time_start TEXT,"
            "id_configuration INTEGER,"
            "id_profile INTEGER,"
            "FOREIGN KEY (id_configuration) REFERENCES configuration(id) ON DELETE CASCADE,"
            "FOREIGN KEY (id_profile) REFERENCES profile(id) ON DELETE CASCADE"
            ")";

    if (sqlite3_exec(db, create_table_3_sql, 0, 0, &error_message) != SQLITE_OK) {
        sqlite3_free(error_message);
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_close(db);

    strcpy(config.name, settings->configuration.name);
    config.move_forward = settings->configuration.move_forward;
    config.move_backward = settings->configuration.move_backward;
    config.move_left = settings->configuration.move_left;
    config.move_right = settings->configuration.move_right;
    config.speed_step = settings->configuration.speed_step;
    config.change_step_button = settings->configuration.change_step_button;
    config.id_profile = 1;

    create_profile(settings->profileUsername);
    create_configuration(&config);
    return 0;
}

// Verify if the database already exists
int verification_database(){
    if (access("../caremote_db", F_OK) == -1) return 1;
    return 0;
}
