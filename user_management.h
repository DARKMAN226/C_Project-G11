#ifndef USER_MANAGEMENT_H
#define USER_MANAGEMENT_H

#include <stdbool.h>

#define MAX_USERS 100
#define MAX_USERNAME_LEN 30
#define MAX_PASSWORD_LEN 30

// on stocke un user avec pseudo et motdp
typedef struct {
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
} User;

// variable globale pour dire si on est connecter ou pas
extern bool is_logged_in;
// pseudo de celui qui est connecter
extern char current_user[MAX_USERNAME_LEN];

// on load les users depuis le fichier
void load_users();
// on save les users dans le fichier
void save_users();

// on cree un compte
int register_user();
// on se connecte
int login_user();
// on se deconnecte
void logout_user();

#endif
