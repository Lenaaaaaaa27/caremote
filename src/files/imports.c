//
// Created by arthu on 13/01/2024.
//
#include "../../includes/define.h"

int importsConfig(int id) {

    char currentDir[MAX_PATH];
    getcwd(currentDir, sizeof(currentDir));

    OPENFILENAME ofn;
    char szFile[260];
    memset(szFile, 0, sizeof(szFile));
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "JSON Files\0*.json\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    Configuration configuration;

    if (GetOpenFileName(&ofn) == TRUE) {

        FILE *file = fopen(szFile, "r");
        if (!file) {
            MessageBox(NULL, "Failed to open file.", "Error", MB_OK | MB_ICONERROR);
            chdir(currentDir);
            error_content(310);
            return EXIT_FAILURE;
        }


        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);


        char *fileContent = (char *)malloc(fileSize + 1);
        if (!fileContent) {
            fclose(file);
            chdir(currentDir);
            error_content(105);
            return EXIT_FAILURE;
        }

        fread(fileContent, 1, fileSize, file);
        fclose(file);

        fileContent[fileSize] = '\0';
        cJSON *json = cJSON_Parse(fileContent);
        if (!json) {
            MessageBox(NULL, "Failed to parse JSON.", "Error", MB_OK | MB_ICONERROR);
            free(fileContent);
            chdir(currentDir);
            error_content(311);
            return EXIT_FAILURE;
        }

        if (extractStringValue(json, "name", configuration.name, sizeof(configuration.name)) != EXIT_SUCCESS) {
            cJSON_Delete(json);
            free(fileContent);
            chdir(currentDir);
            error_content(312);
            return EXIT_FAILURE;
        }

        if (extractCharValue(json, "move_forward", &configuration.move_forward) != EXIT_SUCCESS) {
            cJSON_Delete(json);
            free(fileContent);
            chdir(currentDir);
            error_content(312);
            return EXIT_FAILURE;
        }

        if (extractCharValue(json, "move_backward", &configuration.move_backward) != EXIT_SUCCESS) {
            cJSON_Delete(json);
            free(fileContent);
            chdir(currentDir);
            error_content(314);
            return EXIT_FAILURE;
        }

        if (extractCharValue(json, "move_left", &configuration.move_left) != EXIT_SUCCESS) {
            cJSON_Delete(json);
            free(fileContent);
            chdir(currentDir);
            error_content(315);
            return EXIT_FAILURE;
        }

        if (extractCharValue(json, "move_right", &configuration.move_right) != EXIT_SUCCESS) {
            cJSON_Delete(json);
            free(fileContent);
            chdir(currentDir);
            error_content(316);
            return EXIT_FAILURE;
        }

        if (extractCharValue(json, "change_step_button", &configuration.change_step_button) != EXIT_SUCCESS) {
            cJSON_Delete(json);
            free(fileContent);
            chdir(currentDir);
            error_content(317);
            return EXIT_FAILURE;
        }

        if (extractIntValue(json, "speed_step", &configuration.speed_step) != EXIT_SUCCESS) {
            ZeroMemory(&ofn, sizeof(OPENFILENAME));
            cJSON_Delete(json);
            free(fileContent);
            chdir(currentDir);
            error_content(318);
            return EXIT_FAILURE;
        }
        configuration.id_profile = id;

        cJSON_Delete(json);
        free(fileContent);
    } else {
        chdir(currentDir);
        error_content(320);
        return EXIT_FAILURE;
    }

    chdir(currentDir);
    create_configuration(&configuration);
    return EXIT_SUCCESS;
}

int extractIntValue(cJSON *jsonObject, const char *key, int *result) {
    cJSON *item = cJSON_GetObjectItemCaseSensitive(jsonObject, key);
    if (item != NULL && cJSON_IsNumber(item)) {
        *result = item->valueint;
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int extractCharValue(cJSON *jsonObject, const char *key, char *result) {
    cJSON *item = cJSON_GetObjectItemCaseSensitive(jsonObject, key);
    if (item != NULL && cJSON_IsNumber(item)) {
        int asciiCode = item->valueint;
        if (asciiCode >= 0 && asciiCode <= 127) {
            *result = (char)asciiCode;
            return EXIT_SUCCESS;
        }
    }
    return EXIT_FAILURE;
}

int extractStringValue(cJSON *jsonObject, const char *key, char *result, size_t maxSize) {
    cJSON *item = cJSON_GetObjectItemCaseSensitive(jsonObject, key);
    if (item != NULL && cJSON_IsString(item) && strlen(item->valuestring) < maxSize) {
        strcpy(result, item->valuestring);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}