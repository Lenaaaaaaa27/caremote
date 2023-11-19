//
// Created by Léna on 19/11/2023.
//


#include <stdio.h>
#include <sqlite3.h>
#include <unistd.h>

// To create the database
int create_database() {
    sqlite3 *db;
    char *error_message = 0;

    int rc = sqlite3_open("../caremote_db", &db);


    if (rc != SQLITE_OK) {
        fprintf(stderr, "Impossible d'ouvrir/creer la base de donnees : %s\n", sqlite3_errmsg(db));
        return 1;
    }


    const char *create_table_sql =
            "CREATE TABLE profile("
            "id INTEGER PRIMARY KEY,"
            "username VARCHAR(20)"
            ");";


    rc = sqlite3_exec(db, create_table_sql, 0, 0, &error_message);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la creation de la table : %s\n", error_message);
        sqlite3_free(error_message);
        sqlite3_close(db);
        return 1;
    }

    const char *create_table_2_sql =
            "CREATE TABLE configuration("
            "id INTEGER PRIMARY KEY,"
            "name VARCHAR(30),"
            "move_forward CHAR,"
            "move_backward CHAR,"
            "move_left CHAR,"
            "move_right CHAR,"
            "max_speed_first_step FLOAT,"
            "max_speed_second_stp FLOAT,"
            "id_profile INTEGER,"
            "FOREIGN KEY (id_profile) REFERENCES profile(id)"
            ");";

    rc = sqlite3_exec(db, create_table_2_sql, 0, 0, &error_message);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la creation de la table : %s\n", error_message);
        sqlite3_free(error_message);
        sqlite3_close(db);
        return 1;
    }

    const char *create_table_3_sql =
            "CREATE TABLE session ("
            "id INTEGER PRIMARY KEY,"
            "name VARCHAR(40),"
            "duration INTEGER,"
            "max_speed FLOAT,"
            "average_speed FLOAT,"
            "time_start TEXT,"
            "id_configuration INTEGER,"
            "id_profile INTEGER,"
            "FOREIGN KEY (id_configuration) REFERENCES configuration(id),"
            "FOREIGN KEY (id_profile) REFERENCES profile(id)"
            ")";

    rc = sqlite3_exec(db, create_table_3_sql, 0, 0, &error_message);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la creation de la table : %s\n", error_message);
        sqlite3_free(error_message);
        sqlite3_close(db);
        return 1;
    }

    sqlite3_close(db);

    printf("La base de donnees a ete cree.\n");

    return 0;
}


int verification_database(){
    const char *nom_fichier = "../caremote_db";

int verification_database(){
    if (access("../caremote_db", F_OK) == -1) return 0;
    return 1;
}
