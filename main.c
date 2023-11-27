#include <gtk/gtk.h>
#include "src/Database.c"

int main() {
    if(verification_database()) create_database();

    return 0;
}