#include <stdio.h>
#include <string.h>
#include "category.h"

// ici on garde les categories en memoire, c est notre liste a nous
static Category categories[MAX_CATEGORIES];
static int category_count = 0;

// c est le nom du fichier ou on va regarder les categories, genre la sauvegarde
const char *category_file = "categories.dat";

// ici on charge les categories depuis fichier, comme ca on voit tout ce qu il y a
void load_categories() 
{
    FILE *f = fopen(category_file, "rb");
    if (!f) 
	{
        category_count = 0; // si y a pas de fichier, on part de zero
        return;
    }
    fread(&category_count, sizeof(int), 1, f); // on lit le nombre de categories
    fread(categories, sizeof(Category), category_count, f); // on lit les categories
    fclose(f);
}

// ici on sauvegarde les categories dans le fichier, pour garder les modifs
void save_categories() 
{
    FILE *f = fopen(category_file, "wb");
    if (!f) 
	{
        printf("ERROR saving categories\n");
        return;
    }
    fwrite(&category_count, sizeof(int), 1, f); // on ecrit le nombre de categories
    fwrite(categories, sizeof(Category), category_count, f); // on ecrit les categories
    fclose(f);
}

// ici on affiche les categories en tableau, pour que ce soit clair
void list_categories() 
{
    if (category_count == 0) 
	{
        printf("There is no categories yet\n");
        return;
    }
    printf("+----+------------------------------+\n");
    printf("| ID | category name                |\n");
    printf("+----+------------------------------+\n");
    for (int i = 0; i < category_count; i++) 
	{
        printf("| %-2d | %-28s |\n", i + 1, categories[i].name); // on affiche id + nom
    }
    printf("+----+------------------------------+\n");
}

// ici on ajoute une categorie, on check si on peut encore en mettre
int add_category() 
{
    if (category_count >= MAX_CATEGORIES) 
	{
        printf("Too many categories, cant add more\n");
        return 0;
    }
    char name[MAX_CATEGORY_NAME_LEN];
    printf("Type new category name: ");
    scanf(" %29[^\n]", name);

    // on regarde si cette categorie existe deja, pour pas doublonner
    for (int i = 0; i < category_count; i++) 
	{
        if (strcmp(categories[i].name, name) == 0) 
		{
            printf("This category already exists Here\n");
            return 0;
        }
    }

    // on ajoute la nouvelle categorie a la fin
    strcpy(categories[category_count].name, name);
    category_count++;
    save_categories(); // on sauvegarde direct apres ajout
    printf("Category added with success\n");
    return 1;
}

// ici on supprime une categorie, on fait attention a pas se planter
int delete_category() 
{
    if (category_count == 0) 
	{
        printf("Empty, there is nothing to delete\n");
        return 0;
    }
    
	list_categories(); // on affiche les categories pour choisir
    int choice;
    printf("Choose the category to delete: ");
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > category_count) 
	{
        printf("INVALIDE CHOICE\n");
        while (getchar() != '\n'); // on nettoie le buffer
        return 0;
    }
    // on decale les categories apres celle que on a supprime
    for (int i = choice - 1; i < category_count - 1; i++) 
	{
        categories[i] = categories[i + 1];
    }
    category_count--;
    save_categories(); // on sauvegarde apres la suppression
    printf("This category has been deleted with success\n");
    return 1;
}

// ici on modifie une categorie, on change juste le nom
int modify_category() 
{
    if (category_count == 0) 
	{
        printf(" There are nothing to modify\n");
        return 0;
    }
    list_categories(); // on affiche pour choisir la cathegorie que on veut
    int choice;
    printf("Choose category to modify: ");
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > category_count) 
	{
        printf("INVALID CHOICE\n");
        while (getchar() != '\n'); 
        return 0;
    }
    char new_name[MAX_CATEGORY_NAME_LEN];
    printf("Type th new name: ");
    scanf(" %29[^\n]", new_name);

    // on regarde si le nouveau nom existe deja avant
    for (int i = 0; i < category_count; i++) 
	{
        if (strcmp(categories[i].name, new_name) == 0) 
		{
            printf("This name already exists\n");
            return 0;
        }
    }

    // on remplace l'ancien nom par le nouveau
    strcpy(categories[choice - 1].name, new_name);
    save_categories(); // on sauvegarde apres modif
    printf("This category has been modified\n");
    return 1;
}

// ici on retourne le nombre de categories qu on a
int get_category_count() 
{
    return category_count;
}

// ici on retourne le nom dune categorie a partir de son indice
const char* category_get_name(int index) 
{
    if (index < 0 || index >= category_count) 
	{
        return "";
    }
    return categories[index].name;
}
