//
// Created by arthur on 07/01/2024.
//

#include "../../includes/define.h"


double duration(time_t start_date,time_t end_date) {
    return difftime(end_date, start_date);
}

int distance(int duration, int avg_speed) {
    return duration * avg_speed;
}

double avg_speed(int speed,double duration) {
return (double)speed/duration;
}