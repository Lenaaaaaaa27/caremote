//
// Created by vince on 28/11/2023.
//

#ifndef CAREMOTE_DEFINE_H
#define CAREMOTE_DEFINE_H
#include "../../includes/struct_control.h"

#include<string.h>
#include<stdlib.h>
#include "struct.h"
//control
void init_control(Control *);
void control();

//connexion
int initConnexion();
void closeConnexion(int);

//Error
void handle_error(const char*);
void write_log(const char*);
char * line_formatting(const char*);

#define LINK "../caremote"
#endif //CAREMOTE_DEFINE_H
