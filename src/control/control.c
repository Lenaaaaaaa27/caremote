#include "../../includes/define.h"
#include <stdio.h>
#include <windows.h>  // Pour GetAsyncKeyState

void init_control(Control *control) {
    control->speed_up = XINPUT_GAMEPAD_A;
    control->speed_down = XINPUT_GAMEPAD_B;
    control->right = XINPUT_GAMEPAD_RIGHT_SHOULDER;
    control->left = XINPUT_GAMEPAD_LEFT_SHOULDER;
}

void control() {
    Control control;
    init_control(&control);
    int axe_X = 0;
    int axe_Z = 750;
    int fin = 1;
    int thumbLX = 0;
    int thumbLY = 0;

    int clientSocket = initConnexion();

    while (fin) {
        axe_X = 0;

        XINPUT_STATE state;
        if (XInputGetState(0, &state) == 0) {
            thumbLX = state.Gamepad.sThumbLX;

            if (state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD ||) {
                axe_X = 1;
            }

            if (state.Gamepad.wButtons & control.speed_down) {
            }

            if (thumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || thumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
                if (thumbLX > 0) {
                    if (axe_Z < 1000) {
                        axe_Z += 50;
                    }
                } else {
                    if (axe_Z > 500) {
                        axe_Z -= 50;
                    }
                }
            }

            char buffer[256];
            sprintf(buffer, "d%d", axe_Z);
            send(clientSocket, buffer, strlen(buffer), 0);

            sprintf(buffer, "m%d", axe_X);
            send(clientSocket, buffer, strlen(buffer), 0);
            // Un court délai pour ne pas surcharger le processeur
            Sleep(30);
            } else {
            printf("La manette a etais debrancher");
        }
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8001) {
            fin = 0;
        }
    }

    closeConnexion(clientSocket);
}
