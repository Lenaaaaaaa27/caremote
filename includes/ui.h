//
// Created by vince on 05/01/2024.
//

#ifndef CAREMOTE_UI_H
#define CAREMOTE_UI_H
void activate(GtkApplication *, gpointer);
void on_profile_activate(GtkWidget *widget, gpointer user_data);
void on_add_profile_activate(GtkWidget *widget, gpointer user_data);
void on_edit_session_button_clicked(GtkButton *button, gpointer user_data);
void on_edit_profile_activate(GtkWidget *widget, gpointer user_data);
void on_edit_profile_clicked(GtkWidget *widget, gpointer user_data);
void on_edit_configuration_button_clicked(GtkButton *button, gpointer user_data);
void on_configuration_button_clicked(GtkButton *button, gpointer user_data);
void on_create_config_button_clicked(GtkButton *button, gpointer user_data);
void on_create_profile_clicked(GtkWidget *widget, gpointer user_data);
void on_chosen_configuration_button_clicked(GtkButton *button, gpointer user_data);
void on_session_button_clicked(GtkButton *button, gpointer user_data);
void on_delete_profile_activate(GtkWidget *widget, gpointer user_data);
void on_delete_it_activate(GtkMenuItem *menu_item, gpointer user_data);
void on_delete_session_button_clicked(GtkButton *button, gpointer user_data);
void on_delete_configuration_button_clicked(GtkButton *button, gpointer user_data);
void refresh_sessions_view(int profile_id);
void update_current_profile_label();
void updateChosenConfiguration();
void on_profile_menu_item_activate(GtkMenuItem *menu_item, gpointer user_data);
void refresh_configurations_view(int profile_id);
void arrayConfigurations(int id_profile);
void arraySessions(int id_profile);
gboolean cleanup(GtkWidget *widget, GdkEvent *event, gpointer user_data);
static gboolean on_window_button_press_event();
void update_speed_label(gdouble instantSpeed);
void* control_thread_function(void* data);
void on_start_session_clicked(GtkButton *button, gpointer user_data);
void on_stop_session_clicked(GtkButton *button, gpointer user_data);
void update_distance_label(int distance);
void update_duration_label(int duration);
void errorPopUp(char *errorContent);
void on_ok_button_clicked(GtkButton *button, gpointer user_data);
gboolean update_labels_callback(gpointer user_data);
#endif //CAREMOTE_UI_H
