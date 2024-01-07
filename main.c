#include <gtk/gtk.h>
#include "src/Database.c"

int main() {
    Configuration configuration = get_configuration(1);
    int clientSocket = initConnexion();
    control(&configuration,clientSocket);
    closeConnexion(clientSocket);
    return 0;
}