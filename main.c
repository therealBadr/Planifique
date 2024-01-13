#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

// Function prototypes
void on_login_button_clicked(GtkWidget *widget, gpointer data);
void on_create_account_button_clicked(GtkWidget *widget, gpointer data);
void on_create_button_clicked(GtkWidget *widget, gpointer data);

// Widgets for the main window
GtkWidget *username_entry;
GtkWidget *password_entry;

// Widgets for the account creation window
GtkWidget *create_account_window;
GtkWidget *create_username_entry;
GtkWidget *create_password_entry;
GtkWidget *create_button;

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
    // Create a new window for account creation
    create_account_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(create_account_window), "Create Account");
    gtk_window_set_default_size(GTK_WINDOW(create_account_window), 400, 200);

    // Vertical box to hold widgets in the account creation window
    GtkWidget *create_account_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_container_add(GTK_CONTAINER(create_account_window), create_account_box);

    // Username field in account creation window
    GtkWidget *create_username_label = gtk_label_new("Enter Username:");
    gtk_box_pack_start(GTK_BOX(create_account_box), create_username_label, FALSE, FALSE, 10);
    create_username_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(create_account_box), create_username_entry, FALSE, FALSE, 5);

    // Password field in account creation window
    GtkWidget *create_password_label = gtk_label_new("Enter Password:");
    gtk_box_pack_start(GTK_BOX(create_account_box), create_password_label, FALSE, FALSE, 5);
    create_password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(create_password_entry), FALSE); // Hide password
    gtk_box_pack_start(GTK_BOX(create_account_box), create_password_entry, FALSE, FALSE, 5);

    // Create button in account creation window
    create_button = gtk_button_new_with_label("Create Account");
    gtk_box_pack_start(GTK_BOX(create_account_box), create_button, FALSE, FALSE, 10);

    // Connect "clicked" signal to create account function
    g_signal_connect(create_button, "clicked", G_CALLBACK(on_create_button_clicked), NULL);

    // Show all widgets in the account creation window
    gtk_widget_show_all(create_account_window);
}

// Function to handle create button click in account creation window
void on_create_button_clicked(GtkWidget *widget, gpointer data) {
    const gchar *create_username = gtk_entry_get_text(GTK_ENTRY(create_username_entry));
    const gchar *create_password = gtk_entry_get_text(GTK_ENTRY(create_password_entry));

    // Check if a file with the same username already exists
    char filename[50];
    sprintf(filename, "%s.txt", create_username);

    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        // Close the file if it exists
        fclose(file);

        // Display a pop-up for an existing account
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(create_account_window),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "An account with the same username already exists.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    } else {
        // Create a new file with username and password as its name
        file = fopen(filename, "w");
        if (file != NULL) {
            fprintf(file, "Username: %s\nPassword: %s\n", create_username, create_password);
            fclose(file);

            // Display a pop-up for a successfully created account
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(create_account_window),
                                                       GTK_DIALOG_MODAL,
                                                       GTK_MESSAGE_INFO,
                                                       GTK_BUTTONS_OK,
                                                       "Account created successfully!");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);

            // Close the account creation window
            gtk_widget_destroy(create_account_window);
        } else {
            // Display a pop-up for an error in creating the account file
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(create_account_window),
                                                       GTK_DIALOG_MODAL,
                                                       GTK_MESSAGE_ERROR,
                                                       GTK_BUTTONS_OK,
                                                       "Error creating the account file.");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
        }
    }
}


int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *main_box;
    GtkWidget *welcome_label;
    GtkWidget *username_label;
    GtkWidget *password_label;
    GtkWidget *login_button;
    GtkWidget *create_account_button;

    // Initializing GTK
    gtk_init(&argc, &argv);

    // Creating a window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Planifique - Login");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);

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
    gtk_box_pack_start(GTK_BOX(main_box), create_account_button, FALSE, FALSE, 10);

    // Connect "clicked" signals to respective functions
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_button_clicked), NULL);
    g_signal_connect(create_account_button, "clicked", G_CALLBACK(on_create_account_button_clicked), NULL);

    // Show all widgets
    gtk_widget_show_all(window);

    // Enter the main loop
    gtk_main();

    return 0;
}
