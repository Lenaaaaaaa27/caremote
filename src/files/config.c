//
// Created by arthur on 11/01/2024.
//
#define TAILLE_MAX_LIGNE 256

#include "../../includes/define.h"


int setConfig(const char *nomFichier, Setting *setting) {
    FILE *fichier = fopen(nomFichier, "r");

    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    char ligne[TAILLE_MAX_LIGNE];
    char *endptr;

    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        char *token = strtok(ligne, ":");
        char *valeur = strtok(NULL, "\n");


        if (token != NULL && valeur != NULL) {
            if (strcmp(token, "ForceFeedback") == 0) {
                setting->forceFeedback = strtol(valeur, &endptr, 10);
                if (*endptr != '\0') {
                    return EXIT_FAILURE;
                }
            } else if (strcmp(token, "CarName") == 0) {
                setting->carName = (char *)malloc(strlen(valeur) + 1);
                if (setting->carName == NULL) {
                    return EXIT_FAILURE;
                }
                strcpy(setting->carName, valeur);
            } else if (strcmp(token, "DefaultUserId") == 0) {
                setting->defaultUserId = strtol(valeur, &endptr, 10);
                if (*endptr != '\0') {
                    return EXIT_FAILURE;
                }
            } else if (strcmp(token, "DefaultConfigId") == 0) {
                setting->defaultConfigId = strtol(valeur, &endptr, 10);
                if (*endptr != '\0') {
                    return EXIT_FAILURE;
                }
            } else if (strcmp(token, "MaxSessionTime") == 0) {
                setting->maxSessionTime = strtol(valeur, &endptr, 10);
                if (*endptr != '\0') {
                    return EXIT_FAILURE;
                }
            } else if (strcmp(token, "ProfileUsername") == 0) {
                setting->profileUsername = (char *)malloc(strlen(valeur) + 1);
                if (setting->profileUsername == NULL) {
                    return EXIT_FAILURE;
                }
                strcpy(setting->profileUsername, valeur);
            }
        }
    }
    fclose(fichier);
    return EXIT_SUCCESS;
}