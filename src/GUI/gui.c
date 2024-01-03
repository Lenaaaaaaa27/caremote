#include "../../includes/define.h"

static GtkBuilder *globalBuilder = NULL;
static int current_profile_id = 1;

gboolean is_session_window_destroyed = FALSE;
static GtkWidget *globalProfileSubmenu = NULL;
GtkWidget *submenuProfile = NULL;
static gboolean is_profile_submenu_created = FALSE;

static GtkWidget *addProfileWindow = NULL;
static GtkWidget *sessionWindow = NULL;
static GtkEntry *entry = NULL;
gboolean enterPressed = FALSE;

static void close_submenus() {
    if (is_profile_submenu_created) {
        gtk_widget_destroy(globalProfileSubmenu);
        is_profile_submenu_created = FALSE;
    }
}

static gboolean on_window_button_press_event() {
    close_submenus();
    return FALSE;
}

void cleanup() {
    if(addProfileWindow !=NULL){
        gtk_widget_hide(addProfileWindow);
        entry = NULL;
        g_clear_pointer(&addProfileWindow, gtk_widget_destroy);
        enterPressed = FALSE;
    }

    if(sessionWindow != NULL){
        gtk_widget_hide(sessionWindow);
        sessionWindow = NULL;
    }
}

void on_profile_menu_item_hover(GtkWidget *widget, GdkEvent *event, gpointer user_data) {
    // Cette fonction est appelée lorsque l'utilisateur survole un élément du menu

    submenuProfile = gtk_menu_new();
    // Ajouter les sous-menus "Switch On", "Edit", "Delete"
    GtkWidget *switch_on_item = gtk_menu_item_new_with_label("Switch On");
    GtkWidget *edit_item = gtk_menu_item_new_with_label("Edit");
    GtkWidget *delete_item = gtk_menu_item_new_with_label("Delete");

    //g_signal_connect(switch_on_item, "activate", G_CALLBACK(on_switch_on_activate), user_data);
    //g_signal_connect(edit_item, "activate", G_CALLBACK(on_edit_activate), user_data);
    //g_signal_connect(delete_item, "activate", G_CALLBACK(on_delete_activate), user_data);

    gtk_menu_shell_append(GTK_MENU_SHELL(submenuProfile), switch_on_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(submenuProfile), edit_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(submenuProfile), delete_item);

    gtk_widget_show(switch_on_item);
    gtk_widget_show(edit_item);
    gtk_widget_show(delete_item);

    // Afficher le sous-menu
    gtk_menu_popup_at_pointer(GTK_MENU(submenuProfile), NULL);
}

void on_profile_menu_item_activate(GtkMenuItem *menu_item, gpointer user_data) {
    // Récupérer l'ID du profil associé à l'élément de menu
    gint profile_id = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(menu_item), "user_id"));

    current_profile_id = profile_id;

    // Rafraîchir la vue des sessions avec le nouvel ID de profil
    refresh_sessions_view(current_profile_id);
    refresh_configurations_view(current_profile_id);
}

void on_profile_activate(GtkWidget *widget, gpointer user_data) {
    close_submenus();
    Profile *profiles = get_profiles();

    if (!is_profile_submenu_created) {
        globalProfileSubmenu = gtk_menu_new();

        for (int i = 0; profiles[i].id != -1; ++i) {
            GtkWidget *profile_item = gtk_menu_item_new_with_label(profiles[i].username);
            g_object_set_data(G_OBJECT(profile_item), "user_id", GINT_TO_POINTER(profiles[i].id));

            // Connecter l'événement "activate" à la nouvelle fonction
            g_signal_connect(profile_item, "activate", G_CALLBACK(on_profile_menu_item_activate), user_data);

            gtk_menu_shell_append(GTK_MENU_SHELL(globalProfileSubmenu), profile_item);
            gtk_widget_show(profile_item);
        }

        GtkWidget *separator = gtk_separator_menu_item_new();
        gtk_menu_shell_append(GTK_MENU_SHELL(globalProfileSubmenu), separator);
        gtk_widget_show(separator);

        GtkWidget *add_profile_item = gtk_menu_item_new_with_label("Add a profile");
        gtk_menu_shell_append(GTK_MENU_SHELL(globalProfileSubmenu), add_profile_item);
        g_signal_connect(add_profile_item, "activate", G_CALLBACK(on_add_profile_activate), user_data);
        gtk_widget_show(add_profile_item);

        is_profile_submenu_created = TRUE;
    }

    free(profiles);

    gtk_menu_popup_at_pointer(GTK_MENU(globalProfileSubmenu), NULL);
}


void on_add_profile_activate(GtkWidget *widget, gpointer user_data) {

    if (addProfileWindow == NULL) {
        addProfileWindow = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "addProfile"));
        if (entry == NULL) {
            entry = GTK_ENTRY(gtk_builder_get_object(globalBuilder, "Addprofileinput"));
        }
        gtk_entry_set_text(GTK_ENTRY(entry), "");
        g_signal_connect(entry, "activate", G_CALLBACK(on_create_profile_clicked), NULL);
    }

    gtk_widget_show_all(addProfileWindow);
}

void on_create_profile_clicked(GtkWidget *widget, gpointer user_data) {

    const gchar *username = gtk_entry_get_text(entry);
    char *username_copy = g_strdup(username);

    create_profile(username_copy);
    g_free(username_copy);
    cleanup();
}

void on_session_button_clicked(GtkButton *button, gpointer user_data) {
    if (sessionWindow == NULL) {
        sessionWindow = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "sessionWindow"));
        g_signal_connect(sessionWindow, "destroy", G_CALLBACK(cleanup), NULL);
    }

    gint session_id = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(button), "session_id"));
    Session session = get_session(session_id);
    Configuration config = get_configuration(session.id_configuration);

    GtkWidget *sessionNameInput = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "sessionName"));
    GtkWidget *sessionDurationLabel = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "sessionDuration"));
    GtkWidget *sessionDistanceLabel = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "sessionDistance"));
    GtkWidget *sessionMaxSpeedLabel = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "sessionMaxSpeed"));
    GtkWidget *sessionAverageSpeedLabel = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "sessionAverageSpeed"));
    GtkWidget *sessionTimeStartLabel = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "sessionTimeStart"));
    GtkWidget *sessionConfigurationLabel = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "sessionConfiguration"));
    GtkWidget *editSessionButton = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "editSessionButton"));
    GtkWidget *deleteSessionButton = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "deleteSessionButton"));

    gtk_entry_set_text(GTK_ENTRY(sessionNameInput), session.name);
    gtk_label_set_text(GTK_LABEL(sessionDurationLabel), g_strdup_printf("%d", session.duration));
    gtk_label_set_text(GTK_LABEL(sessionDistanceLabel), g_strdup_printf("%d", session.distance));
    gtk_label_set_text(GTK_LABEL(sessionMaxSpeedLabel), g_strdup_printf("%.2lf",session.max_speed));
    gtk_label_set_text(GTK_LABEL(sessionAverageSpeedLabel), g_strdup_printf("%.2lf",session.average_speed));
    gtk_label_set_text(GTK_LABEL(sessionTimeStartLabel), session.time_start);
    gtk_label_set_text(GTK_LABEL(sessionConfigurationLabel), config.name);

    g_signal_connect(editSessionButton, "clicked", G_CALLBACK(on_edit_session_button_clicked), GINT_TO_POINTER(session_id));
    g_signal_connect(deleteSessionButton, "clicked", G_CALLBACK(on_delete_session_button_clicked), GINT_TO_POINTER(session_id));

    gtk_widget_show(sessionWindow);
}



void on_delete_session_button_clicked(GtkButton *button, gpointer user_data) {
    gint session_id = GPOINTER_TO_INT(user_data);

    g_print("%d", session_id);
    delete_session(session_id);

    refresh_sessions_view(current_profile_id);
    gtk_widget_hide(sessionWindow);
}

void on_edit_session_button_clicked(GtkButton *button, gpointer user_data) {
    gint session_id = GPOINTER_TO_INT(user_data);
    Session session = get_session(session_id);

    GtkWidget *sessionNameInput = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "sessionName"));

    const gchar *new_session_name = gtk_entry_get_text(GTK_ENTRY(sessionNameInput));
    g_strlcpy(session.name, new_session_name, sizeof(session.name));

    update_session(session);
    refresh_sessions_view(current_profile_id);
    gtk_widget_hide(sessionWindow);
}

void refresh_sessions_view(int profile_id) {
    GtkWidget *viewportsession = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "viewportsession"));

    GList *children, *iter;
    children = gtk_container_get_children(GTK_CONTAINER(viewportsession));
    for (iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    Session *sessions = get_sessions_by_profile_id(profile_id);

    int row = 1;

    for (int i = 0; sessions[i].id != -1; ++i) {
        GtkWidget *button = gtk_button_new_with_label(sessions[i].name);

        gtk_widget_set_size_request(button, 360, 120);

        g_object_set_data(G_OBJECT(button), "session_id", GINT_TO_POINTER(sessions[i].id));

        g_signal_connect(button, "clicked", G_CALLBACK(on_session_button_clicked), NULL);

        gtk_grid_attach(GTK_GRID(grid), button, 0, row, 1, 1);

        row++;
    }

    gtk_container_add(GTK_CONTAINER(viewportsession), grid);

    gtk_widget_show_all(viewportsession);
    free(sessions);
}

void arraySessions(int profile_id) {
    GtkWidget *viewportsession;
    GtkWidget *button;

    viewportsession = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "viewportsession"));

    // Créer un GtkGrid pour contenir le label et les boutons de session
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    Session *sessions = get_sessions_by_profile_id(profile_id);

    int row = 1;  // Commencer à la deuxième ligne pour les boutons

    for (int i = 0; sessions[i].id != -1; ++i) {
        button = gtk_button_new_with_label(sessions[i].name);

        gtk_widget_set_size_request(button, 360, 120);

        g_object_set_data(G_OBJECT(button), "session_id", GINT_TO_POINTER(sessions[i].id));

        g_signal_connect(button, "clicked", G_CALLBACK(on_session_button_clicked), NULL);

        gtk_grid_attach(GTK_GRID(grid), button, 0, row, 1, 1);

        row++;
    }

    gtk_container_add(GTK_CONTAINER(viewportsession), grid);

    gtk_widget_show_all(viewportsession);
    free(sessions);
}

void refresh_configurations_view(int profile_id) {
    GtkWidget *viewportconfiguration = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "viewportconfiguration"));

    // Supprimer tous les enfants du conteneur
    GList *children, *iter;
    children = gtk_container_get_children(GTK_CONTAINER(viewportconfiguration));
    for (iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);

    // Créer un GtkGrid pour contenir les nouveaux boutons de configuration
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    Configuration *configurations = get_configurations(profile_id);

    int row = 1;  // Commencer à la deuxième ligne pour les boutons

    for (int i = 0; configurations[i].id != -1; ++i) {
        GtkWidget *button = gtk_button_new_with_label(configurations[i].name);

        gtk_widget_set_size_request(button, 360, 120);

        gtk_grid_attach(GTK_GRID(grid), button, 0, row, 1, 1);

        row++;
    }

    gtk_container_add(GTK_CONTAINER(viewportconfiguration), grid);

    gtk_widget_show_all(viewportconfiguration);
    free(configurations);
}

void arrayConfigurations(int profile_id) {
    GtkWidget *viewportconfiguration;
    GtkWidget *button;

    viewportconfiguration = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "viewportconfiguration"));

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    Configuration *configurations = get_configurations(profile_id);

    int row = 1;

    for (int i = 0; configurations[i].id != -1; ++i) {
        button = gtk_button_new_with_label(configurations[i].name);

        gtk_widget_set_size_request(button, 360, 120);

        gtk_grid_attach(GTK_GRID(grid), button, 0, row, 1, 1);

        row++;
    }

    gtk_container_add(GTK_CONTAINER(viewportconfiguration), grid);

    gtk_widget_show_all(viewportconfiguration);
    free(configurations);
}

void destroyWindow(GtkWidget *widget, gpointer user_data){
    g_object_unref(globalBuilder);
}

void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *profile;

    globalBuilder = gtk_builder_new_from_file("index.glade");
    window = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "window"));

    GError *error = NULL;
    GdkPixbuf *icon = gdk_pixbuf_new_from_file("car.png", &error);

    if (error != NULL) {
        g_printerr("Error loading icon: %s\n", error->message);
        g_error_free(error);
    } else {
        gtk_window_set_icon(GTK_WINDOW(window), icon);
    }

    g_signal_connect(window, "destroy", G_CALLBACK(destroyWindow), user_data);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), user_data);
    g_signal_connect(window, "button-press-event", G_CALLBACK(on_window_button_press_event), NULL);

    profile = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "profile"));

    arraySessions(current_profile_id);
    arrayConfigurations(current_profile_id);

    g_signal_connect(profile, "button_press_event", G_CALLBACK(on_profile_activate), window);

    gtk_widget_show_all(window);
    gtk_main();
}
