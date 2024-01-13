//
// Created by vince on 28/11/2023.
//

#ifndef CAREMOTE_DEFINE_H
#define CAREMOTE_DEFINE_H
#include <Xinput.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <time.h>
#include <sqlite3.h>
#include "struct.h"
#include "ui.h"
#include <pthread.h>
#include <windows.h>
#include <cJSON.h>
#include <commdlg.h>
//toolbox.c
char * truncated_string(char*,int);
//control
int control(Configuration *, int, int);

//connexion
int initConnexion();
void closeConnexion(int);

//Error
void handle_error(const char*);
void write_log(const char*);
char * line_formatting(const char*);

//Data
int distance(int, int);
double avg_speed(int, int);
double speed(int);

//database
int create_database();
int verification_database();
int create_profile(char username[20]);
Profile *get_profiles();
Profile get_profile(int id_profile);
int update_profile(int id, char username[30]);
int delete_profile(int id);
int create_configuration(Configuration *configuration);
boolean does_configuration_exist_with_id(int id_configuration);
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

int setConfig(const char *,Setting *);
int exportConfig(Configuration *);
int exportSession(Session *);
char *intToDate(int);
int importsConfig(int);
int extractIntValue(cJSON *, const char *, int *);
int extractCharValue(cJSON *, const char *, char *);
int extractStringValue(cJSON *, const char *, char *, size_t);
//variable stop
extern int fin;
extern pthread_mutex_t fin_mutex;



#endif //CAREMOTE_DEFINE_H
