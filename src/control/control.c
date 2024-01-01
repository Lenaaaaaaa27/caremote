//
// Created by Arthur on 03/12/2023.
// Car control management (acceleration, steering)
//
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include "../../includes/define.h"



void init_control(Control *control) {
    control->speed_up = 'z';
    control->speed_down = 's';
    control->right = 'd';
    control->left = 'q';
}

void control() {
    Control control;
    init_control(&control);
    int axe_X = 0;
    int axe_Z= 50;
    boolean fin = 1;
    while (fin) {
        if (GetAsyncKeyState('Z') & 0x8001) {
            if(axe_X < 100)
                axe_X = axe_X + 5;
        }
        if (GetAsyncKeyState('S') & 0x8001) {
            if(axe_X > 0)
                axe_X = axe_X - 5;
        }
        if (GetAsyncKeyState('D') & 0x8001) {
            if(axe_Z < 100)
                axe_Z = axe_Z + 5;
        }
        if (GetAsyncKeyState('Q') & 0x8001) {
            if(axe_Z > 0)
                axe_Z = axe_Z - 5;
        }
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8001) {
            fin = 0;
        }

        printf("axe Z : %d\n",axe_Z);
        printf("axe X : %d\n",axe_X);

        // Un court délai pour ne pas surcharger le processeur
        Sleep(100);
    }
}