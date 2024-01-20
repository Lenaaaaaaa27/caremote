//
// Created by arthu on 12/01/2024.
// data exports
#include "../../includes/define.h"

int exportConfig(Configuration *configuration) {

    char currentDir[MAX_PATH];
    getcwd(currentDir, sizeof(currentDir));

    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "name", cJSON_CreateString(configuration->name));
    cJSON_AddItemToObject(root, "move_forward", cJSON_CreateNumber(configuration->move_forward));
    cJSON_AddItemToObject(root, "move_backward", cJSON_CreateNumber(configuration->move_backward));
    cJSON_AddItemToObject(root, "move_left", cJSON_CreateNumber(configuration->move_left));
    cJSON_AddItemToObject(root, "move_right", cJSON_CreateNumber(configuration->move_right));
    cJSON_AddItemToObject(root, "speed_step", cJSON_CreateNumber(configuration->speed_step));
    cJSON_AddItemToObject(root, "change_step_button", cJSON_CreateNumber(configuration->change_step_button));
    char *jsonString = cJSON_Print(root);

    // call up a Windows window so the user can choose where to save the file
    OPENFILENAME ofn;
    char filePath[MAX_PATH] = "configuration.json";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "Fichiers JSON (*.json)\0*.json\0";
    ofn.lpstrFile = filePath;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = "Enregistrer le fichier JSON";
    ofn.Flags = OFN_OVERWRITEPROMPT;

    if (GetSaveFileName(&ofn) != 0) {

        FILE *fichier = fopen(filePath, "w");
        if (fichier) {
            fprintf(fichier, "%s", jsonString);
            fclose(fichier);
        } else {
            chdir(currentDir);
            cJSON_Delete(root);
            free(jsonString);
            error_content(301);
            return EXIT_FAILURE;
        }
    }

    chdir(currentDir);

    cJSON_Delete(root);
    free(jsonString);

    return EXIT_SUCCESS;
}

int exportSessions(Session sessions[], int numSessions) {
    char currentDir[MAX_PATH];
    getcwd(currentDir, sizeof(currentDir));


    char numberString[10];
    char configName[30];
    cJSON *sessionsArray = cJSON_CreateArray();

    for (int i = 0; i < numSessions; i++) {
        cJSON *root = cJSON_CreateObject();
        cJSON_AddItemToObject(root, "name", cJSON_CreateString(sessions[i].name));
        cJSON_AddItemToObject(root, "duration", cJSON_CreateString(intToDate(sessions[i].duration)));
        cJSON_AddItemToObject(root, "distance", cJSON_CreateNumber(sessions[i].distance));
        snprintf(numberString, sizeof(numberString), "%.2lf", sessions[i].average_speed);
        cJSON_AddItemToObject(root, "average_speed", cJSON_CreateString(numberString));
        cJSON_AddItemToObject(root, "time_start", cJSON_CreateString(sessions[i].time_start));
        if (does_configuration_exist_with_id(sessions[i].id_configuration)) {
            strcpy(configName, get_configuration(sessions[i].id_configuration).name);
        } else {
            strcpy(configName, "Configuration deleted");
        }
        cJSON_AddItemToObject(root, "configName", cJSON_CreateString(configName));
        cJSON_AddItemToObject(root, "profileId", cJSON_CreateString(get_profile(sessions[i].id_profile).username));
        cJSON_AddItemToArray(sessionsArray, root);
    }

    char *jsonString = cJSON_Print(sessionsArray);

    // call up a Windows window so the user can choose where to save the file
    OPENFILENAME ofn;
    char filePath[MAX_PATH] = "sessions.json";
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "Fichiers JSON (*.json)\0*.json\0";
    ofn.lpstrFile = filePath;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = "Enregistrer le fichier JSON";
    ofn.Flags = OFN_OVERWRITEPROMPT;

    if (GetSaveFileName(&ofn) != 0) {
        FILE *fichier = fopen(filePath, "w");
        if (fichier) {
            fprintf(fichier, "%s", jsonString);
            fclose(fichier);
        } else {
            error_content(300);
            return 1;
        }
    }

    chdir(currentDir);

    cJSON_Delete(sessionsArray);
    free(jsonString);

    return 0;
}

char *intToDate(int duration) {
    time_t temps = (time_t)duration;
    struct tm *tempsStruct = gmtime(&temps);
    char *chaine = malloc(20);
    strftime(chaine, 10, "%H:%M:%S", tempsStruct);
    return chaine;
}