#include "includes/define.h"


void* gtk_thread_function(void* data) {
    GtkApplication *app;
    int status;

    if(verification_database()){
        create_database();
    }

    app = gtk_application_new("caremote.first.version", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), 0, NULL);
    g_object_unref(app);

    return 0;
}

int main(int argc, char *argv[]) {

    pthread_t gtk_thread;
    pthread_create(&gtk_thread, NULL, gtk_thread_function, NULL);
    pthread_join(gtk_thread, NULL);

    return 0;
}

