//
// Created by Lena on 22/11/2023.
// CRUD for configurations
//
#include<string.h>
#include<sqlite3.h>
#include<stdio.h>
#include <stdlib.h>

typedef struct {
    char *name;
    char *move_forward;
    char *move_backward;
    char *move_left;
    char *move_right;
    char *max_speed_first_step;
    char *max_speed_second_step;
    char *change_step_button;

} Configuration;

int create_configuration(int id_profile){

    sqlite3 *db;
    sqlite3_stmt *stmt;

    if(sqlite3_open("../caremote_db", &db) != SQLITE_OK)
        return 1;

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
        printf("Erreur de preparation de la requete : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
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

    // To execute the request

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Erreur lors de l'execution de la requete : %s\n", sqlite3_errmsg(db));
    } else {
        printf("Nouvelle config ajoutee\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

int get_configurations(int id_profile){
    sqlite3 *db;
    sqlite3_stmt *stmt;

    if(sqlite3_open("../caremote_db", &db) != SQLITE_OK)
        return 1;
    char *req = "SELECT name, move_forward, move_backward, move_left, move_right, max_speed_first_step, max_speed_second_step, change_step_button FROM configuration WHERE id_profile = ?";
    if (sqlite3_prepare_v2(db, req, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Erreur de preparation de la requete : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    sqlite3_bind_int(stmt, 1, id_profile);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *nom = (const char *)sqlite3_column_text(stmt, 0);
        const char *move_forward = (const char *)sqlite3_column_text(stmt, 1);
        const char *move_backward = (const char *)sqlite3_column_text(stmt, 2);
        const char *move_left = (const char *)sqlite3_column_text(stmt, 3);
        const char *move_right = (const char *)sqlite3_column_text(stmt, 4);
        double max_speed_first_step = sqlite3_column_double(stmt, 5);
        double max_speed_second_step = sqlite3_column_double(stmt, 6);
        const char *change_step_button = (const char *)sqlite3_column_text(stmt, 7);

        printf("Name: %s\n", nom);
        printf("Move Forward: %s\n", move_forward);
        printf("Move Backward: %s\n", move_backward);
        printf("Move Left: %s\n", move_left);
        printf("Move Right: %s\n", move_right);
        printf("Max Speed First Step: %d\n", max_speed_first_step);
        printf("Max Speed Second Step: %d\n", max_speed_second_step);
        printf("Change Step Button: %s\n", change_step_button);
        printf("\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}

int get_configuration(int id_configuration){
    sqlite3 *db;
    sqlite3_stmt *stmt;

    if(sqlite3_open("../caremote_db", &db) != SQLITE_OK)
        return 1;
    char *req = "SELECT name, move_forward, move_backward, move_left, move_right, max_speed_first_step, max_speed_second_step, change_step_button FROM configuration WHERE id = ?";
    if (sqlite3_prepare_v2(db, req, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Erreur de preparation de la requete : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    sqlite3_bind_int(stmt, 1, id_configuration);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *nom = (const char *)sqlite3_column_text(stmt, 0);
        const char *move_forward = (const char *)sqlite3_column_text(stmt, 1);
        const char *move_backward = (const char *)sqlite3_column_text(stmt, 2);
        const char *move_left = (const char *)sqlite3_column_text(stmt, 3);
        const char *move_right = (const char *)sqlite3_column_text(stmt, 4);
        double max_speed_first_step = sqlite3_column_double(stmt, 5);
        double max_speed_second_step = sqlite3_column_double(stmt, 6);
        const char *change_step_button = (const char *)sqlite3_column_text(stmt, 7);

        printf("Name: %s\n", nom);
        printf("Move Forward: %s\n", move_forward);
        printf("Move Backward: %s\n", move_backward);
        printf("Move Left: %s\n", move_left);
        printf("Move Right: %s\n", move_right);
        printf("Max Speed First Step: %lf\n", max_speed_first_step);
        printf("Max Speed Second Step: %lf\n", max_speed_second_step);
        printf("Change Step Button: %s\n", change_step_button);
        printf("\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

int update_configuration(int id, Configuration *config) {

    sqlite3 *db;
    sqlite3_stmt *stmt;
    int parameters_count = 0;
    char *sql = "UPDATE configuration SET";

    if(sqlite3_open("../caremote_db", &db) != SQLITE_OK)
        return 1;

    if (config->name != NULL) {
        sql = sqlite3_mprintf("%s name = ?,", sql);
        parameters_count++;
    }

    if (config->move_forward != NULL) {
        sql = sqlite3_mprintf("%s move_forward = ?,", sql);
        parameters_count++;
    }
    if (config->move_backward != NULL) {
        sql = sqlite3_mprintf("%s move_backward = ?,", sql);
        parameters_count++;
    }
    if (config->move_left != NULL) {
        sql = sqlite3_mprintf("%s move_left = ?,", sql);
        parameters_count++;
    }
    if (config->move_right != NULL) {
        sql = sqlite3_mprintf("%s move_right = ?,", sql);
        parameters_count++;
    }
    if (config->max_speed_first_step != NULL) {
        sql = sqlite3_mprintf("%s max_speed_first_step = ?,", sql);
        parameters_count++;
    }
    if (config->max_speed_second_step != NULL) {

        sql = sqlite3_mprintf("%s max_speed_second_step = ?,", sql);
        parameters_count++;
    }

    if (config->change_step_button != NULL) {
        sql = sqlite3_mprintf("%s change_step_button = ?,", sql);
        parameters_count++;
    }

    if (parameters_count == 0) {
        printf("Aucun paramètre de mise à jour spécifié.\n");
        return 1;
    }

    // Retirer la virgule finale
    sql[strlen(sql) - 1] = '\0';

    // Ajouter la condition WHERE pour identifier la configuration à mettre à jour
    sql = sqlite3_mprintf("%s WHERE id = ?", sql);

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Erreur de préparation de la requête : %s\n", sqlite3_errmsg(db));
        sqlite3_free(sql);
        return 1;
    }

    int param_position = 1;

    if (config->name != NULL)
        sqlite3_bind_text(stmt, param_position++, config->name, -1, SQLITE_STATIC);
    if (config->move_forward != NULL)
        sqlite3_bind_text(stmt, param_position++, config->move_forward, -1, SQLITE_STATIC);
    if(config->move_backward != NULL)
        sqlite3_bind_text(stmt, param_position++, config->move_backward, -1, SQLITE_STATIC);
    if (config->move_left != NULL)
        sqlite3_bind_text(stmt, param_position++, config->move_left, -1, SQLITE_STATIC);
    if (config->move_right != NULL)
        sqlite3_bind_text(stmt, param_position++, config->move_right, -1, SQLITE_STATIC);
    if (config->max_speed_first_step != NULL)
        sqlite3_bind_double(stmt, param_position++, atof(config->max_speed_first_step));
    if (config->max_speed_second_step != NULL)
        sqlite3_bind_double(stmt, param_position++, atof(config->max_speed_second_step));
    if (config->change_step_button != NULL)
        sqlite3_bind_text(stmt, param_position++, config->change_step_button, -1, SQLITE_STATIC);

    sqlite3_bind_int(stmt, param_position, id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Erreur lors de l'execution de la requete : %s\n", sqlite3_errmsg(db));
    } else {
        printf("Configuration mise a jour \n");
    }

    sqlite3_finalize(stmt);
    sqlite3_free(sql);

    return 0;}
    
int delete_configuration(int id_configuration) {
    sqlite3 *db;
    sqlite3_stmt *stmt;

    if(sqlite3_open("../caremote_db", &db) != SQLITE_OK)
        return 1;

    if (sqlite3_prepare_v2(db, "DELETE FROM configuration WHERE id = ?", -1, &stmt, NULL) != SQLITE_OK) {
        printf("Erreur de preparation de la requete : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    sqlite3_bind_int(stmt, 1, id_configuration);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Erreur lors de l'exécution de la requête : %s\n", sqlite3_errmsg(db));
    } else {
        printf("Configuration supprimee\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}

int main(){
    int id_profile = 1;
    int id_configuration = 1;
    Configuration config;

    config.name = "Config de thuthur";
    config.move_forward = NULL;
    config.move_backward = NULL;
    config.move_left = NULL;
    config.move_right = NULL;
    config.max_speed_first_step = "3.01";
    config.max_speed_second_step = "6.000";
    config.change_step_button = NULL;

    update_configuration(id_configuration, &config);

    return 0;
}
