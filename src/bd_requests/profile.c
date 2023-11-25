//
// Created by arthu on 22/11/2023.
//

#include<sqlite3.h>
#include<stdio.h>

// CREATE profile

int create_profile(char username[30]){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    if(sqlite3_open("../caremote_db", &db) != SQLITE_OK)
        return 1;

    //To prepare the request
    if (sqlite3_prepare_v2(db, "INSERT INTO profile (username) VALUES (?)", -1, &stmt, NULL) != SQLITE_OK) {
        printf("Erreur de preparation de la requete : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    // To execute the request

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Erreur lors de l'execution de la requete : %s\n", sqlite3_errmsg(db));
    } else {
        printf("Nouvel utilisateur ajoute\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;

    return 0;
}

// READ profile table function

int get_profiles(){
    sqlite3 *db;
    sqlite3_stmt *stmt;

    if(sqlite3_open("../caremote_db", &db) != SQLITE_OK)
        return 1;

    if (sqlite3_prepare_v2(db, "SELECT username FROM profile", -1, &stmt, NULL) != SQLITE_OK) {
        printf("Erreur de preparation de la requete : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *nom = (const char *)sqlite3_column_text(stmt, 0);
        printf("%s\n", nom);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}

int get_profile(int id_profile){
    sqlite3 *db;
    sqlite3_stmt *stmt;

    if(sqlite3_open("../caremote_db", &db) != SQLITE_OK)
        return 1;

    if (sqlite3_prepare_v2(db, "SELECT username FROM profile WHERE id = ?", -1, &stmt, NULL) != SQLITE_OK) {
        printf("Erreur de preparation de la requete : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    sqlite3_bind_int(stmt, 1, id_profile);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *nom = (const char *)sqlite3_column_text(stmt, 0);
        printf("Le profil utilise est celui de %s\n", nom);
    } else {
        printf("Erreur lors de l'execution de la requete : %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}

// UPDATE username in PROFIL table

int update_profile(int id, char username[30]){
    sqlite3 *db;
    sqlite3_stmt *stmt;

    if(sqlite3_open("../caremote_db", &db) != SQLITE_OK)
        return 1;

    if (sqlite3_prepare_v2(db, "UPDATE profile SET username = ? WHERE id = ?", -1, &stmt, NULL) != SQLITE_OK) {
        printf("Erreur de preparation de la requete : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, id);


    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Erreur lors de l'execution de la requete : %s\n", sqlite3_errmsg(db));
    } else {
        printf("Valeur mise a jour\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}

// DELETE profile

int delete_profile(int id){
    sqlite3 *db;
    sqlite3_stmt *stmt;

    if(sqlite3_open("../caremote_db", &db) != SQLITE_OK)
        return 1;

    if (sqlite3_prepare_v2(db, "DELETE FROM profile WHERE id = ?", -1, &stmt, NULL) != SQLITE_OK) {
        printf("Erreur de preparation de la requete : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Erreur lors de l'exécution de la requête : %s\n", sqlite3_errmsg(db));
    } else {
        printf("Profil supprime\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;

}

