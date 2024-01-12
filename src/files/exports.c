//
// Created by arthu on 12/01/2024.
//
#include "../../includes/define.h"

int exportConfig(Configuration *configuration) {

    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "name", cJSON_CreateString(configuration->name));
    cJSON_AddItemToObject(root, "move_forward", cJSON_CreateNumber(configuration->move_forward));
    cJSON_AddItemToObject(root, "move_backward", cJSON_CreateNumber(configuration->move_backward));
    cJSON_AddItemToObject(root, "move_left", cJSON_CreateNumber(configuration->move_left));
    cJSON_AddItemToObject(root, "move_right", cJSON_CreateNumber(configuration->move_right));
    cJSON_AddItemToObject(root, "speed_step", cJSON_CreateNumber(configuration->speed_step));
    cJSON_AddItemToObject(root, "change_step_button", cJSON_CreateNumber(configuration->change_step_button));
    char *jsonString = cJSON_Print(root);

    // Initialiser la structure OPENFILENAME
    OPENFILENAME ofn;
    char cheminFichier[MAX_PATH] = "configuration.json";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "Fichiers JSON (*.json)\0*.json\0";
    ofn.lpstrFile = cheminFichier;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = "Enregistrer le fichier JSON";
    ofn.Flags = OFN_OVERWRITEPROMPT;

    // Afficher la boîte de dialogue de sauvegarde de fichier
    if (GetSaveFileName(&ofn) != 0) {
        // Enregistrer la chaîne JSON dans le fichier spécifié par l'utilisateur
        FILE *fichier = fopen(cheminFichier, "w");
        if (fichier) {
            fprintf(fichier, "%s", jsonString);
            fclose(fichier);
            printf("Fichier enregistré avec succès.\n");
        } else {
            printf("Erreur lors de l'ouverture du fichier.\n");
        }
    } else {
        printf("Annulation de l'enregistrement du fichier.\n");
    }

    cJSON_Delete(root);
    free(jsonString);

    return 0;
}

int exportSession(Session *session) {
    char configName[30];
    if(does_configuration_exist_with_id(session->id_configuration)) {
        strcpy(configName,get_configuration(session->id_configuration).name);
    } else {
        strcpy(configName,"Configuration deleted");
    }

    char numberString[6];  // Assurez-vous que la taille est suffisante
    snprintf(numberString, sizeof(numberString), "%.2lf", session->average_speed);

    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "name", cJSON_CreateString(session->name));
    cJSON_AddItemToObject(root, "duration", cJSON_CreateString(intToDate(session->duration)));
    cJSON_AddItemToObject(root, "distance", cJSON_CreateNumber(session->distance));
    cJSON_AddItemToObject(root, "average_speed", cJSON_CreateString(numberString));
    cJSON_AddItemToObject(root, "time_start", cJSON_CreateString(session->time_start));
    cJSON_AddItemToObject(root, "configName", cJSON_CreateString(configName));
    cJSON_AddItemToObject(root, "profileId", cJSON_CreateNumber(session->id_profile));
    char *jsonString = cJSON_Print(root);

    // Initialiser la structure OPENFILENAME
    OPENFILENAME ofn;
    char cheminFichier[MAX_PATH] = "session.json";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "Fichiers JSON (*.json)\0*.json\0";
    ofn.lpstrFile = cheminFichier;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = "Enregistrer le fichier JSON";
    ofn.Flags = OFN_OVERWRITEPROMPT;

    // Afficher la boîte de dialogue de sauvegarde de fichier
    if (GetSaveFileName(&ofn) != 0) {
        // Enregistrer la chaîne JSON dans le fichier spécifié par l'utilisateur
        FILE *fichier = fopen(cheminFichier, "w");
        if (fichier) {
            fprintf(fichier, "%s", jsonString);
            fclose(fichier);
        } else
            error_content(300);
    }

    cJSON_Delete(root);
    free(jsonString);

    return 0;
}

char *intToDate(int dureeEnSecondes) {
    time_t temps = (time_t)dureeEnSecondes;
    struct tm *tempsStruct = gmtime(&temps);
    char *chaineFormattee = malloc(20);
    strftime(chaineFormattee, 10, "%H:%M:%S", tempsStruct);
    return chaineFormattee;
}