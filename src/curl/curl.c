//
// Created by Arthur on 15/01/2023.
// retrieve the car's code from a github repo and save it on the computer

#include "../../includes/define.h"

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
}

int curl() {

    // curl initialization

    char currentDir[MAX_PATH];
    getcwd(currentDir, sizeof(currentDir));

    CURL *curl = curl_easy_init();
    if (!curl) {
        error_content(321);
        return 2;
    }
    curl_easy_setopt(curl, CURLOPT_CAINFO, "curl-ca-bundle.crt");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    // call a windows window so that the user can choose where to save the zip
    OPENFILENAME ofn;
    char filePath[MAX_PATH] = "caremote_car.zip";
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "ZIP file (*.zip)\0*.zip\0";
    ofn.lpstrFile = filePath;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = "Save ZIP file";
    ofn.Flags = OFN_OVERWRITEPROMPT;

    if (GetSaveFileName(&ofn) != 0) {
        FILE *file = fopen(filePath, "wb");
        if (!file) {
            chdir(currentDir);
            error_content(322);
            return EXIT_FAILURE;
        }
        chdir(currentDir);
        const char *owner = "fantom609";
        const char *repo = "caremote_car";
        char url[256];
        snprintf(url, sizeof(url), "https://github.com/%s/%s/archive/master.zip", owner, repo);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);


        CURLcode res = curl_easy_perform(curl);

        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        // file deletion in case of failure
        if (res != CURLE_OK) {
            fclose(file);
            remove(filePath);
            error_content(323);
            return 2;
        }

        fclose(file);

    }
    curl_easy_cleanup(curl);

    curl_global_cleanup();


    return EXIT_SUCCESS;
}