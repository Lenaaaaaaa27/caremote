//
// Created by vince on 28/11/2023.
//

#ifndef CAREMOTE_DEFINE_H
#define CAREMOTE_DEFINE_H
#include "../../includes/struct_control.h"

#include<string.h>
#include<stdlib.h>
#include "struct.h"
#include "../src/Errors/fatal_errors.c"
//control
void init_control(Control *);
void control();

#define LINK "../caremote"
#endif //CAREMOTE_DEFINE_H
