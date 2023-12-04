//
// Created by arthu on 22/11/2023.
//

#include<sqlite3.h>
#include"../../includes/define.h"

int create_profile(char username[20]){

    sqlite3 *db;
    sqlite3_stmt *stmt;

    if(sqlite3_open("../caremote_db", &db) != SQLITE_OK)
        error_content(101);

    if (sqlite3_prepare_v2(db, "INSERT INTO profile (username) VALUES (?)", -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

/**
@brief Cette fonction permet de get tous les profils.
Une allocation dynamique est faite il faut donc free la mémoire allouer
@return Retourne un tableau de type Profile
@endcode
*/

Profile * get_profiles(){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    Profile *array;
    int size = 0;

    if(sqlite3_open("../caremote_db", &db) != SQLITE_OK)
        error_content(101);

    if (sqlite3_prepare_v2(db, "SELECT id,username FROM profile", -1, &stmt, NULL) != SQLITE_OK) {
        error_content(101);
        sqlite3_close(db);
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        ++size;
    }

    array = malloc((size+1) * sizeof(Profile));

    size = 0;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        array[size].id = atoi(sqlite3_column_text(stmt, 0));
        strcpy(array[size].username, sqlite3_column_text(stmt, 1));
        ++size;
    }

    array[size].id = -1;

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return array;
}

Profile get_profile(int id_profile){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    Profile profile;

    if(sqlite3_open("../caremote_db", &db) != SQLITE_OK)
        error_content(101);

    if (sqlite3_prepare_v2(db, "SELECT id, username FROM profile WHERE id = ?", -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_bind_int(stmt, 1, id_profile);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_close(db);
        error_content(101);
    }

    profile.id = atoi(sqlite3_column_text(stmt, 0));
    strcpy(profile.username, sqlite3_column_text(stmt, 1));

    sqlite3_close(db);
    sqlite3_finalize(stmt);


    return profile;
}

int update_profile(int id, char username[30]){
    sqlite3 *db;
    sqlite3_stmt *stmt;

    if(sqlite3_open("../caremote_db", &db) != SQLITE_OK)
        error_content(101);

    if (sqlite3_prepare_v2(db, "UPDATE profile SET username = ? WHERE id = ?", -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, id);


    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}


int delete_profile(int id){
    sqlite3 *db;
    sqlite3_stmt *stmt;

    if(sqlite3_open("../caremote_db", &db) != SQLITE_OK)
        error_content(101);

    if (sqlite3_prepare_v2(db, "DELETE FROM profile WHERE id = ?", -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        error_content(101);
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        error_content(101);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

