#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

// Function prototypes
void on_login_button_clicked(GtkWidget *widget, gpointer data);
void on_create_account_button_clicked(GtkWidget *widget, gpointer data);
void on_create_button_clicked(GtkWidget *widget, gpointer data);
void open_main_window(const gchar *username);
void on_search_entry_changed(GtkWidget *widget, gpointer data);
void on_add_task_button_clicked(GtkWidget *widget, gpointer data);
void on_edit_task_button_clicked(GtkWidget *widget, gpointer data);
void on_delete_task_button_clicked(GtkWidget *widget, gpointer data);
void on_quit_program_button_clicked(GtkWidget *widget, gpointer data);
void LoadAndDisplayTasks(const gchar *username);

// Widgets for the main window
GtkWidget *username_entry;
GtkWidget *password_entry;
// Replace the GtkTextView with a ListBox for tasks
GtkWidget *task_listbox;
GtkWidget *task_label;

// Widgets for the account creation window
GtkWidget *create_account_window;
GtkWidget *create_username_entry;
GtkWidget *create_password_entry;
GtkWidget *create_button;

// Main application window
GtkWidget *main_window;
GtkWidget *login_window; // To keep track of the login window

void LoadAndDisplayTasks(const gchar *username) {
    // Open the tasks file associated with the user
    char filename[50];
    sprintf(filename, "%s_tasks.txt", username);
    FILE *tasks_file = fopen(filename, "r"); 

    if (tasks_file != NULL) {
        // Clear existing tasks in the ListBox
        gtk_container_foreach(GTK_CONTAINER(task_listbox), (GtkCallback)gtk_widget_destroy, NULL);

        char line[256];
        while (fgets(line, sizeof(line), tasks_file) != NULL) {
            g_print("Tâche de lecture: %s", line);
            // Create a GtkLabel for each task
            GtkWidget *task_label = gtk_label_new(NULL);
            gtk_label_set_text(GTK_LABEL(task_label), line);

            // Create a ListBoxRow and add the label to it
            GtkWidget *task_row = gtk_list_box_row_new();
            gtk_container_add(GTK_CONTAINER(task_row), task_label);

            // Add the ListBoxRow to the ListBox
            gtk_list_box_insert(GTK_LIST_BOX(task_listbox), task_row, -1);
            // Print debug messages
    g_print("Ajout d'une tâche à ListBox: %s", line);
        }

        fclose(tasks_file);
    } else {
        g_error("Erreur lors de l'ouverture du fichier de tâches pour l'utilisateur: %s\n", username);
    }
}


// Function to handle login button click
void on_login_button_clicked(GtkWidget *widget, gpointer data) {
    const gchar *entered_username = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const gchar *entered_password = gtk_entry_get_text(GTK_ENTRY(password_entry));

    // Retrieve stored password based on entered username
    char filename[50];
    sprintf(filename, "%s.txt", entered_username);
    FILE *file = fopen(filename, "r");

    if (file != NULL) {
        char stored_password[100]; // Adjust the size as needed
        fscanf(file, "Nom du compte: %*s\nMot de passe: %99[^\n]", stored_password);
        fclose(file);

        // Compare entered password with stored password
        if (strcmp(entered_password, stored_password) == 0) {
            // Successful login
            g_print("Connexion réussie!\n");
            // Open the main window
            open_main_window(entered_username);
        } else {
            // Failed login
            g_print("Identifiant ou mot de passe incorrect.\n");
            // Display an error message to the user
        }
    } else {
        // File not found, user does not exist
        g_print("User does not exist.\n");
        // Display an error message to the user
    }
}

// Function to open a new main window with a task list
void open_main_window(const gchar *username) {
    // Creating a new main window
    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "Planifique - Acceuil");
    gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 600);

    // Check if tasks file exists for the user
    char filename[50];
    sprintf(filename, "%s_tasks.txt", username);
    FILE *tasks_file = fopen(filename, "r");

    if (tasks_file != NULL) {
        // File exists, close the file
        fclose(tasks_file);
        g_print("Le fichier de tâches existe pour l'utilisateur: %s\n", username);

        // Vertical box to hold widgets in the main window
        GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
        gtk_container_add(GTK_CONTAINER(main_window), main_box);

        // Create a ListBox to hold the tasks
        task_listbox = gtk_list_box_new();
        gtk_box_pack_start(GTK_BOX(main_box), task_listbox, TRUE, TRUE, 5);

        // Load and display tasks
        LoadAndDisplayTasks(username);

        // Search bar
        GtkWidget *search_entry = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(main_box), search_entry, FALSE, FALSE, 5);

        // Buttons
        GtkWidget *add_task_button = gtk_button_new_with_label("Ajouter une tâche");
        GtkWidget *edit_task_button = gtk_button_new_with_label("Modifier une tâche");
        GtkWidget *delete_task_button = gtk_button_new_with_label("Supprimer une tâche");
        GtkWidget *quit_program_button = gtk_button_new_with_label("Quitter le programme");

        gtk_box_pack_start(GTK_BOX(main_box), add_task_button, FALSE, FALSE, 5);
        gtk_box_pack_start(GTK_BOX(main_box), edit_task_button, FALSE, FALSE, 5);
        gtk_box_pack_start(GTK_BOX(main_box), delete_task_button, FALSE, FALSE, 5);
        gtk_box_pack_start(GTK_BOX(main_box), quit_program_button, FALSE, FALSE, 5);

        // Connect button click signals to respective functions
        g_signal_connect(add_task_button, "clicked", G_CALLBACK(on_add_task_button_clicked), (gpointer)g_strdup(username));
        g_signal_connect(edit_task_button, "clicked", G_CALLBACK(on_edit_task_button_clicked), (gpointer)g_strdup(username));
        g_signal_connect(delete_task_button, "clicked", G_CALLBACK(on_delete_task_button_clicked), (gpointer)g_strdup(username));
        g_signal_connect(quit_program_button, "clicked", G_CALLBACK(on_quit_program_button_clicked), NULL);

        // Connect "changed" signal to filter tasks function
        g_signal_connect(search_entry, "changed", G_CALLBACK(on_search_entry_changed), NULL);

        // Show all widgets in the main window
        gtk_widget_show_all(main_window);

        // Close the login window
        gtk_widget_destroy(login_window);
    } else {
        // File does not exist, create a new one
        g_print("Le fichier de tâches n'existe pas pour l'utilisateur: %s\n", username);

        // Create a new tasks file for the user
        tasks_file = fopen(filename, "a");

        if (tasks_file != NULL) {
            // You can add an initial message or header to the tasks file if needed
            fprintf(tasks_file, "# Bienvenue, %s\n", username);

            fclose(tasks_file);
            g_print("Nouveau fichier de tâches créé pour l'utilisateur: %s\n", username);

// Vertical box to hold widgets in the main window
        GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
        gtk_container_add(GTK_CONTAINER(main_window), main_box);

        // Create a ListBox to hold the tasks
        task_listbox = gtk_list_box_new();
        gtk_box_pack_start(GTK_BOX(main_box), task_listbox, TRUE, TRUE, 5);

        // Load and display tasks
        LoadAndDisplayTasks(username);

        // Search bar
        GtkWidget *search_entry = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(main_box), search_entry, FALSE, FALSE, 5);

        // Buttons
        GtkWidget *add_task_button = gtk_button_new_with_label("Ajouter une tâche");
        GtkWidget *edit_task_button = gtk_button_new_with_label("Modifier une tâche");
        GtkWidget *delete_task_button = gtk_button_new_with_label("Supprimer une tâche");
        GtkWidget *quit_program_button = gtk_button_new_with_label("Quitter le programme");

        gtk_box_pack_start(GTK_BOX(main_box), add_task_button, FALSE, FALSE, 5);
        gtk_box_pack_start(GTK_BOX(main_box), edit_task_button, FALSE, FALSE, 5);
        gtk_box_pack_start(GTK_BOX(main_box), delete_task_button, FALSE, FALSE, 5);
        gtk_box_pack_start(GTK_BOX(main_box), quit_program_button, FALSE, FALSE, 5);

        // Connect button click signals to respective functions
        g_signal_connect(add_task_button, "clicked", G_CALLBACK(on_add_task_button_clicked), (gpointer)g_strdup(username));
        g_signal_connect(edit_task_button, "clicked", G_CALLBACK(on_edit_task_button_clicked), (gpointer)g_strdup(username));
        g_signal_connect(delete_task_button, "clicked", G_CALLBACK(on_delete_task_button_clicked), (gpointer)g_strdup(username));
        g_signal_connect(quit_program_button, "clicked", G_CALLBACK(on_quit_program_button_clicked), NULL);

        // Connect "changed" signal to filter tasks function
        g_signal_connect(search_entry, "changed", G_CALLBACK(on_search_entry_changed), NULL);

        // Show all widgets in the main window
        gtk_widget_show_all(main_window);

        // Close the login window
        gtk_widget_destroy(login_window);
        } else {
            g_error("Error creating tasks file for user: %s\n", username);
            return;
        }
    }
}


// Function to handle add task button click
void on_add_task_button_clicked(GtkWidget *widget, gpointer data) {
    const gchar *username = (const gchar *)data;

    // Create a new dialog window for task input
    GtkWidget *task_dialog = gtk_dialog_new_with_buttons("Ajouter une tâche",
                                                        GTK_WINDOW(main_window),
                                                        GTK_DIALOG_MODAL,
                                                        "OK",
                                                        GTK_RESPONSE_OK,
                                                        "Annuler",
                                                        GTK_RESPONSE_CANCEL,
                                                        NULL);

    // Create a grid to organize the dialog content
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(task_dialog))), grid);

    GtkWidget *id_label = gtk_label_new("ID de tâche:");
    GtkWidget *name_label = gtk_label_new("Nom de tâche:");
    GtkWidget *description_label = gtk_label_new("Description de tâche:");
    GtkWidget *date_label = gtk_label_new("Date de tâche:");

    GtkWidget *id_entry = gtk_entry_new();
    GtkWidget *name_entry = gtk_entry_new();
    GtkWidget *description_entry = gtk_entry_new();
    GtkWidget *date_entry = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), id_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), id_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), description_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), description_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), date_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), date_entry, 1, 3, 1, 1);

    // Show all widgets in the dialog
    gtk_widget_show_all(task_dialog);

    // Wait for user response
    gint result = gtk_dialog_run(GTK_DIALOG(task_dialog));

    if (result == GTK_RESPONSE_OK) {
        // Retrieve entered task details
        const gchar *id = gtk_entry_get_text(GTK_ENTRY(id_entry));
        const gchar *name = gtk_entry_get_text(GTK_ENTRY(name_entry));
        const gchar *description = gtk_entry_get_text(GTK_ENTRY(description_entry));
        const gchar *date = gtk_entry_get_text(GTK_ENTRY(date_entry));

        // Check if the task ID already exists in the user's tasks file
        char filename[50];
        sprintf(filename, "%s_tasks.txt", username);
        FILE *tasks_file = fopen(filename, "r");

        gboolean idExists = FALSE;
        if (tasks_file != NULL) {
            char line[256];
            while (fgets(line, sizeof(line), tasks_file) != NULL) {
                // Assuming the ID is the first word on each line, adjust accordingly
                char existingID[50];
                sscanf(line, "%s", existingID);

                if (strcmp(existingID, id) == 0) {
                    idExists = TRUE;
                    break;
                }
            }

            fclose(tasks_file);
        }

        // If the task ID exists, show an error message
        if (idExists) {
            GtkWidget *error_dialog = gtk_message_dialog_new(GTK_WINDOW(task_dialog),
                                                             GTK_DIALOG_MODAL,
                                                             GTK_MESSAGE_ERROR,
                                                             GTK_BUTTONS_OK,
                                                             "L'ID de tâche existe déjà. Veuillez choisir un autre identifiant.");
            gtk_dialog_run(GTK_DIALOG(error_dialog));
            gtk_widget_destroy(error_dialog);
        } else {
            // If the task ID does not exist, proceed with adding the task
            tasks_file = fopen(filename, "a");

            if (tasks_file != NULL) {
                fprintf(tasks_file, "%s - %s - %s - %s\n", id, name, description, date);
                fclose(tasks_file);
                LoadAndDisplayTasks(username);
                g_print("Tâche ajoutée avec succès!\n");

                
            } else {
                g_error("Erreur lors de l'ouverture du fichier de tâches pour l'utilisateur: %s\n", username);
            }
        }
    }

    // Close the dialog
    gtk_widget_destroy(task_dialog);
}

// Function to handle edit task button click
void on_edit_task_button_clicked(GtkWidget *widget, gpointer data) {
    const gchar *username = (const gchar *)data;

    // Create a new dialog window for task modification
    GtkWidget *edit_task_dialog = gtk_dialog_new_with_buttons("Editer tâche",
                                                             GTK_WINDOW(main_window),
                                                             GTK_DIALOG_MODAL,
                                                             "OK",
                                                             GTK_RESPONSE_OK,
                                                             "Annuler",
                                                             GTK_RESPONSE_CANCEL,
                                                             NULL);

    // Create a grid to organize the dialog content
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(edit_task_dialog))), grid);

    // Labels and entry fields for task details
    GtkWidget *id_label = gtk_label_new("ID de tâche:");
    GtkWidget *name_label = gtk_label_new("Nom de tâche:");
    GtkWidget *description_label = gtk_label_new("Description de tâche:");
    GtkWidget *date_label = gtk_label_new("Date de tâche:");

    GtkWidget *id_entry = gtk_entry_new();
    GtkWidget *name_entry = gtk_entry_new();
    GtkWidget *description_entry = gtk_entry_new();
    GtkWidget *date_entry = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), id_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), id_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), description_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), description_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), date_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), date_entry, 1, 3, 1, 1);

    // Show all widgets in the dialog
    gtk_widget_show_all(edit_task_dialog);

    // Wait for user response
    gint result = gtk_dialog_run(GTK_DIALOG(edit_task_dialog));

    if (result == GTK_RESPONSE_OK) {
        // Retrieve entered task details
        const gchar *edit_id = gtk_entry_get_text(GTK_ENTRY(id_entry));
        const gchar *edit_name = gtk_entry_get_text(GTK_ENTRY(name_entry));
        const gchar *edit_description = gtk_entry_get_text(GTK_ENTRY(description_entry));
        const gchar *edit_date = gtk_entry_get_text(GTK_ENTRY(date_entry));

        // Open the tasks file associated with the user
        char filename[50];
        sprintf(filename, "%s_tasks.txt", username);
        FILE *tasks_file = fopen(filename, "r");

        if (tasks_file != NULL) {
            // Create a temporary file for writing modified task information
            char temp_filename[50];
            sprintf(temp_filename, "%s_tasks_temp.txt", username);
            FILE *temp_file = fopen(temp_filename, "w");

            if (temp_file != NULL) {
                char line[256]; // Adjust the size as needed
                gboolean task_found = FALSE;

                // Read each line from the original file
                while (fgets(line, sizeof(line), tasks_file) != NULL) {
                    char current_id[50];
                    if (sscanf(line, "%s", current_id) == 1 && strcmp(current_id, edit_id) == 0) {
                        // Found the task with the provided ID, replace it with the new information
                        fprintf(temp_file, "%s - %s - %s - %s\n", edit_id, edit_name, edit_description, edit_date);
                        task_found = TRUE;
                    } else {
                        // Copy the line unchanged to the temporary file
                        fprintf(temp_file, "%s", line);
                    }
                }

                // Close both files
                fclose(tasks_file);
                fclose(temp_file);

                // Remove the original file and rename the temporary file
                remove(filename);
                rename(temp_filename, filename);

                if (task_found) {
                    g_print("Tâche modifiée avec succès !\n");

                    // Refresh and display the tasks in the GtkTextView
                    // (You will need to implement the function to load and display tasks)
                    // ReloadTasks(username);

                } else {
                    // Display an error message for task not found
                    GtkWidget *error_dialog = gtk_message_dialog_new(GTK_WINDOW(edit_task_dialog),
                                                                     GTK_DIALOG_MODAL,
                                                                     GTK_MESSAGE_ERROR,
                                                                     GTK_BUTTONS_OK,
                                                                     "Tâche avec l'ID %s introuvable.", edit_id);
                    gtk_dialog_run(GTK_DIALOG(error_dialog));
                    gtk_widget_destroy(error_dialog);
                }
            } else {
                g_error("Erreur lors de la création du fichier temporaire pour la modification de la tâche.\n");
            }
        } else {
            g_error("Erreur lors de l'ouverture du fichier de tâches pour l'utilisateur : %s\n", username);
        }
    }

    // Close the dialog
    gtk_widget_destroy(edit_task_dialog);
}


// Function to handle delete task button click
void on_delete_task_button_clicked(GtkWidget *widget, gpointer data) {
    const gchar *username = (const gchar *)data;

    // Create a new dialog window for task deletion
    GtkWidget *delete_dialog = gtk_dialog_new_with_buttons("Supprimer la tâche",
                                                           GTK_WINDOW(main_window),
                                                           GTK_DIALOG_MODAL,
                                                           "Supprimer",
                                                           GTK_RESPONSE_ACCEPT,
                                                           "Annuler",
                                                           GTK_RESPONSE_CANCEL,
                                                           NULL);

    // Create a grid to organize the dialog content
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(delete_dialog))), grid);

    // Labels and entry field for task ID
    GtkWidget *id_label = gtk_label_new("Task ID:");
    GtkWidget *id_entry = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), id_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), id_entry, 1, 0, 1, 1);

    // Show all widgets in the dialog
    gtk_widget_show_all(delete_dialog);

    // Wait for user response
    gint result = gtk_dialog_run(GTK_DIALOG(delete_dialog));

    if (result == GTK_RESPONSE_ACCEPT) {
        // Retrieve entered task ID
        const gchar *task_id = gtk_entry_get_text(GTK_ENTRY(id_entry));

        // Open the user's tasks file for reading
        char filename[50];
        sprintf(filename, "%s_tasks.txt", username);
        FILE *tasks_file = fopen(filename, "r");

        if (tasks_file != NULL) {
            // Create a temporary file for writing
            FILE *temp_file = fopen("temp_tasks_file.txt", "w");

            // Initialize variables to store the current line and a flag indicating whether the task ID is found
            char current_line[256];
            gboolean task_found = FALSE;

            // Read each line of the file
            while (fgets(current_line, sizeof(current_line), tasks_file) != NULL) {
                // Check if the task ID matches the one entered by the user
                if (strstr(current_line, task_id) == NULL) {
                    // If not, write the line to the temporary file
                    fprintf(temp_file, "%s", current_line);
                } else {
                    // If found, set the flag to indicate success
                    task_found = TRUE;
                }
            }

            // Close the files
            fclose(tasks_file);
            fclose(temp_file);

            if (task_found) {
                // Replace the original file with the temporary file
                remove(filename);
                rename("temp_tasks_file.txt", filename);

                g_print("Tâche supprimée avec succès !\n");

                // Refresh and display the tasks in the GtkTextView
                // (You will need to implement the function to load and display tasks)
                // ReloadTasks(username);
            } else {
                // Display an error message for task not found
                GtkWidget *error_dialog = gtk_message_dialog_new(GTK_WINDOW(delete_dialog),
                                                                 GTK_DIALOG_MODAL,
                                                                 GTK_MESSAGE_ERROR,
                                                                 GTK_BUTTONS_OK,
                                                                 "Tâche avec l'ID %s introuvable.", task_id);
                gtk_dialog_run(GTK_DIALOG(error_dialog));
                gtk_widget_destroy(error_dialog);
            }
        } else {
            // Display an error message for file not found
            GtkWidget *error_dialog = gtk_message_dialog_new(GTK_WINDOW(delete_dialog),
                                                             GTK_DIALOG_MODAL,
                                                             GTK_MESSAGE_ERROR,
                                                             GTK_BUTTONS_OK,
                                                             "Erreur lors de l'ouverture du fichier de tâches pour l'utilisateur: %s", username);
            gtk_dialog_run(GTK_DIALOG(error_dialog));
            gtk_widget_destroy(error_dialog);
        }
    }

    // Close the dialog
    gtk_widget_destroy(delete_dialog);
}


// Function to handle quit program button click
void on_quit_program_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Cliquez sur le bouton Quitter le programme\n");
    gtk_main_quit();
}


void on_search_entry_changed(GtkWidget *widget, gpointer data) {
    const gchar *search_text = gtk_entry_get_text(GTK_ENTRY(widget));

    // Update the label with the search text
    gtk_label_set_text(GTK_LABEL(task_label), search_text);
}

// Function to handle create account button click
void on_create_account_button_clicked(GtkWidget *widget, gpointer data) {
    // Create a new window for account creation
    create_account_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(create_account_window), "Créer un compte");
    gtk_window_set_default_size(GTK_WINDOW(create_account_window), 400, 200);

    // Vertical box to hold widgets in the account creation window
    GtkWidget *create_account_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_container_add(GTK_CONTAINER(create_account_window), create_account_box);

    // Username field in account creation window
    GtkWidget *create_username_label = gtk_label_new("Saisissez votre nom d'utilisateur:");
    gtk_box_pack_start(GTK_BOX(create_account_box), create_username_label, FALSE, FALSE, 10);
    create_username_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(create_account_box), create_username_entry, FALSE, FALSE, 5);

    // Password field in account creation window
    GtkWidget *create_password_label = gtk_label_new("Saisissez le mot de passe:");
    gtk_box_pack_start(GTK_BOX(create_account_box), create_password_label, FALSE, FALSE, 5);
    create_password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(create_password_entry), FALSE); // Hide password
    gtk_box_pack_start(GTK_BOX(create_account_box), create_password_entry, FALSE, FALSE, 5);

    // Create button in account creation window
    create_button = gtk_button_new_with_label("Créer un compte");
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
                                                   "Un compte avec le même nom d'utilisateur existe déjà.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    } else {
        // Create a new file with username and password as its name
        file = fopen(filename, "w");

        if (file != NULL) {
            fprintf(file, "Nom du compte: %s\nMot de passe: %s\n", create_username, create_password);
            fclose(file);

            // Display a pop-up for a successfully created account
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(create_account_window),
                                                       GTK_DIALOG_MODAL,
                                                       GTK_MESSAGE_INFO,
                                                       GTK_BUTTONS_OK,
                                                       "Compte créé avec succès !");
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
                                                       "Erreur - Fichier compte.");
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
    gtk_window_set_title(GTK_WINDOW(window), "Planifique - Connexion");
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
    login_button = gtk_button_new_with_label("Se connecter");
    gtk_box_pack_start(GTK_BOX(main_box), login_button, FALSE, FALSE, 10);

    // Create account button
    create_account_button = gtk_button_new_with_label("Créer une compte");
    gtk_box_pack_start(GTK_BOX(main_box), create_account_button, FALSE, FALSE, 10);

    // Create a label for tasks
    task_label = gtk_label_new(NULL);
    gtk_box_pack_start(GTK_BOX(main_box), task_label, TRUE, TRUE, 5);

    // Connect "clicked" signals to respective functions
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_button_clicked), NULL);
    g_signal_connect(create_account_button, "clicked", G_CALLBACK(on_create_account_button_clicked), NULL);

    // Show all widgets
    gtk_widget_show_all(window);

    // Keep track of the login window
    login_window = window;

    // Enter the main loop
    gtk_main();

    return 0;
}
