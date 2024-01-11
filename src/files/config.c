//
// Created by arthur on 11/01/2024.
//
#define TAILLE_MAX_LIGNE 256

#include "../../includes/define.h"


void setConfig(const char *nomFichier, Setting *setting) {
    FILE *fichier = fopen(nomFichier, "r");

    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    char ligne[TAILLE_MAX_LIGNE]; // Vous pouvez ajuster la taille selon vos besoins

    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        char *token = strtok(ligne, ":");
        char *valeur = strtok(NULL, "\n");

        if (token != NULL && valeur != NULL) {
            if (strcmp(token, "forceFeedback") == 0) {
                setting->forceFeedback = atoi(valeur);
            } else if (strcmp(token, "carName") == 0) {
                strcpy(setting->carName, valeur);
            }
        }
    }

    fclose(fichier);
}