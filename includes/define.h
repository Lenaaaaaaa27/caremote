//
// Created by vince on 28/11/2023.
//

#ifndef CAREMOTE_DEFINE_H
#define CAREMOTE_DEFINE_H
#include <Xinput.h>  // Bibliothèque pour la gestion des manettes Xbox
#include<string.h>
#include<stdlib.h>
#include <time.h>
#include "struct.h"
//control
void control(Configuration *, int);

//connexion
int initConnexion();
void closeConnexion(int);

//Error
void handle_error(const char*);
void write_log(const char*);
char * line_formatting(const char*);

//Data
double duration(time_t,time_t);
double avg_speed(int speed,double duration);
#define LINK "../caremote"
#endif //CAREMOTE_DEFINE_H
