# Planifique
Gestionnaire des tâches - Made with C &amp; GTK Library

**Pour compiler le fichier source:** gcc -o planifique main.c `pkg-config --cflags --libs gtk+-3.0` (Notez qu'il faut d'abord installer GTK dans votre machine)

Les pilliers de la bibliothèque GTK:

# Widgets (Widgets):
Les widgets sont les éléments de base de l'interface utilisateur dans GTK. Ce sont des composants graphiques tels que les boutons, les fenêtres, les boîtes de dialogue, etc., qui peuvent être assemblés pour créer des interfaces graphiques.

# Layout Managers (Gestionnaires de disposition):
Les gestionnaires de disposition contrôlent la disposition des widgets à l'intérieur des conteneurs. GTK offre différents gestionnaires de disposition pour organiser efficacement les widgets dans une fenêtre.

# Signals and Callbacks (Signaux et rappels):
Les signaux sont des événements générés par les widgets, tels que le clic d'un bouton. Les rappels (callbacks) sont des fonctions associées à ces signaux pour définir le comportement en réponse à ces événements.

# Event Handling (Gestion des événements):
La gestion des événements permet aux développeurs de réagir aux actions de l'utilisateur, tels que les clics de souris, les saisies clavier, etc. Cela garantit une interaction utilisateur réactive.

# Styles and Themes (Styles et thèmes):
GTK offre des mécanismes pour personnaliser l'apparence des applications en utilisant des thèmes et des styles. Cela permet d'harmoniser visuellement l'application avec l'environnement utilisateur.

---------------------------------------------

# Fonction : `int main(int argc, char *argv[])`

## But :
Cette fonction est le point d'entrée principal de votre programme GTK. Elle initialise l'environnement GTK, crée la fenêtre principale de connexion de l'application "Planifique", configure les différents widgets, connecte les signaux aux fonctions correspondantes, et entre dans la boucle principale GTK.


## Fonctionnement :
1. **Initialisation GTK :** `gtk_init(&argc, &argv);` Initialise l'environnement GTK en passant les arguments de la ligne de commande.

2. **Création de la fenêtre :** `window = gtk_window_new(GTK_WINDOW_TOPLEVEL);` Crée une fenêtre GTK de niveau supérieur.

3. **Configuration de la fenêtre :**
    - `gtk_window_set_title(GTK_WINDOW(window), "Planifique - Connexion");` Définit le titre de la fenêtre.
    - `gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);` Définit la taille par défaut de la fenêtre.

4. **Création du conteneur principal :** `main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);` Crée une boîte verticale pour contenir les widgets.

5. **Ajout de la boîte au conteneur de la fenêtre :** `gtk_container_add(GTK_CONTAINER(window), main_box);`

6. **Ajout des widgets à la boîte :**
    - `welcome_label = gtk_label_new("Bienvenue sur Planifique:");` Crée une étiquette de bienvenue.
    - `username_label = gtk_label_new("Nom du compte:");` Crée une étiquette pour le nom d'utilisateur.
    - `username_entry = gtk_entry_new();` Crée un champ de saisie pour le nom d'utilisateur.
    - `password_label = gtk_label_new("Mot de passe:");` Crée une étiquette pour le mot de passe.
    - `password_entry = gtk_entry_new();` Crée un champ de saisie pour le mot de passe.
    - `login_button = gtk_button_new_with_label("Se connecter");` Crée un bouton de connexion.
    - `create_account_button = gtk_button_new_with_label("Créer un compte");` Crée un bouton pour créer un compte.
    - `task_label = gtk_label_new(NULL);` Crée une étiquette pour les tâches (peut-être utilisée ultérieurement).

7. **Connexion des signaux aux fonctions correspondantes :**
    - `g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_button_clicked), NULL);` Connecte le signal "clicked" du bouton de connexion à la fonction `on_login_button_clicked`.
    - `g_signal_connect(create_account_button, "clicked", G_CALLBACK(on_create_account_button_clicked), NULL);` Connecte le signal "clicked" du bouton de création de compte à la fonction `on_create_account_button_clicked`.

8. **Affichage de tous les widgets :** `gtk_widget_show_all(window);`

9. **Suivi de la fenêtre de connexion :** `login_window = window;` Permet de suivre la fenêtre de connexion pour une utilisation ultérieure.

10. **Entrée dans la boucle principale GTK :** `gtk_main();` Démarre la boucle principale GTK, où le programme attend les événements de l'utilisateur.

---------------------------------------------


# Fonction : `on_create_button_clicked`

## But :
Cette fonction est associée au clic sur le bouton de création de compte dans la fenêtre de création de compte. Elle vérifie d'abord si un fichier avec le même nom d'utilisateur existe déjà. Si tel est le cas, elle affiche un message d'erreur. Sinon, elle crée un nouveau fichier avec le nom d'utilisateur et le mot de passe, affiche un message de succès, puis ferme la fenêtre de création de compte.

## Contenu :
- Récupère le nom d'utilisateur et le mot de passe à partir des champs de saisie.
- Vérifie si un fichier avec le même nom d'utilisateur existe.
- Affiche un message d'erreur si le fichier existe, sinon crée un nouveau fichier.
- Écrit le nom d'utilisateur et le mot de passe dans le fichier.
- Affiche un message de succès.
- Ferme la fenêtre de création de compte.

## Remarque :
Cette fonction gère les étapes de vérification, de création de fichier, et d'interaction avec l'interface utilisateur lors de la création d'un compte.


---------------------------------------------

# Fonction : `on_create_account_button_clicked`

## But :
Cette fonction gère le clic sur le bouton de création de compte. Son objectif est de créer une nouvelle fenêtre pour la création de compte, avec des champs pour le nom d'utilisateur et le mot de passe, ainsi qu'un bouton pour effectuer la création.

## Contenu :
- Crée une nouvelle fenêtre GTK.
- Ajoute des champs de saisie pour le nom d'utilisateur et le mot de passe.
- Ajoute un bouton de création de compte.
- Connecte le signal "clicked" du bouton à la fonction `on_create_button_clicked`.
- Affiche tous les widgets dans la fenêtre de création de compte.

---------------------------------------------

# Fonction : `on_delete_task_button_clicked`

## But :
Cette fonction gère le clic sur le bouton de suppression de tâche. Son objectif est de permettre à l'utilisateur de supprimer une tâche en fournissant un ID de tâche. La fonction crée une boîte de dialogue avec un champ d'entrée pour l'ID de la tâche et effectue la suppression si l'ID correspond à une tâche existante.

## Contenu :
- Récupère le nom d'utilisateur à partir des données passées.
- Crée une boîte de dialogue pour la suppression de tâche.
- Affiche un champ d'entrée pour l'ID de la tâche dans la boîte de dialogue.
- Attend la réponse de l'utilisateur.
- Si l'utilisateur confirme la suppression :
  - Ouvre le fichier de tâches de l'utilisateur.
  - Crée un fichier temporaire pour la réécriture.
  - Parcours le fichier, supprime la tâche correspondant à l'ID.
  - Actualise le fichier original avec le fichier temporaire.
  - Affiche un message de réussite.
  - (Commenté : Actualise et affiche les tâches dans GtkTextView).
- Si l'ID de la tâche n'est pas trouvé, affiche un message d'erreur.
- En cas d'erreur de fichier, affiche un message d'erreur.
- Ferme la boîte de dialogue.

---------------------------------------------

# Fonction : `on_edit_task_button_clicked`

## But :
Cette fonction gère le clic sur le bouton d'édition de tâche. Son objectif est de permettre à l'utilisateur de modifier les détails d'une tâche en fournissant un ID de tâche. La fonction crée une boîte de dialogue avec des champs d'entrée pour les détails de la tâche, effectue les modifications si l'ID correspond à une tâche existante, et affiche le résultat.

## Contenu :
- Récupère le nom d'utilisateur à partir des données passées.
- Crée une boîte de dialogue pour la modification de tâche.
- Affiche des champs d'entrée pour l'ID, le nom, la description et la date de la tâche.
- Attend la réponse de l'utilisateur.
- Si l'utilisateur confirme la modification :
  - Ouvre le fichier de tâches de l'utilisateur.
  - Crée un fichier temporaire pour la réécriture avec les modifications.
  - Parcours le fichier, modifie la tâche correspondant à l'ID.
  - Actualise le fichier original avec le fichier temporaire.
  - Affiche un message de réussite.
  - (Commenté : Actualise et affiche les tâches dans GtkTextView).
- Si l'ID de la tâche n'est pas trouvé, affiche un message d'erreur.
- En cas d'erreur de fichier, affiche un message d'erreur.
- Ferme la boîte de dialogue.

---------------------------------------------

# Fonction : `on_add_task_button_clicked`

## But :
Cette fonction gère le clic sur le bouton d'ajout de tâche. Son objectif est de permettre à l'utilisateur d'ajouter une nouvelle tâche en fournissant des détails tels que l'ID, le nom, la description et la date de la tâche. La fonction vérifie si l'ID de la tâche existe déjà dans le fichier de tâches de l'utilisateur et ajoute la tâche si l'ID est unique.

## Contenu :
- Récupère le nom d'utilisateur à partir des données passées.
- Crée une boîte de dialogue pour la saisie des détails de la tâche.
- Affiche des champs d'entrée pour l'ID, le nom, la description et la date de la tâche.
- Attend la réponse de l'utilisateur.
- Si l'utilisateur confirme l'ajout :
  - Vérifie si l'ID de la tâche existe déjà dans le fichier de tâches.
  - Si l'ID existe, affiche un message d'erreur.
  - Si l'ID est unique, ajoute la nouvelle tâche au fichier de tâches.
  - Actualise et affiche les tâches dans GtkTextView.
  - Affiche un message de réussite.
- En cas d'erreur de fichier, affiche un message d'erreur.
- Ferme la boîte de dialogue.

---------------------------------------------

# Fonction : `open_main_window`

## But :
Cette fonction ouvre une nouvelle fenêtre principale avec une liste de tâches. Elle vérifie si un fichier de tâches existe pour l'utilisateur. Si le fichier existe, la fonction charge et affiche les tâches dans une GtkListBox. Si le fichier n'existe pas, elle crée un nouveau fichier de tâches, y ajoute un message d'accueil, puis charge et affiche les tâches.

## Contenu :
- Crée une nouvelle fenêtre principale.
- Vérifie l'existence du fichier de tâches pour l'utilisateur.
  - Si le fichier existe :
    - Charge et affiche les tâches dans une GtkListBox.
    - Ajoute une barre de recherche et des boutons pour ajouter, modifier, supprimer des tâches, et quitter le programme.
    - Connecte les signaux des boutons aux fonctions respectives.
    - Connecte le signal "changed" de la barre de recherche à la fonction de filtrage des tâches.
  - Si le fichier n'existe pas :
    - Crée un nouveau fichier de tâches.
    - Ajoute un message d'accueil.
    - Répète les étapes pour créer la fenêtre principale comme pour le cas où le fichier existe.

---------------------------------------------

# Fonction : `on_login_button_clicked`

## But :
Cette fonction gère le clic sur le bouton de connexion. Elle récupère le nom d'utilisateur et le mot de passe saisis, puis compare ce mot de passe avec celui stocké dans un fichier associé au nom d'utilisateur. Si la comparaison est réussie, elle affiche un message de connexion réussie et ouvre la fenêtre principale. Sinon, elle affiche un message d'échec de connexion.

## Contenu :
- Récupère le nom d'utilisateur et le mot de passe saisis.
- Ouvre le fichier correspondant au nom d'utilisateur pour récupérer le mot de passe stocké.
- Compare le mot de passe saisi avec le mot de passe stocké.
  - Si la comparaison est réussie :
    - Affiche un message de connexion réussie.
    - Ouvre la fenêtre principale en appelant la fonction `open_main_window` avec le nom d'utilisateur.
  - Sinon :
    - Affiche un message d'identifiant ou de mot de passe incorrect.
- Si le fichier n'est pas trouvé, affiche un message indiquant que l'utilisateur n'existe pas.

---------------------------------------------

# Fonction : `LoadAndDisplayTasks`

## But :
Cette fonction charge et affiche les tâches d'un utilisateur à partir d'un fichier associé à son nom d'utilisateur. Elle ouvre le fichier, lit chaque ligne (représentant une tâche) et crée des étiquettes GtkLabel pour chaque tâche. Ces étiquettes sont ensuite ajoutées à un GtkListBox pour affichage.

## Contenu :
- Ouvre le fichier de tâches associé à l'utilisateur.
- Efface les tâches existantes dans le GtkListBox.
- Lit chaque ligne du fichier, créant un GtkLabel pour chaque tâche.
- Ajoute les GtkLabel dans des GtkListBoxRows.
- Ajoute ces GtkListBoxRows au GtkListBox.
- Affiche des messages de débogage pour chaque tâche lue et ajoutée à la liste.
- Si le fichier n'est pas trouvé, affiche une erreur.
