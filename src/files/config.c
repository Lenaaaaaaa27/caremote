//
// Created by arthur on 11/01/2024.
//
#define MAX_SIZE_LINE 256

#include "../../includes/define.h"


int setConfig(const char *myfile, Setting *setting) {
    FILE *file = fopen(myfile, "r");

    if (file == NULL) {
        perror("file did't open correctly");
        exit(EXIT_FAILURE);
    }

    char line[MAX_SIZE_LINE];
    char *endptr;
    char letter;

    while (fgets(line, sizeof(line), file) != NULL) {
        char *token = strtok(line, ":");
        char *value = strtok(NULL, "\n");


        if (token != NULL && value != NULL) {
            if (strcmp(token, "ForceFeedback") == 0) {
                setting->forceFeedback = strtol(value, &endptr, 10);
                if (*endptr != '\0') {
                    return EXIT_FAILURE;
                }
            } else if (strcmp(token, "CarName") == 0) {
                setting->carName = (char *)malloc(strlen(value) + 1);
                if (setting->carName == NULL) {
                    return EXIT_FAILURE;
                }
                strcpy(setting->carName, value);
            } else if (strcmp(token, "DefaultUserId") == 0) {
                setting->defaultUserId = strtol(value, &endptr, 10);
                if (*endptr != '\0') {
                    return EXIT_FAILURE;
                }
            } else if (strcmp(token, "DefaultConfigId") == 0) {
                setting->defaultConfigId = strtol(value, &endptr, 10);
                if (*endptr != '\0') {
                    return EXIT_FAILURE;
                }
            } else if (strcmp(token, "MaxSessionTime") == 0) {
                setting->maxSessionTime = strtol(value, &endptr, 10);
                if (*endptr != '\0') {
                    return EXIT_FAILURE;
                }
            } else if (strcmp(token, "ProfileUsername") == 0) {
                setting->profileUsername = (char *)malloc(strlen(value) + 1);
                if (setting->profileUsername == NULL) {
                    return EXIT_FAILURE;
                }
                strcpy(setting->profileUsername, value);
            } else if (strcmp(token, "configurationName") == 0) {
                char *name = malloc(30 *sizeof(char));
                if (name == NULL) {
                    return EXIT_FAILURE;
                }
                strcpy(name, value);
                strcpy(setting->configuration.name, name);
                free(name);
            } else if (strcmp(token, "configurationSpeedStep") == 0) {
                setting->configuration.speed_step = strtol(value, &endptr, 10);
                if (*endptr != '\0') {
                    return EXIT_FAILURE;
                }
            } else if (strcmp(token, "configurationMoveForward") == 0) {
                letter = value[0];
                if (!isalpha(letter) || letter == '\0')
                    return EXIT_FAILURE;
                setting->configuration.move_forward = letter;
            } else if (strcmp(token, "configurationMoveBackward") == 0) {
                letter = value[0];
                if (!isalpha(letter))
                    return EXIT_FAILURE;
                setting->configuration.move_backward = letter;
            } else if (strcmp(token, "configurationMoveLeft") == 0) {
                letter = value[0];
                if (!isalpha(letter))
                    return EXIT_FAILURE;
                setting->configuration.move_left = letter;
            } else if (strcmp(token, "configurationMoveRight") == 0) {
                letter = value[0];
                if (!isalpha(letter))
                    return EXIT_FAILURE;
                setting->configuration.move_right = letter;
            } else if (strcmp(token, "configurationChangeStepButton") == 0) {
                letter = value[0];
                if (!isalpha(letter))
                    return EXIT_FAILURE;
                setting->configuration.change_step_button = letter;
            }
        }
    }
    fclose(file);
    return EXIT_SUCCESS;
}