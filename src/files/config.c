//
// Created by arthur on 11/01/2024.
//
#define MAX_SIZE_LINE 256

#include "../../includes/define.h"


int setConfig(const char *myfile) {
    FILE *file = fopen(myfile, "r");
    boolean condition = TRUE;

    if (file == NULL) {
        error_content(400);
        return EXIT_FAILURE;
    }

    char line[MAX_SIZE_LINE];
    char *endptr;
    char letter;

    while (fgets(line, sizeof(line), file) != NULL) {
        char *token = strtok(line, ":");
        char *value = strtok(NULL, "\n");


        if (token != NULL && value != NULL) {
            if (strcmp(token, "ForceFeedback") == 0) {
                settings->forceFeedback = strtol(value, &endptr, 10);
                if (*endptr != '\0') {
                    return EXIT_FAILURE;
                }
            } else if (strcmp(token, "CarName") == 0) {
                settings->carName = (char *)malloc(strlen(value) + 1);
                if (settings->carName == NULL) {
                    return EXIT_FAILURE;
                }
                strcpy(settings->carName, value);
            } else if (strcmp(token, "DefaultUserId") == 0) {
                settings->defaultUserId = strtol(value, &endptr, 10);
                if (*endptr != '\0') {
                    return EXIT_FAILURE;
                }
            } else if (strcmp(token, "MaxSessionTime") == 0) {
                settings->maxSessionTime = strtol(value, &endptr, 10);
                if (*endptr != '\0') {
                    return EXIT_FAILURE;
                }
            } else if (strcmp(token, "ProfileUsername") == 0) {
                settings->profileUsername = (char *)malloc(strlen(value) + 1);
                if (settings->profileUsername == NULL) {
                    return EXIT_FAILURE;
                }
                strcpy(settings->profileUsername, value);
            } else if (strcmp(token, "configurationName") == 0) {
                char *name = malloc(30 *sizeof(char));
                if (name == NULL) {
                    return EXIT_FAILURE;
                }
                strcpy(name, value);
                strcpy(settings->configuration.name, name);
                free(name);
            } else if (strcmp(token, "configurationSpeedStep") == 0) {
                settings->configuration.speed_step = strtol(value, &endptr, 10);
                if (*endptr != '\0') {
                    return EXIT_FAILURE;
                }
            } else if (strcmp(token, "configurationMoveForward") == 0) {
                letter = value[0];
                if (!isalpha(letter) || letter == '\0')
                    return EXIT_FAILURE;
                settings->configuration.move_forward = letter;
            } else if (strcmp(token, "configurationMoveBackward") == 0) {
                letter = value[0];
                if (!isalpha(letter))
                    return EXIT_FAILURE;
                settings->configuration.move_backward = letter;
            } else if (strcmp(token, "configurationMoveLeft") == 0) {
                letter = value[0];
                if (!isalpha(letter))
                    return EXIT_FAILURE;
                settings->configuration.move_left = letter;
            } else if (strcmp(token, "configurationMoveRight") == 0) {
                letter = value[0];
                if (!isalpha(letter))
                    return EXIT_FAILURE;
                settings->configuration.move_right = letter;
            } else if (strcmp(token, "configurationChangeStepButton") == 0) {
                letter = value[0];
                if (!isalpha(letter))
                    return EXIT_FAILURE;
                settings->configuration.change_step_button = letter;
            }
        }
    }

    if(settings->forceFeedback < 0 || settings->forceFeedback > 1 ){
        error_content(401);
        condition = FALSE;
    }

    if(settings->carName == NULL || settings->carName[0] == '\0'){
        error_content(402);
        condition = FALSE;
    }

    if(settings->defaultUserId < 0){
        settings->defaultUserId = 1;
        error_content(403);
    }

    if(settings->maxSessionTime < 0 || settings->maxSessionTime == '\0'){
        error_content(404);
        condition = FALSE;
    }

    if(settings->profileUsername == NULL || settings->profileUsername[0] == '\0'){
        error_content(405);
        condition = FALSE;
    }

    if(settings->configuration.name[0] == '\0'){
        error_content(406);
        condition = FALSE;
    }

    if(settings->configuration.speed_step < 0 || settings->configuration.speed_step == '\0'){
        error_content(407);
        condition = FALSE;
    }

    if(!isalpha(settings->configuration.move_forward) || !isalpha(settings->configuration.move_backward) ||
       !isalpha(settings->configuration.move_left) || !isalpha(settings->configuration.move_right) ||
       !isalpha(settings->configuration.change_step_button)){
        error_content(408);
        condition = FALSE;
    }

    fclose(file);
    if(!condition){
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}