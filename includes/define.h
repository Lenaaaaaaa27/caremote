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
//control
int control(Configuration *configuration, int id_user, int clientSocket);

//connexion
int initConnexion();
void closeConnexion(int);

//Error
void write_log(const char*);
void line_formatting(const char* content, char *line);

//Data
int distance(int, int);
double avg_speed(int, int);
double speed(int);

//database
int create_database();
int verification_database();

//profile
boolean does_profile_exist_with_id(int id_configuration);
int create_profile(char username[20]);
Profile *get_profiles();
Profile get_profile(int id_profile);
int update_profile(int id, char username[30]);
int delete_profile(int id);

//configuration
int create_configuration(Configuration *configuration);
boolean does_configuration_exist_with_id(int id_configuration);
Configuration *get_configurations(int id_profile);
Configuration get_configuration(int id_configuration);
int update_configuration(Configuration configuration);
int delete_configuration(int id_configuration);

//session
int create_session(Session session);
Session *get_sessions_by_profile_id(int id_profile);
Session get_session(int id_session);
int update_session(Session session);
int delete_session(int id);
void error_content(int);

int setConfig(const char *);
int exportConfig(Configuration *);
int exportSessions(Session[],int);
char *intToDate(int);
int importsConfig(int);
int extractIntValue(cJSON *, const char *, int *);
int extractCharValue(cJSON *, const char *, char *);
int extractStringValue(cJSON *, const char *, char *, size_t);
//variable stop
extern int fin;
extern pthread_mutex_t fin_mutex;
extern Setting *settings;

#endif //CAREMOTE_DEFINE_H
