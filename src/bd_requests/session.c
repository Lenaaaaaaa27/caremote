//
// Created by Arthur on 23/11/2023.
// Goal : CRUD for session
// Edit Session Struct + CRUD new version : Lena
//

#include"../../includes/define.h"

int create_session(Session session){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;

    if(sqlite3_open("caremote_db", &db) != SQLITE_OK)
        error_content(101);

    const char *sql_requests = "INSERT INTO session (name, duration, distance, average_speed, time_start, id_configuration, id_profile) VALUES (?,?,?,?,?,?,?)";
    rc = sqlite3_prepare_v2(db, sql_requests, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_bind_text(stmt, 1, session.name, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, session.duration);
    sqlite3_bind_int(stmt, 3, session.distance);
    sqlite3_bind_double(stmt, 4, session.average_speed);
    sqlite3_bind_text(stmt, 5, session.time_start, 20, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, session.id_configuration);
    sqlite3_bind_int(stmt, 7, session.id_profile);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
/**
@brief This function gets all the sessions of a user.
A dynamic allocation is made, so free the allocated memory.
@return Returns a dynamic Session array
@endcode
*/

Session * get_sessions_by_profile_id(int id_profile){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    Session * array;
    int size = 0;


    if(sqlite3_open("caremote_db", &db) != SQLITE_OK)
        error_content(101);

    char *sql_requests = "SELECT id, name, duration, distance, average_speed, time_start, id_configuration, id_profile FROM session WHERE id_profile = ? ORDER BY id DESC";

    if (sqlite3_prepare_v2(db, sql_requests, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_bind_int(stmt, 1, id_profile);

    while ((sqlite3_step(stmt)) == SQLITE_ROW) {
        ++size;
    }

    array = malloc((size+1) *sizeof(Session));
    size = 0;
    while ((sqlite3_step(stmt)) == SQLITE_ROW) {
        array[size].id = sqlite3_column_int(stmt, 0);
        strcpy(array[size].name, (char *) sqlite3_column_text(stmt, 1));
        array[size].duration = sqlite3_column_int(stmt, 2);
        array[size].distance = sqlite3_column_int(stmt, 3);
        array[size].average_speed = sqlite3_column_double(stmt, 4);
        strcpy(array[size].time_start, (char *) sqlite3_column_text(stmt, 5));
        array[size].id_configuration = sqlite3_column_int(stmt, 6);
        array[size].id_profile = sqlite3_column_int(stmt, 7);
        ++size;
    }

    array[size].id = -1;
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return array;
}

Session get_session(int id_session){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    Session array;
    int rc;

    if(sqlite3_open("caremote_db", &db) != SQLITE_OK)
        error_content(101);

    char *sql_requests = "SELECT id, name, duration, distance, average_speed, time_start, id_configuration, id_profile FROM session WHERE id = ?";
    rc = sqlite3_prepare_v2(db, sql_requests, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_bind_int(stmt, 1, id_session);

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {

        array.id = id_session;
        strcpy(array.name, (char *) sqlite3_column_text(stmt, 1));
        array.duration = sqlite3_column_int(stmt, 2);
        array.distance = sqlite3_column_int(stmt, 3);
        array.average_speed = sqlite3_column_double(stmt, 4);
        strcpy(array.time_start, (char *) sqlite3_column_text(stmt, 5));
        array.id_configuration = sqlite3_column_int(stmt, 6);
        array.id_profile = sqlite3_column_int(stmt, 7);
    }

    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return array;
}

int update_session(Session session){
    sqlite3 *db;
    sqlite3_stmt *stmt;

    if(sqlite3_open("caremote_db", &db) != SQLITE_OK)
        error_content(101);

    char *sql_request = "UPDATE session SET name = ?, duration = ?, distance = ?, average_speed = ?, time_start = ?, id_configuration = ?, id_profile = ? WHERE id = ?";
    if (sqlite3_prepare_v2(db,sql_request, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_bind_text(stmt, 1, session.name, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, session.duration);
    sqlite3_bind_int(stmt, 3, session.distance);
    sqlite3_bind_double(stmt, 4, session.average_speed);
    sqlite3_bind_text(stmt, 5, session.time_start, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, session.id_configuration);
    sqlite3_bind_int(stmt, 7, session.id_profile);
    sqlite3_bind_int(stmt, 8, session.id);

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

    if(sqlite3_open("caremote_db", &db) != SQLITE_OK)
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


