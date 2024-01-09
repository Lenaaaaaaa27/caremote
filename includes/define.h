//
// Created by vince on 28/11/2023.
//

#ifndef CAREMOTE_DEFINE_H
#define CAREMOTE_DEFINE_H
#include <Xinput.h>  // Bibliothèque pour la gestion des manettes Xbox
#include<string.h>
#include<stdlib.h>
#include<gtk/gtk.h>
#include <time.h>
#include "struct.h"
#include "ui.h"
//toolbox.c
char * truncated_string(char*,int);
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
int distance(int, int);
double avg_speed(int, int);
double speed(int);

//fatal_errors.c
void control();
int create_profile(char username[20]);
Profile *get_profiles();
Profile get_profile(int id_profile);
int update_profile(int id, char username[30]);
int delete_profile(int id);
int create_configuration(int id_profile);
Configuration *get_configurations(int id_profile);
Configuration get_configuration(int id_configuration);
int update_configuration(Configuration configuration);
int delete_configuration(int id_configuration);
int create_session(Session session);
Session *get_sessions_by_profile_id(int id_profile);
Session get_session(int id_session);
int update_session(Session session);
int delete_session(int id);
void error_content(int);
#define LINK "../caremote"
#endif //CAREMOTE_DEFINE_H
