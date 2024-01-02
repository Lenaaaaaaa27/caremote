#include "../../includes/define.h"

static GtkBuilder *globalBuilder = NULL;
static int current_profile_id = 1;


static GtkWidget *globalProfileSubmenu = NULL;
static gboolean is_profile_submenu_created = FALSE;

static GtkWidget *addProfileWindow = NULL;
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

void on_profile_menu_item_activate(GtkMenuItem *menu_item, gpointer user_data) {
    int user_id = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(menu_item), "user_id"));
    current_profile_id = user_id;

    globalBuilder = gtk_builder_new_from_file("index.glade");

    // Appelez les fonctions avec l'ID du profil
    arraySessions(user_id);
    arrayConfigurations(user_id);
}

void on_profile_activate(GtkWidget *widget, gpointer user_data) {
    close_submenus();
    Profile *profiles = get_profiles();

    if (!is_profile_submenu_created) {
        globalProfileSubmenu = gtk_menu_new();

        for (int i = 0; profiles[i].id != -1; ++i) {
            GtkWidget *profile_item = gtk_menu_item_new_with_label(profiles[i].username);
            g_object_set_data(G_OBJECT(profile_item), "user_id", GINT_TO_POINTER(profiles[i].id));
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
        g_signal_connect(entry, "activate", G_CALLBACK(on_activate_entry), NULL);
        g_signal_connect(addProfileWindow, "delete-event", G_CALLBACK(on_delete_event), NULL);
    }

    gtk_widget_show_all(addProfileWindow);
}


void on_activate_entry() {
    enterPressed = TRUE;
    on_create_profile_clicked(NULL, NULL);
}

gboolean on_delete_event() {
    if (enterPressed) {
        enterPressed = FALSE;
        return FALSE;
    } else {
        gtk_widget_hide(addProfileWindow);
        entry = NULL;
        addProfileWindow = NULL;
        return TRUE;
    }
}

void on_create_profile_clicked(GtkWidget *widget, gpointer user_data) {

    const gchar *username = gtk_entry_get_text(entry);
    char *username_copy = g_strdup(username);

    create_profile(username_copy);
    g_free(username_copy);

    gtk_widget_hide(addProfileWindow);
    entry = NULL;
    addProfileWindow = NULL;
    enterPressed = FALSE;
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
        // Créer un bouton avec le nom de la session
        button = gtk_button_new_with_label(sessions[i].name);

        // Définir la largeur et la hauteur du bouton
        gtk_widget_set_size_request(button, 360, 120);

        // Ajouter le bouton à la grille
        gtk_grid_attach(GTK_GRID(grid), button, 0, row, 1, 1);

        // Incrémenter le compteur de lignes
        row++;
    }

    // Placer la grille dans le GtkScrolledWindow
    gtk_container_add(GTK_CONTAINER(viewportsession), grid);

    gtk_widget_show_all(viewportsession);  // Afficher tous les widgets
    free(sessions);
}

void arrayConfigurations(int profile_id) {
    GtkWidget *viewportconfiguration;
    GtkWidget *button;

    viewportconfiguration = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "viewportconfiguration"));

    // Créer un GtkGrid pour contenir le label et les boutons de session
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    Configuration *configurations = get_configurations(profile_id);

    int row = 1;  // Commencer à la deuxième ligne pour les boutons

    for (int i = 0; configurations[i].id != -1; ++i) {
        // Créer un bouton avec le nom de la session
        button = gtk_button_new_with_label(configurations[i].name);

        // Définir la largeur et la hauteur du bouton
        gtk_widget_set_size_request(button, 360, 120);

        // Ajouter le bouton à la grille
        gtk_grid_attach(GTK_GRID(grid), button, 0, row, 1, 1);

        // Incrémenter le compteur de lignes
        row++;
    }

    // Placer la grille dans le GtkScrolledWindow
    gtk_container_add(GTK_CONTAINER(viewportconfiguration), grid);

    gtk_widget_show_all(viewportconfiguration);  // Afficher tous les widgets
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

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), user_data);
    g_signal_connect(window, "button-press-event", G_CALLBACK(on_window_button_press_event), NULL);

    gtk_builder_connect_signals(globalBuilder, user_data);

    profile = GTK_WIDGET(gtk_builder_get_object(globalBuilder, "profile"));

    arraySessions(current_profile_id);
    arrayConfigurations(current_profile_id);

    g_signal_connect(profile, "button_press_event", G_CALLBACK(on_profile_activate), window);

    gtk_widget_show_all(window);
    gtk_main();

}
