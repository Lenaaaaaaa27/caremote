#include "../../includes/define.h"


#define MAX_AXE_Z 1000
#define AVG_AXE_Z 750
#define MIN_AXE_Z 500
#define VAR_AXE_Z 50

#define MAX_AXE_X 255
#define MIN_AXE_X 0
#define VAR_AXE_X 15

#define TIME_LOOP 50
#define VMAXPULSE ((configuration->speed_step*255)/100)

int control(Configuration *configuration, int id_user, int clientSocket, Setting *setting){
    int axe_X = MIN_AXE_X;
    int axe_Z = AVG_AXE_Z;
    int thumbLX;
    int vMax = VMAXPULSE;
    int previousSpeedSwitchState = 0;
    int duration = 0;
    int avg_X = 0;
    int loop = 0;
    int pulse = 0;
    int distanceCovered = 0;
    int forceFeedback = setting->forceFeedback;
    int maxSessionTime = setting->maxSessionTime;
    Session session;
    time_t t;
    char buffer[256];
    char name[40] = "Session of ";
    char tempsStr[20];

    struct tm *tempsStruct;
    time(&t);
    tempsStruct = localtime(&t);
    strftime(tempsStr, sizeof(tempsStr), "%Y-%m-%d %H:%M:%S", tempsStruct);
    strcat(name, tempsStr);

    configuration->speed_step < 0?0:configuration->speed_step > 100?100:configuration->speed_step; // 0 <= speed_step <= 100 

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

            if (GetAsyncKeyState(toupper(configuration->move_forward)) & 0x8001) {
                axe_X = abs(axe_X) > vMax?vMax:axe_X;
                if (axe_X < vMax)
                    axe_X += VAR_AXE_X;
            }

            if (GetAsyncKeyState(toupper(configuration->move_backward)) & 0x8001) {
                axe_X = abs(axe_X) > vMax?-vMax:axe_X;
                if (axe_X > -vMax)
                    axe_X -= VAR_AXE_X;
            }

            if (!(GetAsyncKeyState(toupper(configuration->move_backward)) & 0x8001) && !(GetAsyncKeyState(toupper(configuration->move_forward)) & 0x8001)) {
                axe_X = MIN_AXE_X;
            }

            if (GetAsyncKeyState(toupper(configuration->move_right)) & 0x8001) {
                if (axe_Z < MAX_AXE_Z) {
                    axe_Z += VAR_AXE_Z;
                }
            }
            if (GetAsyncKeyState(toupper(configuration->move_left)) & 0x8001) {
                if (axe_Z > MIN_AXE_Z) {
                    axe_Z -= VAR_AXE_Z;
                }
            }
            forceFeedback;
            if (!(GetAsyncKeyState(toupper(configuration->move_left)) & 0x8001) && !(GetAsyncKeyState(toupper(configuration->move_right)) & 0x8001) && forceFeedback == 1) {
                axe_Z = AVG_AXE_Z;
            }

            if ((GetAsyncKeyState(toupper(configuration->change_step_button)) & 0x8001) && !(previousSpeedSwitchState & 0x8001)) {
                if(vMax == VMAXPULSE) {
                    vMax = MAX_AXE_X;
                } else {
                    vMax = VMAXPULSE;
                }
            }

            previousSpeedSwitchState = GetAsyncKeyState(toupper(configuration->change_step_button));
        }

        pulse+= axe_X;
        if(loop%(1000/TIME_LOOP) == 0) {
            duration += 1;
            distanceCovered += abs(distance(1,(int)avg_speed(pulse,1000/TIME_LOOP)));
            pulse = 0;
        }

        if(duration >= maxSessionTime) {
            g_idle_add(on_stop_session_callback, NULL);
        }

        if(axe_X < 5 || axe_X > 100) avg_X += abs(axe_X);
        sprintf(buffer, "c%d", axe_Z);
        send(clientSocket, buffer, strlen(buffer), 0);
        sprintf(buffer, "m%d", axe_X);
        send(clientSocket, buffer, strlen(buffer), 0);

        Sleep(TIME_LOOP);
        loop +=1;
        gdouble instantSpeed = fabs(speed(axe_X));

        struct UpdateLabelsData *data = g_malloc(sizeof(struct UpdateLabelsData));
        data->instantSpeed = fabs(speed(axe_X));
        data->duration = duration;
        data->distanceCovered = distanceCovered;

        g_idle_add(update_labels_callback, data);

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
    return 0;
}