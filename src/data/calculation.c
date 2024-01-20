//
// Created by arthur on 07/01/2024.
// distance and speed calculation

#include "../../includes/define.h"

int distance(int duration, int avg_speed) {
    return duration * avg_speed;
}

double avg_speed(int pulse, int loop) {
int avg_pulse = pulse/loop;
return speed(avg_pulse);
}

double speed(int pulse) {
    return (double)(50*pulse)/255;
}
