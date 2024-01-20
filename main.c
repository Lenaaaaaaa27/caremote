//
// Created by Lena on 05/11/2023.
//
//starting point of the program


#include "includes/define.h"

Setting *settings;

void* gtk_thread_function(void* data) {
    GtkApplication *app;

    if(verification_database()){
        create_database();
    }

    app = gtk_application_new("caremote.first.version", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    g_application_run(G_APPLICATION(app), 0, NULL);
    g_object_unref(app);

    return 0;
}

int main(int argc, char *argv[]) {

    settings = malloc(sizeof(Setting));

    if (setConfig("config.ini") == EXIT_FAILURE) {
        MessageBox(NULL, "The configuration has failed. Please check the config.txt file", "Error in configuration", MB_ICONERROR | MB_OK);
        return EXIT_FAILURE;
    }

    pthread_t gtk_thread;
    pthread_create(&gtk_thread, NULL, gtk_thread_function, NULL);
    pthread_join(gtk_thread, NULL);

    return 0;
}

