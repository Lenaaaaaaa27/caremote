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

// Gestionnaire d'événements pour fermer les sous-menus lorsqu'un clic est effectué ailleurs
static gboolean on_window_button_press_event() {
    close_submenus();
    return FALSE;
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

        // Ajouter un séparateur
        GtkWidget *separator = gtk_separator_menu_item_new();
        gtk_menu_shell_append(GTK_MENU_SHELL(globalProfileSubmenu), separator);
        gtk_widget_show(separator);

        // Ajouter "Add a Profile"
        GtkWidget *add_profile_item = gtk_menu_item_new_with_label("Add a profile");
        gtk_menu_shell_append(GTK_MENU_SHELL(globalProfileSubmenu), add_profile_item);
        g_signal_connect(add_profile_item, "activate", G_CALLBACK(on_add_profile_activate), user_data);
        gtk_widget_show(add_profile_item);

        is_profile_submenu_created = TRUE;
    }

    free(profiles);

    gtk_menu_popup_at_pointer(GTK_MENU(globalProfileSubmenu), NULL);
}

void on_add_profile_activate() {
    GtkBuilder *builder = gtk_builder_new_from_file("index.glade");
    GtkWidget *addProfileWindow = GTK_WIDGET(gtk_builder_get_object(builder, "addProfile"));

    if (addProfileWindow != NULL) {
        gtk_widget_show_all(addProfileWindow);

        // Récupérer l'entrée de l'utilisateur
        GtkEntry *entry = GTK_ENTRY(gtk_builder_get_object(builder, "Addprofileinput"));

        // Vérifier si l'objet entry est non nul
        if (entry != NULL) {
            // Connecter le signal "activate" du champ d'entrée à la fonction de création de profil
            g_signal_connect(entry, "activate", G_CALLBACK(on_create_profile_clicked), NULL);
        } else {
            g_warning("Objet entry est NULL.");
        }
    } else {
        g_warning("Impossible de charger la fenetre addProfile depuis le fichier Glade.");
    }

    g_object_unref(builder);
}

void on_create_profile_clicked(GtkWidget *widget, gpointer user_data) {

    GtkEntry *entry = GTK_ENTRY(widget);
    const gchar *username = gtk_entry_get_text(entry);

    char *username_copy = g_strdup(username);

    create_profile(username_copy);

    g_free(username_copy);

    GtkWidget *addProfileWindow = gtk_widget_get_toplevel(GTK_WIDGET(widget));
    if (GTK_IS_WIDGET(addProfileWindow)) {
        gtk_widget_destroy(addProfileWindow);
    }
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

void arraySession(GtkBuilder *builder) {
    GtkWidget *viewportsession;
    GtkWidget *label;
    GtkWidget *button;

    viewportsession = GTK_WIDGET(gtk_builder_get_object(builder, "viewportsession"));

    // Créer un GtkGrid pour contenir le label et les boutons de session
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    Session *sessions = get_sessions_by_profile_id(1);

    int row = 1;  // Commencer à la deuxième ligne pour les boutons

    for (int i = 0; sessions[i].id != -1; ++i) {  // Utilisez votre propre logique pour les sessions
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
}

void arrayConfigurations(GtkBuilder *builder) {
    GtkWidget *viewportconfiguration;
    GtkWidget *button;

    viewportconfiguration = GTK_WIDGET(gtk_builder_get_object(builder, "viewportconfiguration"));

    // Créer un GtkGrid pour contenir le label et les boutons de session
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    Configuration *configurations = get_configurations(1);


    int row = 1;  // Commencer à la deuxième ligne pour les boutons

    for (int i = 0; configurations[i].id != -1; ++i) {  // Utilisez votre propre logique pour les configurations
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
}

void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *profile;
    GtkWidget *session;
    GtkWidget *configuration;
    GtkBuilder *builder;

    builder = gtk_builder_new_from_file("index.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

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

    gtk_builder_connect_signals(builder, user_data);

    profile = GTK_WIDGET(gtk_builder_get_object(builder, "profile"));
    session = GTK_WIDGET(gtk_builder_get_object(builder, "sessions"));
    configuration = GTK_WIDGET(gtk_builder_get_object(builder, "configuration"));
    arraySession(builder);
    arrayConfigurations(builder);

    g_signal_connect(profile, "button_press_event", G_CALLBACK(on_profile_activate), window);
    g_signal_connect(session, "button_press_event", G_CALLBACK(on_session_activate), window);
    g_signal_connect(configuration, "button_press_event", G_CALLBACK(on_configuration_activate), window);

    g_object_unref(builder);

    gtk_widget_show_all(window);
    gtk_main();
}
