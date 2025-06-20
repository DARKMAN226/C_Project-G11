#ifndef CATEGORY_H
#define CATEGORY_H

#define MAX_CATEGORIES 50
#define MAX_CATEGORY_NAME_LEN 30

// on garde une categorie avec juste un nom par exemple
typedef struct 
{
    char name[MAX_CATEGORY_NAME_LEN];
} 
Category;

// on load les categories depuis le fichier
void load_categories();
// on save les categories dans le fichier
void save_categories();

// on affiche les categories
void list_categories();
// on ajoute une categorie
int add_category();
// on supprime une categorie
int delete_category();
// on modifie une categorie
int modify_category();

// on compte combien y en a
int get_category_count();
// on recupere le nom d une categorie
const char* category_get_name(int index);

#endif
