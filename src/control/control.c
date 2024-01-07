#include "../../includes/define.h"
#include <stdio.h>
#include <windows.h>

#define MAX_AXE_Z 1000
#define AVG_AXE_Z 750
#define MIN_AXE_Z 500
#define VAR_AXE_Z 50

#define MAX_AXE_X 255
#define MIN_AXE_X 0
#define VAR_AXE_X 10

#define TIME_LOOP 30

void control(Configuration *configuration, int clientSocket) {
    int axe_X = MIN_AXE_X;
    int axe_Z = AVG_AXE_Z;
    int fin = 1;
    int thumbLX;
    int vMax = configuration->max_speed_first_step;
    int previousSpeedSwitchState = 0;
    char buffer[256];
    while (fin) {

        //Joystick control
        XINPUT_STATE state;

        if (XInputGetState(0, &state) == 0) {
            thumbLX = state.Gamepad.sThumbLX;
            axe_X = 0;

            if (state.Gamepad.bRightTrigger > 100) {
                axe_X = state.Gamepad.bRightTrigger <vMax?state.Gamepad.bRightTrigger:vMax;
            } else if (state.Gamepad.bLeftTrigger > 100) {
                axe_X = state.Gamepad.bLeftTrigger < vMax?-state.Gamepad.bRightTrigger:-vMax;
            }

            if (thumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || thumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
                if (thumbLX > 0) {
                    if (axe_Z < MAX_AXE_Z)
                        axe_Z += VAR_AXE_Z;
                } else {
                    if (axe_Z > MIN_AXE_Z)
                        axe_Z -= VAR_AXE_Z;
                }
            } else {
                axe_Z = AVG_AXE_Z;
            }

            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
                if (!previousSpeedSwitchState) {
                    previousSpeedSwitchState = 1;
                    if (vMax == configuration->max_speed_first_step) {
                        vMax = MAX_AXE_X;
                    } else {
                        vMax = configuration->max_speed_first_step;
                    }
                }
            } else {
                previousSpeedSwitchState = 0;
            }
        } else {
            // Keyboard control

            if (GetAsyncKeyState(configuration->move_forward) & 0x8001) {
                axe_X = abs(axe_X) > vMax?vMax:axe_X;
                if (axe_X < vMax)
                    axe_X += VAR_AXE_X;
            }

            if (GetAsyncKeyState(configuration->move_backward) & 0x8001) {
                axe_X = abs(axe_X) > vMax?-vMax:axe_X;
                if (axe_X > -vMax)
                    axe_X -= VAR_AXE_X;
            }

            if (!(GetAsyncKeyState(configuration->move_backward) & 0x8001) && !(GetAsyncKeyState(configuration->move_forward) & 0x8001)) {
                axe_X = MIN_AXE_X;
            }

            if (GetAsyncKeyState(configuration->move_right) & 0x8001) {
                if (axe_Z < MAX_AXE_Z) {
                    axe_Z += VAR_AXE_Z;
                }
            }

            if (GetAsyncKeyState(configuration->move_left) & 0x8001) {
                if (axe_Z > MIN_AXE_Z) {
                    axe_Z -= VAR_AXE_Z;
                }
            }

            if ((GetAsyncKeyState(configuration->change_step_button) & 0x8001) && !(previousSpeedSwitchState & 0x8001)) {
                if(vMax == configuration->max_speed_first_step) {
                    vMax = MAX_AXE_X;
                } else {
                    vMax = configuration->max_speed_first_step;
                }
            }

            previousSpeedSwitchState = GetAsyncKeyState(configuration->change_step_button);
        }

        sprintf(buffer, "c%d", axe_Z);
        send(clientSocket, buffer, strlen(buffer), 0);

        sprintf(buffer, "m%d", axe_X);
        send(clientSocket, buffer, strlen(buffer), 0);

        // Un court délai pour ne pas surcharger le processeur
        Sleep(TIME_LOOP);

        if (GetAsyncKeyState(VK_ESCAPE) & 0x8001) {
            fin = 0;
        }
    }
}