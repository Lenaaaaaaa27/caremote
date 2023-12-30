#include "../../includes/define.h"

static GtkWidget *globalProfileSubmenu = NULL;
static GtkWidget *globalSessionSubmenu = NULL;
static GtkWidget *globalConfigurationSubmenu = NULL;
static gboolean is_profile_submenu_created = FALSE;
static gboolean is_session_submenu_created = FALSE;
static gboolean is_configuration_submenu_created = FALSE;

// Fonction pour fermer les sous-menus s'ils sont ouverts
static void close_submenus() {
    if (is_profile_submenu_created) {
        gtk_widget_destroy(globalProfileSubmenu);
        is_profile_submenu_created = FALSE;
    }

    if (is_session_submenu_created) {
        gtk_widget_destroy(globalSessionSubmenu);
        is_session_submenu_created = FALSE;
    }

    if (is_configuration_submenu_created) {
        gtk_widget_destroy(globalConfigurationSubmenu);
        is_configuration_submenu_created = FALSE;
    }
}

void on_profile_activate(GtkWidget *widget, gpointer user_data) {
    // Fermer les sous-menus s'ils sont ouverts
    close_submenus();

    Profile *profiles = get_profiles();

    // Si le sous-menu n'a pas été créé, créez-le
    if (!is_profile_submenu_created) {
        globalProfileSubmenu = gtk_menu_new();

        for (int i = 0; profiles[i].id != -1; ++i) {
            GtkWidget *profile_item = gtk_menu_item_new_with_label(profiles[i].username);
            gtk_menu_shell_append(GTK_MENU_SHELL(globalProfileSubmenu), profile_item);
            gtk_widget_show(profile_item);
        }

        is_profile_submenu_created = TRUE;
    }

    free(profiles);

    gtk_menu_popup_at_pointer(GTK_MENU(globalProfileSubmenu), NULL);
}

void on_session_activate(GtkWidget *widget, gpointer user_data) {
    // Fermer les sous-menus s'ils sont ouverts
    close_submenus();

    Session *sessions = get_sessions_by_profile_id(1);

    // Si le sous-menu n'a pas été créé, créez-le
    if (!is_session_submenu_created) {
        globalSessionSubmenu = gtk_menu_new();

        for (int i = 0; sessions[i].id != -1; ++i) {
            GtkWidget *session_item = gtk_menu_item_new_with_label(sessions[i].name);
            gtk_menu_shell_append(GTK_MENU_SHELL(globalSessionSubmenu), session_item);
            gtk_widget_show(session_item);
        }

        is_session_submenu_created = TRUE;
    }

    free(sessions);

    gtk_menu_popup_at_pointer(GTK_MENU(globalSessionSubmenu), NULL);
}

void on_configuration_activate(GtkWidget *widget, gpointer user_data) {
    // Fermer les sous-menus s'ils sont ouverts
    close_submenus();

    Configuration *configuration = get_configurations(1);

    // Si le sous-menu n'a pas été créé, créez-le
    if (!is_configuration_submenu_created) {
        globalConfigurationSubmenu = gtk_menu_new();

        for (int i = 0; configuration[i].id != -1; ++i) {
            GtkWidget *configuration_item = gtk_menu_item_new_with_label(configuration[i].name);
            gtk_menu_shell_append(GTK_MENU_SHELL(globalConfigurationSubmenu), configuration_item);
            gtk_widget_show(configuration_item);
        }

        is_configuration_submenu_created = TRUE;
    }

    free(configuration);

    gtk_menu_popup_at_pointer(GTK_MENU(globalConfigurationSubmenu), NULL);
}

// Gestionnaire d'événements pour fermer les sous-menus lorsqu'un clic est effectué ailleurs
static gboolean on_window_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer user_data) {
    close_submenus();
    return FALSE;
}

void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *profile;
    GtkWidget *session;
    GtkWidget *configuration;
    GtkBuilder *builder;

    builder = gtk_builder_new_from_file("index.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), user_data);
    g_signal_connect(window, "button-press-event", G_CALLBACK(on_window_button_press_event), NULL);

    gtk_builder_connect_signals(builder, user_data);

    profile = GTK_WIDGET(gtk_builder_get_object(builder, "profile"));
    session = GTK_WIDGET(gtk_builder_get_object(builder, "sessions"));
    configuration = GTK_WIDGET(gtk_builder_get_object(builder, "configuration"));

    g_signal_connect(profile, "button_press_event", G_CALLBACK(on_profile_activate), window);
    g_signal_connect(session, "button_press_event", G_CALLBACK(on_session_activate), window);
    g_signal_connect(configuration, "button_press_event", G_CALLBACK(on_configuration_activate), window);

    g_object_unref(builder);

    gtk_widget_show_all(window);
    gtk_main();
}
