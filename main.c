#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

typedef struct {
    char category[50];
    char name[50];
    char description[100];
    char date[20];
    gboolean completed;
} Task;

// Function prototypes
void on_login_button_clicked(GtkWidget *widget, gpointer data);
void on_create_account_button_clicked(GtkWidget *widget, gpointer data);
void on_create_button_clicked(GtkWidget *widget, gpointer data);
void open_main_window(const gchar *username);
void on_search_entry_changed(GtkWidget *widget, gpointer data);
void on_task_selected(GtkWidget *widget, gpointer data);
void on_completed_tasks_toggled(GtkWidget *widget, gpointer data);
void on_add_task_button_clicked(GtkWidget *widget, gpointer data);
int load_tasks_from_file(const gchar *username, Task *tasks, int max_tasks);

// Widgets for the main window
GtkWidget *username_entry;
GtkWidget *password_entry;

// Widgets for the account creation window
GtkWidget *create_account_window;
GtkWidget *create_username_entry;
GtkWidget *create_password_entry;
GtkWidget *create_button;

// Main application window
GtkWidget *main_window;
GtkWidget *login_window; // To keep track of the login window

Task tasks[100]; // Adjust the size as needed
int num_tasks = 0;  // Initialize num_tasks to zero
const gchar *username;  // Declare username
const gchar *global_username;

// Function to create a ListBoxRow for a task
GtkWidget *create_task_row(Task *task) {
    // Create a ListBoxRow
    GtkWidget *row = gtk_list_box_row_new();

    // Create a label with task information
    GtkWidget *label = gtk_label_new(task->name);  // You can customize this based on your task structure

    // Add the label to the row
    gtk_container_add(GTK_CONTAINER(row), label);

    // Connect "activate" signal to the on_task_selected function
    g_signal_connect(row, "activate", G_CALLBACK(on_task_selected), NULL);

    return row;
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
        fscanf(file, "Username: %*s\nPassword: %[^\n]", stored_password);
        fclose(file);

        // Compare entered password with stored password
        if (strcmp(entered_password, stored_password) == 0) {
            // Successful login
            g_print("Login successful!\n");
            // Open the main window
            open_main_window(entered_username);
        } else {
            // Failed login
            g_print("Incorrect username or password.\n");
            // Display an error message to the user
        }
    } else {
        // File not found, user does not exist
        g_print("User does not exist.\n");
        // Display an error message to the user
    }
}

// Function to open a new main window with a search bar and task list
void open_main_window(const gchar *username) {
    g_print("Bienvenue, %s\n", username);
     // Load tasks from file
    num_tasks = load_tasks_from_file(username, tasks, sizeof(tasks) / sizeof(tasks[0]));
    global_username = g_strdup(username);
    
    // Creating a new main window
    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "Planifique - Main");
    gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 600);

    // Vertical box to hold widgets in the main window
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(main_window), main_box);

    // Search bar
    GtkWidget *search_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(main_box), search_entry, FALSE, FALSE, 5);

    GtkWidget *completed_tasks_checkbox = gtk_check_button_new_with_label("Afficher seulement les tâches complétées");
    gtk_box_pack_start(GTK_BOX(main_box), completed_tasks_checkbox, FALSE, FALSE, 5);   

    GtkWidget *add_task_button = gtk_button_new_with_label("Ajouter une tâche");
    gtk_box_pack_start(GTK_BOX(main_box), add_task_button, FALSE, FALSE, 5);

    // Scrolled window for the task list
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    // Set a fixed size for the scrolled window (replace 600, 400 with your desired size)
    gtk_widget_set_size_request(scrolled_window, 600, 300);

    gtk_box_pack_start(GTK_BOX(main_box), scrolled_window, FALSE, FALSE, 5);

    // ListBox to hold tasks
    GtkWidget *tasks_listbox = gtk_list_box_new();
    gtk_container_add(GTK_CONTAINER(scrolled_window), tasks_listbox);

    // Example tasks (replace it with your actual task list)
    for (int i = 0; i < num_tasks; ++i) {
    GtkWidget *task_row = create_task_row(&tasks[i]);
    gtk_container_add(GTK_CONTAINER(tasks_listbox), task_row);
    }

    // Connect "changed" signal to filter tasks function
    g_signal_connect(search_entry, "changed", G_CALLBACK(on_search_entry_changed), tasks_listbox);

    // Connect "toggled" signal to filter tasks function
    g_signal_connect(completed_tasks_checkbox, "toggled", G_CALLBACK(on_completed_tasks_toggled), tasks_listbox);

    // Connect "clicked" signal to add task function
    g_signal_connect(add_task_button, "clicked", G_CALLBACK(on_add_task_button_clicked), tasks_listbox);   
    
    // Show all widgets in the main window
    gtk_widget_show_all(main_window);

    // Close the login window
    gtk_widget_destroy(login_window);
}

// Function to handle task selection
void on_task_selected(GtkWidget *widget, gpointer data) {
    // You can add your logic here to handle the selected task
    // For example, you can get the text of the selected task label
    const gchar *task_text = gtk_label_get_text(GTK_LABEL(gtk_bin_get_child(GTK_BIN(widget))));
    g_print("Selected Task: %s\n", task_text);
}

// Function to handle changes in the search entry
void on_search_entry_changed(GtkWidget *widget, gpointer data) {
    const gchar *search_text = gtk_entry_get_text(GTK_ENTRY(widget));

    // Perform the filtering based on the entered text
    // Replace this with your actual filtering criteria and logic
    g_print("Filtering tasks based on: %s\n", search_text);

    // For now, just update the label with the search text
    gtk_label_set_text(GTK_LABEL(data), g_strdup_printf("Task List: %s", search_text));
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

// Add this function to handle the toggle of the completed tasks checkbox
void on_completed_tasks_toggled(GtkWidget *widget, gpointer data) {
    // Implement your logic here to filter out completed tasks
    gboolean show_completed = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
    // You may want to reload the task list based on the show_completed value
    // For simplicity, I'm printing the status here
    if (show_completed) {
        g_print("Show completed tasks.\n");
    } else {
        g_print("Hide completed tasks.\n");
    }
}

// Function to save tasks to a text file
void save_tasks_to_file(const gchar *username, const Task *tasks, int num_tasks) {
    char filename[50];
    snprintf(filename, sizeof(filename), "%s_tasks.txt", username);

    FILE *file = fopen(filename, "w");
    if (file != NULL) {
        for (int i = 0; i < num_tasks; ++i) {
            fprintf(file, "%s|%s|%s|%s|%d\n", tasks[i].category, tasks[i].name,
                    tasks[i].description, tasks[i].date, tasks[i].completed);
        }
        fclose(file);
    } else {
        g_print("Error opening file for writing.\n");
    }
}

// Function to load tasks from a text file
int load_tasks_from_file(const gchar *username, Task *tasks, int max_tasks) {
    char filename[50];
    snprintf(filename, sizeof(filename), "%s_tasks.txt", username);

    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        int i = 0;
        while (i < max_tasks && fscanf(file, "%49[^|]|%49[^|]|%99[^|]|%19[^|]|%d\n",
                                       tasks[i].category, tasks[i].name, tasks[i].description,
                                       tasks[i].date, &tasks[i].completed) == 5) {
            i++;
        }
        fclose(file);
        return i;
    } else {
        g_print("Error opening file for reading.\n");
        return 0;
    }
}



// Function to handle the click of the "Add Task" button
void on_add_task_button_clicked(GtkWidget *widget, gpointer data) {
    // Implement your logic here to add a new task
    Task new_task;
    // You may want to open a dialog or another window for user input
    // for task details (category, name, description, date, etc.)

    // For simplicity, let's assume some default values
    snprintf(new_task.category, sizeof(new_task.category), "Default Category");
    snprintf(new_task.name, sizeof(new_task.name), "New Task");
    snprintf(new_task.description, sizeof(new_task.description), "Description for New Task");
    snprintf(new_task.date, sizeof(new_task.date), "2024-01-15"); // Replace with actual date
    new_task.completed = FALSE;

    // Add the new task to the tasks array
    if (num_tasks < sizeof(tasks) / sizeof(tasks[0])) {
        tasks[num_tasks++] = new_task;

        // Save the updated tasks to the file
        save_tasks_to_file(username, tasks, num_tasks);

        // Clear the existing rows in the listbox
        GList *children, *iter;
        children = gtk_container_get_children(GTK_CONTAINER(data));
        for (iter = children; iter != NULL; iter = g_list_next(iter)) {
            gtk_widget_destroy(GTK_WIDGET(iter->data));
        }
        g_list_free(children);

        // Reload and display tasks in the listbox
        for (int i = 0; i < num_tasks; ++i) {
            GtkWidget *task_row = create_task_row(&tasks[i]);
            gtk_container_add(GTK_CONTAINER(data), task_row);
        }
    } else {
        g_print("Reached the maximum number of tasks.\n");
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

    // Keep track of the login window
    login_window = window;

    // Enter the main loop
    gtk_main();

    return 0;
}
