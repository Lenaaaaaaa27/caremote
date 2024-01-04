#include "../../includes/define.h"
#include <stdio.h>
#include <windows.h>  // Pour GetAsyncKeyState

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
    int thumbLX = 0;
    int thumbLY = 0;

    int clientSocket = initConnexion();

    while (fin) {
        XINPUT_STATE state;
        if (XInputGetState(0, &state) == 0) {
            thumbLX = state.Gamepad.sThumbLX;
            axe_X = 0;
            if(state.Gamepad.bRightTrigger) {
                    axe_X = state.Gamepad.bRightTrigger > 100?state.Gamepad.bRightTrigger:0;
                }
                if(state.Gamepad.bLeftTrigger) {
                    axe_X = state.Gamepad.bLeftTrigger > 100?state.Gamepad.bLeftTrigger * -1:0;
                }
            if (thumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || thumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
                if (thumbLX > 0) {
                    if (axe_Z < 1000)
                        axe_Z += 50;
                } else
                    if (axe_Z > 500)
                        axe_Z -= 50;
            } else {
                axe_Z = 750;
            }

        } else {
            if (GetAsyncKeyState(control.speed_up) & 0x8001) {
                if(axe_X < 255)
                    axe_X += 10;
            }

            if (GetAsyncKeyState(control.speed_down) & 0x8001) {
                if(axe_X > -255)
                    axe_X -= 10;
            }

            if(!(GetAsyncKeyState(control.speed_down) & 0x8001) && !((GetAsyncKeyState(control.speed_up) & 0x8001))) {
                axe_X = 0;
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
        }
            char buffer[256];
            sprintf(buffer, "c%d", axe_Z);
            send(clientSocket, buffer, strlen(buffer), 0);

            sprintf(buffer, "m%d", axe_X);
            send(clientSocket, buffer, strlen(buffer), 0);
            // Un court délai pour ne pas surcharger le processeur
            Sleep(30);

        if (GetAsyncKeyState(VK_ESCAPE) & 0x8001) {
            fin = 0;
        }
    }
    closeConnexion(clientSocket);
}
