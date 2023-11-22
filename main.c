#include "includes.h"
#include <gtk/gtk.h>

int main() {
    if(verification_database()) create_database();

    return 0;
}