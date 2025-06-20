#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "user_management.h" 


// ici on garde les users en memoire, c est notre base a nous
static User users[MAX_USERS];
static int user_count = 0;

// variables globales pour savoir si on est co ou pas
bool is_logged_in = false;
char current_user[MAX_USERNAME_LEN] = "";

// nom du fichier ou on stoke les users
const char *user_file = "users.dat";

// ici je charge les users depuis le fichier, si y en a
void load_users()
{
    FILE *f = fopen(user_file, "rb");
    if (!f)
    {
        user_count = 0; // si fichier absent de la on part de zero
        return;
    }
    fread(&user_count, sizeof(int), 1, f);
    fread(users, sizeof(User), user_count, f);
    fclose(f);
}

// ici je sauvegarde les users dans le fichier, pour pas perdre les comptes des users
void save_users()
{
    FILE *f = fopen(user_file, "wb");
    if (!f)
    {
        printf("error saving users\n");
        return;
    }
    fwrite(&user_count, sizeof(int), 1, f);
    fwrite(users, sizeof(User), user_count, f);
    fclose(f);
}

// ici on ajoute un nouvel utilisateur, cest l'inscription
int register_user() {
    if (user_count >= MAX_USERS)
    {
        printf("SORRY,Too many users, cant add more\n");
        return 0;
    }
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];

    printf("Type your username: ");
    scanf(" %29[^\n]", username);

    // on regarde si le pseudo est deja pris
    for (int i = 0; i < user_count; i++)
    {
        if (strcmp(users[i].username, username) == 0)
        {
            printf("Unfortunatly this username is already taken, sorry\n");
            return 0;
        }
    }

    printf("Type your password: ");
    scanf(" %29[^\n]", password);

    // on stoque le nouveau user
    strcpy(users[user_count].username, username);
    strcpy(users[user_count].password, password);
    user_count++;
    save_users();
    printf("Registration success, welcome %s!\n", username);
    return 1;
}

// ici le user essaie de se connecter
int login_user()
{
    if (is_logged_in)
    {
        printf("you are already logged in as %s\n", current_user);
        return 0;
    }
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];

    printf("Username: ");
    scanf(" %29[^\n]", username);
    printf("Password: ");
    scanf(" %29[^\n]", password);

    // on cherche dans la liste
    for (int i = 0; i < user_count; i++)
    {
        if (strcmp(users[i].username, username) == 0 &&
            strcmp(users[i].password, password) == 0)
            {
            is_logged_in = true;
            strcpy(current_user, username);
            printf("Login ok, hi %s!\n", current_user);
            return 1;
        }
    }
    printf("Oups Wrong username or password\n");
    return 0;
}

// ici on se deconnecte
void logout_user()
{
    if (!is_logged_in)
    {
        printf("you are not logged in dude\n");
        return;
    }
    is_logged_in = false;
    current_user[0] = '\0';
    printf("Logout ok\n");
}
