#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

GtkWidget *username_entry;
GtkWidget *password_entry;

// Function to handle login button click
void on_login_button_clicked(GtkWidget *widget, gpointer data) {
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(password_entry));

    // Add authentication logic here (e.g., check against a predefined username and password)

    // For now, let's print the entered credentials
    g_print("Username: %s\nPassword: %s\n", username, password);
}

// Function to handle create account button click
void on_create_account_button_clicked(GtkWidget *widget, gpointer data) {
    // Add logic to open the "Create Account" window or perform other actions
    g_print("Opening Create Account window...\n");
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *main_box;
    GtkWidget *welcome_label;
    GtkWidget *username_label;
    GtkWidget *password_label;
    GtkWidget *login_button;
    GtkWidget *create_account_button;  // Replace create_account_link with a button

    // Initializing GTK
    gtk_init(&argc, &argv);

    // Creating a window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Planifique - Login");
    gtk_window_set_default_size(GTK_WINDOW(window),600, 400);

    // Vertical box to hold widgets in the main window
    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    // Welcome label
    welcome_label = gtk_label_new("Bienvenue sur Planifique:");
    gtk_box_pack_start(GTK_BOX(main_box), welcome_label, FALSE, FALSE, 20);

    // Username field
    username_label = gtk_label_new("Nom du compte:");
    gtk_box_pack_start(GTK_BOX(main_box), username_label, FALSE, FALSE, 10);
    username_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(main_box), username_entry, FALSE, FALSE, 5);

    // Password field
    password_label = gtk_label_new("Mot de passe:");
    gtk_box_pack_start(GTK_BOX(main_box), password_label, FALSE, FALSE, 5);
    password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE); // Hide password
    gtk_box_pack_start(GTK_BOX(main_box), password_entry, FALSE, FALSE, 5);

    // Login button
    login_button = gtk_button_new_with_label("Login");
    gtk_box_pack_start(GTK_BOX(main_box), login_button, FALSE, FALSE, 10);

    // Create account button
    create_account_button = gtk_button_new_with_label("Create an account");
    gtk_box_pack_start(GTK_BOX(main_box), create_account_button, FALSE, FALSE, 5);

    // Connect "clicked" signals to respective functions
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_button_clicked), NULL);
    g_signal_connect(create_account_button, "clicked", G_CALLBACK(on_create_account_button_clicked), NULL);

    // Show all widgets
    gtk_widget_show_all(window);

    // Enter the main loop
    gtk_main();

    return 0;
}
