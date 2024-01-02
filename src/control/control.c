#include <stdio.h>
#include <windows.h>  // Pour GetAsyncKeyState
#include "../../includes/define.h"

void init_control(Control *control) {
    control->speed_up = 'Z';
    control->speed_down = 'S';
    control->right = 'D';
    control->left = 'Q';
}

void control() {
    Control control;
    init_control(&control);
    int axe_X = 0;
    int axe_Z = 750;
    int fin = 1;

    int clientSocket = initConnexion();

    while (fin) {
        axe_X = 0;
        if (GetAsyncKeyState(control.speed_up) & 0x8001) {
            axe_X = 1;
        }
        if (GetAsyncKeyState(control.speed_down) & 0x8001) {
        }
        if (GetAsyncKeyState(control.right) & 0x8001) {
            if (axe_Z < 1000) {
                axe_Z += 50;
            }
        }
        if (GetAsyncKeyState(control.left) & 0x8001) {
            if (axe_Z > 500) {
                axe_Z -= 50;
            }
        }
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8001) {
            fin = 0;
        }

            char buffer[256];
            sprintf(buffer, "d%d", axe_Z);
            send(clientSocket, buffer, strlen(buffer), 0);

            sprintf(buffer, "m%d", axe_X);
            send(clientSocket, buffer, strlen(buffer), 0);
            // Un court délai pour ne pas surcharger le processeur
        Sleep(30);
    }
    closeConnexion(clientSocket);
}
