#include <stdio.h>
#include <curl/curl.h>

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
}

int curl() {
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Erreur lors de l'initialisation de cURL.\n");
        return 1;
    }

    FILE *fp = fopen("caremote.zip", "wb");
    if (!fp) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier de sortie.\n");
        curl_easy_cleanup(curl);
        return 1;
    }

    const char *owner = "fantom609";
    const char *repo = "caremote_car";
    char url[256];
    snprintf(url, sizeof(url), "https://github.com/%s/%s/archive/master.zip", owner, repo);

    curl_easy_setopt(curl, CURLOPT_CAINFO, "curl-ca-bundle.crt");
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    CURLcode res = curl_easy_perform(curl);

    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() a échoué : %s\n", curl_easy_strerror(res));
    }

    curl_easy_cleanup(curl);
    fclose(fp);

    curl_global_cleanup();

    return 0;
}
