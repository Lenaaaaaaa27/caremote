//
// Created by Lena on 05/12/2023.
//
// it allows you to get all the widgets and interactions with them, windows of the user interface.


#include "../../includes/define.h"

static GtkBuilder *globalBuilder = NULL;

int clientSocket;
int end = 0;
pthread_mutex_t end_mutex = PTHREAD_MUTEX_INITIALIZER;

static int current_profile_id;
int chosen_config_id = -1;

static GtkWidget *globalProfileSubmenu = NULL;
static gboolean is_profile_submenu_created = FALSE;

static GtkWidget *globalDeleteProfileSubmenu = NULL;
static gboolean is_delete_profile_submenu_created = FALSE;

static GtkWidget *globalExportSubmenu = NULL;
static gboolean is_export_submenu_created = FALSE;

static void close_deleted_submenus() {
    if (is_delete_profile_submenu_created) {
        gtk_widget_destroy(globalDeleteProfileSubmenu);
        is_delete_profile_submenu_created = FALSE;
    }
}

static void close_exportation_submenus(){
    if (is_export_submenu_created) {
        gtk_widget_destroy(globalExportSubmenu);
        is_export_submenu_created = FALSE;
    }
}

void close_submenus() {
    if (is_profile_submenu_created) {
        gtk_widget_destroy(globalProfileSubmenu);
        is_profile_submenu_created = FALSE;
    }
}

static gboolean on_window_button_press_event() {
    close_submenus();
    close_deleted_submenus();
    close_exportation_submenus();
    return FALSE;
}

gboolean cleanup(GtkWidget *widget, GdkEvent *event, gpointer user_data) {
    GetSession *getsession = (GetSession *)user_data;
    g_object_unref(getsession->builder);
    return FALSE;
}

void on_profile_menu_item_activate(GtkMenuItem *menu_item, gpointer user_data) {
    gint profile_id = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(menu_item), "user_id"));
    if(current_profile_id != profile_id){
        chosen_config_id = -1;
    }
    current_profile_id = profile_id;
    update_current_profile_label();
    updateChosenConfiguration();

    refresh_sessions_view(current_profile_id);
    refresh_configurations_view(current_profile_id);
}

void on_export_configuration_activate(GtkWidget *widget, gpointer user_data){

    if(chosen_config_id == -1){
        errorPopUp("You have to select a configuration before exporting it !");
        return;
    }
    Configuration config = get_configuration(chosen_config_id);
    if(exportConfig(&config) == EXIT_FAILURE){
        errorPopUp("Failure to export configuration");
    }
}

void on_export_sessions_activate(GtkWidget *widget, gpointer user_data){
    Session *sessions = get_sessions_by_profile_id(current_profile_id);
    int count;
    for (count = 0; sessions[count].id != -1; ++count);
    if(exportSessions(sessions, count)){
        errorPopUp("Failure to export sessions");
    }
}

void on_import_configuration_activate(GtkWidget *widget, gpointer user_data){

    if(importsConfig(current_profile_id)){
        errorPopUp("Failure to import configuration");
        return;
    }

    refresh_configurations_view(current_profile_id);
    refresh_sessions_view(current_profile_id);
}

void on_exportations_activate(GtkWidget *widget, gpointer user_data){
    close_submenus();
    close_deleted_submenus();
    close_exportation_submenus();

    if (!is_export_submenu_created) {
        globalExportSubmenu = gtk_menu_new();
        gtk_style_context_add_class(gtk_widget_get_style_context(globalExportSubmenu), "MenuBar");


        GtkWidget *importation = gtk_menu_item_new_with_label("Import a configuration from JSON");
        gtk_menu_shell_append(GTK_MENU_SHELL(globalExportSubmenu), importation);
        gtk_style_context_add_class(gtk_widget_get_style_context(importation), "MenuBar");
        g_signal_connect(importation, "activate", G_CALLBACK(on_import_configuration_activate), user_data);
        gtk_widget_show(importation);

        GtkWidget *exportation = gtk_menu_item_new_with_label("Export your chosen configuration");
        gtk_menu_shell_append(GTK_MENU_SHELL(globalExportSubmenu), exportation);
        gtk_style_context_add_class(gtk_widget_get_style_context(exportation), "MenuBar");
        g_signal_connect(exportation, "activate", G_CALLBACK(on_export_configuration_activate), NULL);
        gtk_widget_show(exportation);

        GtkWidget *separator = gtk_separator_menu_item_new();
        gtk_style_context_add_class(gtk_widget_get_style_context(separator), "separator");
        gtk_menu_shell_append(GTK_MENU_SHELL(globalExportSubmenu), separator);
        gtk_widget_show(separator);

        GtkWidget *exportationSession = gtk_menu_item_new_with_label("Export your sessions");
        gtk_menu_shell_append(GTK_MENU_SHELL(globalExportSubmenu), exportationSession);
        gtk_style_context_add_class(gtk_widget_get_style_context(exportationSession), "MenuBar");
        g_signal_connect(exportationSession, "activate", G_CALLBACK(on_export_sessions_activate), NULL);
        gtk_widget_show(exportationSession);

        is_export_submenu_created = TRUE;
    }

    gtk_menu_popup_at_pointer(GTK_MENU(globalExportSubmenu), NULL);
}
void on_delete_profile_activate(GtkWidget *widget, gpointer user_data){
    close_submenus();
    close_deleted_submenus();
    close_exportation_submenus();

    Profile *profiles = get_profiles();

    if (!is_delete_profile_submenu_created) {
        globalDeleteProfileSubmenu = gtk_menu_new();
        gtk_style_context_add_class(gtk_widget_get_style_context(globalDeleteProfileSubmenu), "MenuBar");

        for (int i = 0; profiles[i].id != -1; ++i) {
            if (profiles[i].id != 1 && profiles[i].id != current_profile_id) {
                GtkWidget *profile_item = gtk_menu_item_new_with_label(profiles[i].username);
                g_object_set_data(G_OBJECT(profile_item), "user_id", GINT_TO_POINTER(profiles[i].id));
                gtk_style_context_add_class(gtk_widget_get_style_context(profile_item), "MenuBar");
                g_signal_connect(profile_item, "activate", G_CALLBACK(on_delete_it_activate), user_data);
                gtk_menu_shell_append(GTK_MENU_SHELL(globalDeleteProfileSubmenu ), profile_item);
                gtk_widget_show(profile_item);
            }
        }

        is_delete_profile_submenu_created = TRUE;
    }
    free(profiles);
    gtk_menu_popup_at_pointer(GTK_MENU(globalDeleteProfileSubmenu), NULL);
}

void on_curl_activate(GtkWidget *widget, gpointer user_data){
    int error = curl();

    if(error == 2){
        errorPopUp("Curl didn't work. Check your connection");
        return;
    }

    if(error == EXIT_FAILURE){
        errorPopUp("Export of caremote_car failed");
    }
}

void on_profile_activate(GtkWidget *widget, gpointer user_data) {
    close_submenus();
    close_deleted_submenus();
    close_exportation_submenus();

    Profile *profiles = get_profiles();

    if (!is_profile_submenu_created) {
        globalProfileSubmenu = gtk_menu_new();
        gtk_style_context_add_class(gtk_widget_get_style_context(globalProfileSubmenu), "MenuBar");

        for (int i = 0; profiles[i].id != -1; ++i) {
            GtkWidget *profile_item = gtk_menu_item_new_with_label(profiles[i].username);
            g_object_set_data(G_OBJECT(profile_item), "user_id", GINT_TO_POINTER(profiles[i].id));
            gtk_style_context_add_class(gtk_widget_get_style_context(profile_item), "MenuBar");
            g_signal_connect(profile_item, "activate", G_CALLBACK(on_profile_menu_item_activate), user_data);
            gtk_menu_shell_append(GTK_MENU_SHELL(globalProfileSubmenu), profile_item);
            gtk_widget_show(profile_item);
        }

        GtkWidget *separator = gtk_separator_menu_item_new();
        gtk_style_context_add_class(gtk_widget_get_style_context(separator), "separator");
        gtk_menu_shell_append(GTK_MENU_SHELL(globalProfileSubmenu), separator);
        gtk_widget_show(separator);

        GtkWidget *edit_profile_item = gtk_menu_item_new_with_label("Edit your current profile");
        gtk_menu_shell_append(GTK_MENU_SHELL(globalProfileSubmenu), edit_profile_item);
        gtk_style_context_add_class(gtk_widget_get_style_context(edit_profile_item), "MenuBar");
        g_signal_connect(edit_profile_item, "activate", G_CALLBACK(on_edit_profile_activate), user_data);
        gtk_widget_show(edit_profile_item);

        GtkWidget *add_profile_item = gtk_menu_item_new_with_label("Add a profile");
        gtk_menu_shell_append(GTK_MENU_SHELL(globalProfileSubmenu), add_profile_item);
        gtk_style_context_add_class(gtk_widget_get_style_context(add_profile_item), "MenuBar");
        g_signal_connect(add_profile_item, "activate", G_CALLBACK(on_add_profile_activate), NULL);
        gtk_widget_show(add_profile_item);

        is_profile_submenu_created = TRUE;
    }

    free(profiles);

    gtk_menu_popup_at_pointer(GTK_MENU(globalProfileSubmenu), NULL);
}

void on_edit_profile_activate(GtkWidget *widget, gpointer user_data){
    GetSession *editUser = (GetSession *)malloc(sizeof(GetSession));
    editUser->builder = gtk_builder_new_from_file("files/editProfile.glade");
    GError *error = NULL;
    GtkWidget *editProfileWindow = GTK_WIDGET(gtk_builder_get_object(editUser->builder, "editProfile"));
    GtkWidget *entry = GTK_WIDGET(gtk_builder_get_object(editUser->builder, "editProfileinput"));

    gtk_entry_set_text(GTK_ENTRY(entry), "");
    g_signal_connect(entry, "activate", G_CALLBACK(on_edit_profile_clicked), editUser);
    g_signal_connect(editProfileWindow, "delete-event", G_CALLBACK(cleanup), editUser);

    GdkPixbuf *icon = gdk_pixbuf_new_from_file("files/car.png", &error);
    gtk_window_set_icon(GTK_WINDOW(editProfileWindow), icon);

    gtk_widget_show(GTK_WIDGET(editProfileWindow));
}

void on_edit_profile_clicked(GtkWidget *widget, gpointer user_data){
    GetSession *editUsername = (GetSession *)user_data;
    GtkBuilder *builder = editUsername->builder;

    GtkWidget *entry = GTK_WIDGET(gtk_builder_get_object(builder, "editProfileinput"));
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(entry));
    char *username_copy = g_strdup(username);

    if (strlen(username_copy) == 0) {
        GtkWidget *errorLabel = GTK_WIDGET(gtk_builder_get_object(editUsername->builder, "errorLabel"));
        gtk_label_set_text(GTK_LABEL(errorLabel), "You must give a name to your current profile");
        gtk_widget_show(errorLabel);
    }else{
        update_profile(current_profile_id, username_copy);
        g_free(username_copy);
        update_current_profile_label();
        GtkWidget *editProfileWindow = GTK_WIDGET(gtk_builder_get_object(builder, "editProfile"));
        gtk_widget_destroy(editProfileWindow);
        free(editUsername);
    }

}

void on_add_profile_activate(GtkWidget *widget, gpointer user_data) {

    GetSession *addUser = (GetSession *)malloc(sizeof(GetSession));
    GError *error = NULL;
    GdkPixbuf *icon = gdk_pixbuf_new_from_file("files/car.png", &error);

    addUser->builder = gtk_builder_new_from_file("files/addProfile.glade");

    GtkWidget *addProfileWindow = GTK_WIDGET(gtk_builder_get_object(addUser->builder, "addProfile"));

    GtkWidget *entry = GTK_WIDGET(gtk_builder_get_object(addUser->builder, "Addprofileinput"));
    gtk_entry_set_text(GTK_ENTRY(entry), "");
    g_signal_connect(entry, "activate", G_CALLBACK(on_create_profile_clicked), addUser);
    g_signal_connect(addProfileWindow, "delete-event", G_CALLBACK(cleanup), addUser);

    gtk_window_set_icon(GTK_WINDOW(addProfileWindow), icon);
    gtk_widget_show(GTK_WIDGET(addProfileWindow));
}

void on_create_profile_clicked(GtkWidget *widget, gpointer user_data) {
    GetSession *getUsername = (GetSession *)user_data;
    GtkBuilder *builder = getUsername->builder;

    GtkWidget *entry = GTK_WIDGET(gtk_builder_get_object(builder, "Addprofileinput"));
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(entry));
    char *username_copy = g_strdup(username);

    if (strlen(username_copy) == 0) {
        GtkWidget *errorLabel = GTK_WIDGET(gtk_builder_get_object(getUsername->builder, "errorLabel"));
        gtk_label_set_text(GTK_LABEL(errorLabel), "You must give a name to your new profile");
        gtk_widget_show(errorLabel);
    }else{
        create_profile(username_copy);
        g_free(username_copy);

        GtkWidget *addProfileWindow = GTK_WIDGET(gtk_builder_get_object(builder, "addProfile"));
        gtk_widget_destroy(addProfileWindow);
        free(getUsername);
    }

}

void on_delete_it_activate(GtkMenuItem *menu_item, gpointer user_data) {
    gint profile_id = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(menu_item), "user_id"));
    delete_profile(profile_id);
}

void on_session_button_clicked(GtkButton *button, gpointer user_data) {
    char *configName;
    GError *error = NULL;
    GdkPixbuf *icon = gdk_pixbuf_new_from_file("files/car.png", &error);
    GetSession *getsession = (GetSession *)malloc(sizeof(GetSession));
    getsession->builder = gtk_builder_new_from_file("files/session.glade");
    GtkWidget *sessionWindow = GTK_WIDGET(gtk_builder_get_object(getsession->builder, "sessionWindow"));

    gint session_id = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(button), "session_id"));

    getsession->id_session = session_id;
    Session session = get_session(session_id);
    if(does_configuration_exist_with_id(session.id_configuration)){
        Configuration config = get_configuration(session.id_configuration);
        configName = config.name;
    }else{
        configName = "Deleted configuration";
    }

    int hours = session.duration / 3600;
    int minutes = (session.duration % 3600) / 60;
    int secondes = session.duration % 60;

    GtkWidget *sessionNameInput = GTK_WIDGET(gtk_builder_get_object(getsession->builder, "sessionName"));
    GtkWidget *sessionDurationLabel = GTK_WIDGET(gtk_builder_get_object(getsession->builder, "sessionDuration"));
    GtkWidget *sessionDistanceLabel = GTK_WIDGET(gtk_builder_get_object(getsession->builder, "sessionDistance"));
    GtkWidget *sessionMaxSpeedLabel = GTK_WIDGET(gtk_builder_get_object(getsession->builder, "sessionMaxSpeed"));
    GtkWidget *sessionAverageSpeedLabel = GTK_WIDGET(gtk_builder_get_object(getsession->builder, "sessionAverageSpeed"));
    GtkWidget *sessionTimeStartLabel = GTK_WIDGET(gtk_builder_get_object(getsession->builder, "sessionTimeStart"));
    GtkWidget *sessionConfigurationLabel = GTK_WIDGET(gtk_builder_get_object(getsession->builder, "sessionConfiguration"));
    GtkWidget *editSessionButton = GTK_WIDGET(gtk_builder_get_object(getsession->builder, "editSessionButton"));
    GtkWidget *deleteSessionButton = GTK_WIDGET(gtk_builder_get_object(getsession->builder, "deleteSessionButton"));

    gtk_entry_set_text(GTK_ENTRY(sessionNameInput), session.name);
    gtk_label_set_text(GTK_LABEL(sessionDurationLabel), g_strdup_printf("%dh %dmin %ds", hours, minutes, secondes));
    gtk_label_set_text(GTK_LABEL(sessionDistanceLabel), g_strdup_printf("%d cm", session.distance));
    gtk_label_set_text(GTK_LABEL(sessionAverageSpeedLabel), g_strdup_printf("%.2lf cm/s",session.average_speed));
    gtk_label_set_text(GTK_LABEL(sessionTimeStartLabel), session.time_start);
    gtk_label_set_text(GTK_LABEL(sessionConfigurationLabel), configName);


    g_signal_connect(editSessionButton, "clicked", G_CALLBACK(on_edit_session_button_clicked), getsession);
    g_signal_connect(deleteSessionButton, "clicked", G_CALLBACK(on_delete_session_button_clicked), getsession);
    g_signal_connect(sessionWindow, "delete-event", G_CALLBACK(cleanup), getsession);

    gtk_window_set_icon(GTK_WINDOW(sessionWindow), icon);
    gtk_widget_show(sessionWindow);
}

void on_delete_session_button_clicked(GtkButton *button, gpointer user_data) {
    GetSession *getsession = (GetSession *)user_data;
    GtkBuilder *builder = getsession->builder;
    gint session_id = getsession->id_session;
    delete_session(session_id);
    refresh_sessions_view(current_profile_id);

    GtkWidget *sessionWindow = GTK_WIDGET(gtk_builder_get_object(builder, "sessionWindow"));
    gtk_widget_destroy(sessionWindow);

    free(getsession);
}

void on_edit_session_button_clicked(GtkButton *button, gpointer user_data) {
    GetSession *getsession = (GetSession *)user_data;
    GtkBuilder *builder = getsession->builder;
    gint session_id = getsession->id_session;
    Session session = get_session(session_id);
    GtkWidget *sessionWindow = GTK_WIDGET(gtk_builder_get_object(builder, "sessionWindow"));
    GtkEntry *sessionNameInput = GTK_ENTRY(gtk_builder_get_object(builder, "sessionName"));

    const gchar *new_session_name = gtk_entry_get_text(sessionNameInput);


   if (strlen(new_session_name) == 0) {
        GtkWidget *errorLabel = GTK_WIDGET(gtk_builder_get_object(getsession->builder, "errorLabel"));
        gtk_label_set_text(GTK_LABEL(errorLabel), "You must give a name to your session");
        gtk_widget_show(errorLabel);
    }else{
       g_strlcpy(session.name, new_session_name, sizeof(session.name));
       update_session(session);
       refresh_sessions_view(current_profile_id);
       gtk_widget_destroy(sessionWindow);
       free(getsession);
   }

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

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    Session *sessions = get_sessions_by_profile_id(profile_id);

    int row = 1;

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

    GList *children, *iter;
    children = gtk_container_get_children(GTK_CONTAINER(viewportconfiguration));
    for (iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    Configuration *configurations = get_configurations(profile_id);

    int row = 1;

    for (int i = 0; configurations[i].id != -1; ++i) {
        GtkWidget *button = gtk_button_new_with_label(configurations[i].name);

        gtk_widget_set_size_request(button, 360, 120);
        g_object_set_data(G_OBJECT(button), "config_id", GINT_TO_POINTER(configurations[i].id));

        g_signal_connect(button, "clicked", G_CALLBACK(on_configuration_button_clicked), NULL);
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

        g_object_set_data(G_OBJECT(button), "config_id", GINT_TO_POINTER(configurations[i].id));
        gtk_style_context_add_class(gtk_widget_get_style_context(button), "sessionFrame");
        g_signal_connect(button, "clicked", G_CALLBACK(on_configuration_button_clicked), NULL);

        gtk_grid_attach(GTK_GRID(grid), button, 0, row, 1, 1);

        row++;
    }

    gtk_container_add(GTK_CONTAINER(viewportconfiguration), grid);

    gtk_widget_show_all(viewportconfiguration);
    free(configurations);
}

void on_edit_configuration_button_clicked(GtkButton *button, gpointer user_data) {
    GetSession *getConfig = (GetSession *) user_data;
    GtkBuilder *builder = getConfig->builder;
    boolean conditions = TRUE;
    GtkWidget *configWindow = GTK_WIDGET(gtk_builder_get_object(builder, "configWindow"));
    GtkWidget *errorLabel = GTK_WIDGET(gtk_builder_get_object(builder, "errorLabel"));

    GtkWidget *getNameConfig = GTK_WIDGET(gtk_builder_get_object(builder, "getNameConfig"));
    GtkWidget *getMoveForwardConfig = GTK_WIDGET(gtk_builder_get_object(builder, "getMoveForwardConfig"));
    GtkWidget *getMoveBackwardConfig = GTK_WIDGET(gtk_builder_get_object(builder, "getMoveBackwardConfig"));
    GtkWidget *getMoveleftConfig = GTK_WIDGET(gtk_builder_get_object(builder, "getMoveleftConfig"));
    GtkWidget *getMoverightConfig = GTK_WIDGET(gtk_builder_get_object(builder, "getMoverightConfig"));
    GtkWidget *getChangeStepConfig = GTK_WIDGET(gtk_builder_get_object(builder, "getChangeStepConfig"));
    GtkWidget *getSpeedStep = GTK_WIDGET(gtk_builder_get_object(builder, "getSpeedStep"));

    const gchar *name = gtk_entry_get_text(GTK_ENTRY(getNameConfig));
    const gchar *moveForward = gtk_entry_get_text(GTK_ENTRY(getMoveForwardConfig));
    const gchar *moveBackward = gtk_entry_get_text(GTK_ENTRY(getMoveBackwardConfig));
    const gchar *moveLeft = gtk_entry_get_text(GTK_ENTRY(getMoveleftConfig));
    const gchar *moveRight = gtk_entry_get_text(GTK_ENTRY(getMoverightConfig));
    const gchar *changeStep = gtk_entry_get_text(GTK_ENTRY(getChangeStepConfig));
    const gchar *speedStep = gtk_entry_get_text(GTK_ENTRY(getSpeedStep));

    if (strlen(name) == 0 || strlen(moveForward) == 0 || strlen(moveBackward) == 0 ||
    strlen(moveLeft) == 0 || strlen(moveRight) == 0 || strlen(changeStep) == 0 ||
    strlen(speedStep) == 0 || atoi(speedStep) < 0 || atoi(speedStep) > 100)
        conditions = FALSE;


    if(conditions){
        Configuration editedConfiguration;

        strncpy(editedConfiguration.name, name, sizeof(editedConfiguration.name) - 1);
        editedConfiguration.name[sizeof(editedConfiguration.name) - 1] = '\0';

        if (strlen(moveForward) > 0)
            editedConfiguration.move_forward = moveForward[0];
        else
            editedConfiguration.move_forward = 'z';

        if (strlen(moveBackward) > 0)
            editedConfiguration.move_backward = moveBackward[0];
        else
            editedConfiguration.move_backward = 's';

        if (strlen(moveLeft) > 0)
            editedConfiguration.move_left = moveLeft[0];
        else
            editedConfiguration.move_left = 'q';

        if (strlen(moveRight) > 0)
            editedConfiguration.move_right = moveRight[0];
        else
            editedConfiguration.move_right = 'd';

        if (strlen(changeStep) > 0)
            editedConfiguration.change_step_button = changeStep[0];
        else
            editedConfiguration.change_step_button = '&';

        editedConfiguration.id = getConfig->id_session;
        editedConfiguration.speed_step = atoi(speedStep);

        update_configuration(editedConfiguration);
        refresh_configurations_view(current_profile_id);
        updateChosenConfiguration();
        gtk_widget_destroy(configWindow);
        free(getConfig);
    }else{
        gtk_label_set_text(GTK_LABEL(errorLabel), "The fields are not filled in correctly");
    }
}

void on_delete_configuration_button_clicked(GtkButton *button, gpointer user_data) {
    GetSession *getConfig = (GetSession *)user_data;
    GtkBuilder *builder = getConfig->builder;

    delete_configuration(getConfig->id_session);
    refresh_configurations_view(current_profile_id);

    GtkWidget *configWindow = GTK_WIDGET(gtk_builder_get_object(builder, "configWindow"));
    gtk_widget_destroy(configWindow);
    chosen_config_id = -1;
    updateChosenConfiguration();
    free(getConfig);
}

void on_configuration_button_clicked(GtkButton *button, gpointer user_data) {

    GetSession *getConfig = (GetSession *)malloc(sizeof(GetSession));
    gint config_id = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(button), "config_id"));

    GError *error = NULL;
    GdkPixbuf *icon = gdk_pixbuf_new_from_file("files/car.png", &error);


    getConfig->builder = gtk_builder_new_from_file("files/configuration.glade");
    getConfig->id_session = config_id;

    GtkWidget *configWindow = GTK_WIDGET(gtk_builder_get_object(getConfig->builder, "configWindow"));
    Configuration configuration = get_configuration(config_id);

    GtkWidget *getNameConfig = GTK_WIDGET(gtk_builder_get_object(getConfig->builder, "getNameConfig"));
    GtkWidget *getMoveForwardConfig = GTK_WIDGET(gtk_builder_get_object(getConfig->builder, "getMoveForwardConfig"));
    GtkWidget *getMoveBackwardConfig = GTK_WIDGET(gtk_builder_get_object(getConfig->builder, "getMoveBackwardConfig"));
    GtkWidget *getMoveleftConfig = GTK_WIDGET(gtk_builder_get_object(getConfig->builder, "getMoveleftConfig"));
    GtkWidget *getMoverightConfig = GTK_WIDGET(gtk_builder_get_object(getConfig->builder, "getMoverightConfig"));
    GtkWidget *getChangeStepConfig = GTK_WIDGET(gtk_builder_get_object(getConfig->builder, "getChangeStepConfig"));
    GtkWidget *speedStep = GTK_WIDGET(gtk_builder_get_object(getConfig->builder, "getSpeedStep"));
    GtkWidget *deleteConfigButton = GTK_WIDGET(gtk_builder_get_object(getConfig->builder, "deleteConfig"));
    GtkWidget *editConfigButton = GTK_WIDGET(gtk_builder_get_object(getConfig->builder, "editConfig1"));
    GtkWidget *chosenConfigurationButton = GTK_WIDGET(gtk_builder_get_object(getConfig->builder, "chosenConfigurationButton"));

    gtk_entry_set_text(GTK_ENTRY(getNameConfig), configuration.name);
    gtk_entry_set_text(GTK_ENTRY(getMoveForwardConfig), g_strdup_printf("%c", configuration.move_forward));
    gtk_entry_set_text(GTK_ENTRY(getMoveBackwardConfig), g_strdup_printf("%c", configuration.move_backward));
    gtk_entry_set_text(GTK_ENTRY(getMoveleftConfig), g_strdup_printf("%c", configuration.move_left));
    gtk_entry_set_text(GTK_ENTRY(getMoverightConfig), g_strdup_printf("%c", configuration.move_right));
    gtk_entry_set_text(GTK_ENTRY(getChangeStepConfig), g_strdup_printf("%c", configuration.change_step_button));
    gtk_entry_set_text(GTK_ENTRY(speedStep), g_strdup_printf("%d", configuration.speed_step));


    g_signal_connect(deleteConfigButton, "clicked", G_CALLBACK(on_delete_configuration_button_clicked), getConfig);
    g_signal_connect(editConfigButton, "clicked", G_CALLBACK(on_edit_configuration_button_clicked), getConfig);
    g_signal_connect(chosenConfigurationButton, "clicked", G_CALLBACK(on_chosen_configuration_button_clicked), getConfig);
    g_signal_connect(configWindow, "delete-event", G_CALLBACK(cleanup), getConfig);

    gtk_window_set_icon(GTK_WINDOW(configWindow), icon);
    gtk_widget_show(configWindow);
}

void on_chosen_configuration_button_clicked(GtkButton *button, gpointer user_data){
    GetSession *getConfig = (GetSession *)user_data;
    GtkBuilder *builder = getConfig->builder;

    chosen_config_id = getConfig->id_session;
    updateChosenConfiguration();
    GtkWidget *configWindow = GTK_WIDGET(gtk_builder_get_object(builder, "configWindow"));
    gtk_widget_destroy(configWindow);

    free(getConfig);
}

void on_create_config_button_clicked(GtkButton *button, gpointer user_data) {
    Configuration config;

    strcpy(config.name, settings->configuration.name);
    config.move_forward = settings->configuration.move_forward;
    config.move_backward = settings->configuration.move_backward;
    config.move_left = settings->configuration.move_left;
    config.move_right = settings->configuration.move_right;
    config.speed_step = settings->configuration.speed_step;
    config.change_step_button = settings->configuration.change_step_button;
    config.id_profile = current_profile_id;

    create_configuration(&config);
    refresh_configurations_view(current_profile_id);
}

void destroyWindow(GtkWidget *widget, gpointer user_data){
    g_object_unref(globalBuilder);
}

void update_current_profile_label() {
    GtkWidget *currentProfileLabel = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "currentProfile"));
    Profile current_profile = get_profile(current_profile_id);

    const char *profile_name = current_profile.username;
    gtk_label_set_text(GTK_LABEL(currentProfileLabel), profile_name);
}

void updateChosenConfiguration(){

    GtkWidget *currentConfigLabel = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "chosenConfigurationLabel"));
    if(chosen_config_id == -1){
        gtk_label_set_text(GTK_LABEL(currentConfigLabel), "");
        return;
    }
    Configuration configuration = get_configuration(chosen_config_id);

    const char *configuration_name = configuration.name;
    gtk_label_set_text(GTK_LABEL(currentConfigLabel), configuration_name);
}

void update_speed_label(gdouble instantSpeed) {
    GtkWidget *speedLabel = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "instantSpeed"));
    gtk_label_set_text(GTK_LABEL(speedLabel), g_strdup_printf("%.2f cm/s", instantSpeed));
    GtkWidget *speedBar = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "speedProgress"));
    double fraction = instantSpeed / 50.0;
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(speedBar), fraction);
}

void update_distance_label(int distance){
    GtkWidget *distanceLabel = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "distance"));
    gtk_label_set_text(GTK_LABEL(distanceLabel), g_strdup_printf("%d cm", distance));
}

void update_duration_label(int duration){

    int hours = duration / 3600;
    int minutes = (duration % 3600) / 60;
    int secondes = duration % 60;
    GtkWidget *durationLabel = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "timerSession"));
    gtk_label_set_text(GTK_LABEL(durationLabel), g_strdup_printf("%dh %dmin %ds", hours, minutes, secondes));
}


void* control_thread_function(void* data) {
    ControlData* control_data = (ControlData*)data;
    Configuration* configuration = &(control_data->configuration);
    control(configuration, control_data->id_profile, control_data->clientSocket);
    return NULL;
}

void on_stop_session_clicked(GtkButton *button, gpointer user_data){
    GtkButton *stopSession = GTK_BUTTON(gtk_builder_get_object(globalBuilder, "stopSession"));
    GtkButton *startSession = GTK_BUTTON(gtk_builder_get_object(globalBuilder,"startsession"));
    gtk_widget_set_sensitive(GTK_WIDGET(stopSession), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(startSession), TRUE);

    pthread_mutex_lock(&end_mutex);
    end = 0;
    pthread_mutex_unlock(&end_mutex);
    closeConnexion(clientSocket);
}

void on_ok_button_clicked(GtkButton *button, gpointer user_data){
    GtkBuilder *builder = (GtkBuilder *)user_data;
    GtkWidget *errorWindow = GTK_WIDGET(gtk_builder_get_object(builder, "popUpError"));
    gtk_widget_hide(errorWindow);
    g_object_unref(builder);
}

void errorPopUp(char *errorContent){

    GError *error = NULL;
    GdkPixbuf *icon = gdk_pixbuf_new_from_file("files/car.png", &error);
    GtkBuilder *builder = gtk_builder_new_from_file("files/popUpError.glade");
    GtkWidget *errorWindow = GTK_WIDGET(gtk_builder_get_object(builder, "popUpError"));
    GtkWidget *errorLabel = GTK_WIDGET(gtk_builder_get_object(builder, "errorContent"));
    GtkButton *errorButton = GTK_BUTTON(gtk_builder_get_object(builder, "errorValidation"));

    gtk_label_set_text(GTK_LABEL(errorLabel), errorContent);
    g_signal_connect(errorButton, "clicked", G_CALLBACK(on_ok_button_clicked), builder);

    gtk_window_set_icon(GTK_WINDOW(errorWindow), icon);
    gtk_widget_show_all(errorWindow);
}
void on_start_session_clicked(GtkButton *button, gpointer user_data){

    if(chosen_config_id == -1){
        errorPopUp("You must choose a configuration to start a session !");
        return;
    }
    clientSocket = initConnexion();
    if(clientSocket == 1){
        errorPopUp("Connexion issues with the car !");
        error_content(200);
        return;
    }

    GtkButton *stopSession = GTK_BUTTON(gtk_builder_get_object(globalBuilder, "stopSession"));
    GtkButton *startSession = GTK_BUTTON(gtk_builder_get_object(globalBuilder,"startsession"));
    gtk_widget_set_sensitive(GTK_WIDGET(stopSession), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(startSession), FALSE);
    g_signal_connect(stopSession, "clicked", G_CALLBACK(on_stop_session_clicked), NULL);

    Configuration configuration = get_configuration(chosen_config_id);

    ControlData *control_data = g_new(ControlData, 1);
    control_data->configuration = configuration;
    control_data->id_profile = current_profile_id;
    control_data->clientSocket = clientSocket;

    pthread_mutex_lock(&end_mutex);
    end = 1;
    pthread_mutex_unlock(&end_mutex);

    pthread_t control_thread;
    pthread_create(&control_thread, NULL, control_thread_function, control_data);
    pthread_detach(control_thread);
}

gboolean update_labels_callback(gpointer user_data) {
    struct UpdateLabelsData *data = (struct UpdateLabelsData *)user_data;

    update_speed_label(data->instantSpeed);
    update_duration_label(data->duration);
    update_distance_label(data->distanceCovered);

    g_free(data);

    return FALSE;
}

gboolean on_stop_session_callback(gpointer user_data){
    on_stop_session_clicked(NULL, NULL);
    return FALSE;
}


void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *profile;
    GtkWidget *deleteProfile;
    GtkWidget *exportation;
    GtkWidget *getCode;
    GtkButton *startSession;
    GtkButton *stopSession;

    if(does_profile_exist_with_id(settings->defaultUserId)) {
        current_profile_id = settings->defaultUserId;
    }else{
        current_profile_id = 1;
    }


    globalBuilder = gtk_builder_new_from_file("files/index.glade");

    window = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "window"));

    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "styles/style.css", NULL);

    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                              GTK_STYLE_PROVIDER(cssProvider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);

    startSession = GTK_BUTTON(gtk_builder_get_object(globalBuilder,"startsession"));
    stopSession = GTK_BUTTON(gtk_builder_get_object(globalBuilder, "stopSession"));
    gtk_widget_set_sensitive(GTK_WIDGET(stopSession), FALSE);

    GError *error = NULL;
    GdkPixbuf *icon = gdk_pixbuf_new_from_file("files/car.png", &error);
    gtk_window_set_icon(GTK_WINDOW(window), icon);

    if(end == 1){
        g_signal_connect(window, "destroy", G_CALLBACK(on_stop_session_clicked), NULL);
    }
    g_signal_connect(window, "destroy", G_CALLBACK(destroyWindow), user_data);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), user_data);
    g_signal_connect(window, "button-press-event", G_CALLBACK(on_window_button_press_event), NULL);

    profile = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "profile"));
    deleteProfile = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "deleteProfiles"));
    exportation = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "exportation"));
    getCode = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "curlCode"));

    arraySessions(current_profile_id);
    arrayConfigurations(current_profile_id);
    update_current_profile_label();

    g_signal_connect(deleteProfile, "button_press_event", G_CALLBACK(on_delete_profile_activate), window);
    g_signal_connect(exportation, "button_press_event", G_CALLBACK(on_exportations_activate), window);
    g_signal_connect(profile, "button_press_event", G_CALLBACK(on_profile_activate), window);
    g_signal_connect(getCode, "button_press_event", G_CALLBACK(on_curl_activate), window);
    g_signal_connect(GTK_BUTTON(gtk_builder_get_object(globalBuilder, "addConfigButton")), "clicked", G_CALLBACK(on_create_config_button_clicked), NULL);
    g_signal_connect(startSession, "clicked", G_CALLBACK(on_start_session_clicked), NULL);
    gtk_widget_show_all(window);
    gtk_main();
}
