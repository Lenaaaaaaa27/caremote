//
// Created by Arthur on 23/11/2023.
// Goal : CRUD for session
//

#include<sqlite3.h>
#include<stdio.h>
#include"../../includes/define.h"

int create_session(char name[40], int duration, int distance, double max_speed, double average_speed, char time_start[20], int id_configuration, int id_profile){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;

    if(sqlite3_open("../caremote_db", &db) != SQLITE_OK)
        error_content(101);

    const char *sql_requests = "INSERT INTO session (name, duration, distance, max_speed, average_speed, time_start, id_configuration, id_profile) VALUES (?,?,?,?,?,?,?,?)";
    rc = sqlite3_prepare_v2(db, sql_requests, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, duration);
    sqlite3_bind_int(stmt, 3, distance);
    sqlite3_bind_double(stmt, 4, max_speed);
    sqlite3_bind_double(stmt, 5, average_speed);
    sqlite3_bind_text(stmt, 6, time_start, 20, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 7, id_configuration);
    sqlite3_bind_int(stmt, 8, id_profile);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}

int get_sessions_by_profile_id(int id_profile){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;

    if(sqlite3_open("../caremote_db", &db) != SQLITE_OK)
        error_content(101);

    const char *sql_requests = "SELECT id, name, duration, distance, max_speed, average_speed, time_start, id_configuration, id_profile FROM session WHERE id_profile = ?";
    rc = sqlite3_prepare_v2(db, sql_requests, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_bind_int(stmt, 1, id_profile);

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        sqlite3_column_int(stmt, 0);
        sqlite3_column_text(stmt, 1);
        sqlite3_column_int(stmt, 2);
        sqlite3_column_int(stmt, 3);
        sqlite3_column_double(stmt, 4);
        sqlite3_column_double(stmt, 5);
        sqlite3_column_text(stmt, 6);
        sqlite3_column_int(stmt, 7);
        sqlite3_column_int(stmt, 8);
    }

    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        printf("Erreur lors de l'execution de la requete : %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}

int get_sessions_by_id(int id){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;

    if(sqlite3_open("../caremote_db", &db) != SQLITE_OK)
        error_content(101);

    const char *sql_requests = "SELECT id, name, duration, max_speed, average_speed, time_start, id_configuration, id_profile FROM session WHERE id = ?";
    rc = sqlite3_prepare_v2(db, sql_requests, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_bind_int(stmt, 1, id);

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int session_id = sqlite3_column_int(stmt, 0);
        const char *name = (const char *)sqlite3_column_text(stmt, 1);
        int duration = sqlite3_column_int(stmt, 2);
        double max_speed = sqlite3_column_int(stmt, 3);
        double average_speed = sqlite3_column_double(stmt, 4);
        const char *time_start_str = (const char *)sqlite3_column_text(stmt, 5);
        int id_configuration = sqlite3_column_int(stmt, 6);
        int id_profile = sqlite3_column_int(stmt, 7);
    }

    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}

int update_session(char name[40], int duration, double max_speed, double average_speed, char time_start[20], int id_configuration, int id_profile, int id){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;

    if(sqlite3_open("../caremote_db", &db) != SQLITE_OK)
        error_content(101);

    char *sql_request = "UPDATE session SET name = ?, duration = ?, max_speed = ?, average_speed = ?, time_start = ?, id_configuration = ?, id_profile = ? WHERE id = ?";
    if (sqlite3_prepare_v2(db,sql_request, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, duration);
    sqlite3_bind_double(stmt, 3, max_speed);
    sqlite3_bind_double(stmt, 4, average_speed);
    sqlite3_bind_text(stmt, 5, time_start, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, id_configuration);
    sqlite3_bind_int(stmt, 7, id_profile);
    sqlite3_bind_int(stmt, 8, id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_close(db);
        error_content(101);
    } else {
        printf("Valeur mise à jour\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}

int update_session_name(char name[40], int id){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;

    if(sqlite3_open("../caremote_db", &db) != SQLITE_OK)
        error_content(101);

    if (sqlite3_prepare_v2(db,"UPDATE session SET name = ? WHERE id = ?", -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_close(db);
        error_content(101);
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}

int delete_session(int id){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;

    if(sqlite3_open("../caremote_db", &db) != SQLITE_OK)
        error_content(101);

    char *sql_requests = "DELETE FROM session WHERE id = ?";
    if (sqlite3_prepare_v2(db, sql_requests, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

  //  create_session("Session 1", 6005, 400, 3.4, 2.3, "2023-04-23, 22:34:23", 1, 1);

