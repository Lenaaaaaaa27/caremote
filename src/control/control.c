#include "../../includes/define.h"


#define MAX_AXE_Z 1000
#define AVG_AXE_Z 750
#define MIN_AXE_Z 500
#define VAR_AXE_Z 50

#define MAX_AXE_X 255
#define MIN_AXE_X 0
#define VAR_AXE_X 15

#define TIME_LOOP 50
#define VMAXPULSE (configuration->max_speed_first_step*255)/100

void control(Configuration *configuration, int id_user) {
    int axe_X = MIN_AXE_X;
    int axe_Z = AVG_AXE_Z;
    int clientSocket = initConnexion();
    int thumbLX;
    int vMax = VMAXPULSE;
    int previousSpeedSwitchState = 0;
    char buffer[256];
    int duration = 0;
    int avg_X = 0;
    int loop = 0;
    int pulse = 0;
    int distanceCovered = 0;
    Session session;
    time_t t;
    char name[40] = "Session of ";
    struct tm *tempsStruct;
    char tempsStr[20];
    time(&t);
    tempsStruct = localtime(&t);
    strftime(tempsStr, sizeof(tempsStr), "%Y-%m-%d %H:%M:%S", tempsStruct);
    strcat(name, tempsStr);
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
                    if (vMax == VMAXPULSE) {
                        vMax = MAX_AXE_X;
                    } else {
                        vMax = VMAXPULSE;
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
                if(vMax == VMAXPULSE) {
                    vMax = MAX_AXE_X;
                } else {
                    vMax = VMAXPULSE;
                }
            }

            previousSpeedSwitchState = GetAsyncKeyState(configuration->change_step_button);
        }

        pulse+= axe_X;
        if(loop%(1000/TIME_LOOP) == 0) {
            duration += 1;
            distanceCovered += abs(distance(1,(int)avg_speed(pulse,1000/TIME_LOOP)));
            pulse = 0;
        }

        if(axe_X < 5 || axe_X > 100) avg_X += abs(axe_X);
        sprintf(buffer, "c%d", axe_Z);
        send(clientSocket, buffer, strlen(buffer), 0);
        sprintf(buffer, "m%d", axe_X);
        send(clientSocket, buffer, strlen(buffer), 0);

        Sleep(TIME_LOOP);
        loop +=1;
        gdouble instantSpeed = fabs(speed(axe_X));

        update_speed_label(instantSpeed);
        update_duration_label(duration);
        update_distance_label(distanceCovered);
    }

    session.id_profile = id_user;
    session.id_configuration = configuration->id;
    strcpy(session.name, name);
    session.duration = duration;
    session.distance = distanceCovered;
    session.average_speed = distanceCovered/(double)duration;
    strcpy(session.time_start, tempsStr);
    create_session(session);
    refresh_sessions_view(id_user);

    closeConnexion(clientSocket);
}