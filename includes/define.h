//
// Created by vince on 28/11/2023.
//

#ifndef CAREMOTE_DEFINE_H
#define CAREMOTE_DEFINE_H

#include<string.h>
#include<stdlib.h>
#include<gtk/gtk.h>
#include "struct.h"
//toolbox.c
char * truncated_string(char*,int);

//fatal_errors.c
void error_content(int);
void handle_error(const char*);
void write_log(const char*);
char * line_formatting(const char*);
void control();
void on_profile_activate(GtkWidget *, gpointer);
void activate(GtkApplication *, gpointer);
void on_add_profile_activate(GtkWidget *widget, gpointer user_data);
void on_profile_menu_item_hover(GtkWidget *widget, GdkEvent *event, gpointer user_data);
gboolean on_profile_menu_item_hover_timeout(gpointer user_data);
void on_edit_session_button_clicked(GtkButton *button, gpointer user_data);
void on_session_button_clicked(GtkButton *button, gpointer user_data);
void on_create_profile_clicked(GtkWidget *widget, gpointer user_data);
void on_delete_session_button_clicked(GtkButton *button, gpointer user_data);
void refresh_sessions_view(int profile_id);
void refresh_configurations_view(int profile_id);
void arrayConfigurations(int id_profile);
void arraySessions(int id_profile);
static gboolean on_window_button_press_event();
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
#define LINK "../caremote"
#endif //CAREMOTE_DEFINE_H
