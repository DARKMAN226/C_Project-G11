// es fonctions de gestion des fournisseurs

#include <stdio.h>
#include <string.h>
#include "supplier.h"

// la tableau pour stocker les fournisseur
static Supplier suppliers[MAX_SUPPLIERS];
static int supplier_count = 0;

//la nom du fichier de sauvegarde des fournisseurs
const char *supplier_file = "suppliers.dat";

// on charger les fournisseurs depuis le fichier
void load_suppliers()
{
    FILE *f = fopen(supplier_file, "rb");
    if (!f)
    {
        supplier_count = 0; // si le fichier n'existe pas, on commence avec 0 fournisseur
        return;
    }
    fread(&supplier_count, sizeof(int), 1, f);
    fread(suppliers, sizeof(Supplier), supplier_count, f);
    fclose(f);
}

// save les fournisseurs dans le fichier
void save_suppliers()
{
    FILE *f = fopen(supplier_file, "wb");
    if (!f)
    {
        printf("ERROR saving suppliers\n");
        return;
    }
    fwrite(&supplier_count, sizeof(int), 1, f);
    fwrite(suppliers, sizeof(Supplier), supplier_count, f);
    fclose(f);
}

// on generer un ID unique pour un fournisseur
static int generate_supplier_id()
{
    // on commence les ID a partir de 1000 pour les fournisseurs
    static int last_id = 1000;
    if (supplier_count > 0) {
        // trouver le plus grand ID existant pour eviter lles truc deux deux la
        for (int i = 0; i < supplier_count; i++) {
            if (suppliers[i].id > last_id) {
                last_id = suppliers[i].id;
            }
        }
        last_id++;
    }
    return last_id++;
}

// afficher la liste des fournisseurs sous forme de tableau sinon ca sera pas simple pour la lecture
void list_suppliers()
{
    if (supplier_count == 0)
    {
        printf("No suppliers registered yet.\n");
        return;
    }
    printf("+------+--------------------------------------------------+--------------------------------------------------+\n");
    printf("| ID   | Name                                             | Contact                                          |\n");
    printf("+------+--------------------------------------------------+--------------------------------------------------+\n");
    for (int i = 0; i < supplier_count; i++)
    {
        printf("| %-4d | %-48s | %-48s |\n", suppliers[i].id, suppliers[i].name, suppliers[i].contact);
    }
    printf("+------+--------------------------------------------------+--------------------------------------------------+\n");
}

// ajouter un nouveau fournisseur
int add_supplier()
{
    if (supplier_count >= MAX_SUPPLIERS)
    {
        printf("Moo many suppliers, cannot add more.\n");//sorry botherrr
        return 0;
    }
    char name[MAX_SUPPLIER_NAME_LEN];
    char contact[MAX_SUPPLIER_CONTACT_LEN];

    printf("Enter supplier name: ");
    scanf(" %49[^\n]", name);

    // verifier si le nom du fournisseur existe deja
    for (int i = 0; i < supplier_count; i++)
    {
        if (strcmp(suppliers[i].name, name) == 0)
        {
            printf("upplier with this name already exists.\n");
            return 0;
        }
    }

    printf("Enter supplier contact info: ");
    scanf(" %99[^\n]", contact);

    // ajouter le nouveau fournisseur
    suppliers[supplier_count].id = generate_supplier_id();
    strcpy(suppliers[supplier_count].name, name);
    strcpy(suppliers[supplier_count].contact, contact);
    supplier_count++;
    save_suppliers();
    printf("Supplier added successfully with ID %d.\n", suppliers[supplier_count-1].id);
    return 1;
}

// supprimer un fournisseur par son ID
int delete_supplier()
{
    if (supplier_count == 0)
    {
        printf("No suppliers to delete.\n");
        return 0;
    }
    list_suppliers();
    int id_to_delete;
    printf("Enter ID of supplier to delete: ");
    if (scanf("%d", &id_to_delete) != 1)
    {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return 0;
    }

    int found_index = -1;
    for (int i = 0; i < supplier_count; i++)
    {
        if (suppliers[i].id == id_to_delete)
        {
            found_index = i;
            break;
        }
    }

    if (found_index == -1)
    {
        printf("Supplier with ID %d not found.\n", id_to_delete);
        return 0;
    }

    // Decaler un peu les elements apres celui supprimer
    for (int i = found_index; i < supplier_count - 1; i++)
    {
        suppliers[i] = suppliers[i + 1];
    }
    supplier_count--;
    save_suppliers();
    printf("Supplier deleted successfully.\n");
    return 1;
}

// modifier un fournisseur par son ID
int modify_supplier()
{
    if (supplier_count == 0)
    {
        printf("No suppliers to modify.\n");
        return 0;
    }
    list_suppliers();
    int id_to_modify;
    printf("Enter ID of supplier to modify: ");
    if (scanf("%d", &id_to_modify) != 1)
    {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return 0;
    }

    int found_index = -1;
    for (int i = 0; i < supplier_count; i++)
    {
        if (suppliers[i].id == id_to_modify)
        {
            found_index = i;
            break;
        }
    }

    if (found_index == -1)
    {
        printf("Supplier with ID %d not found.\n", id_to_modify);
        return 0;
    }

    printf("Modifying supplier '%s'.\n", suppliers[found_index].name);

    char new_name[MAX_SUPPLIER_NAME_LEN];
    char new_contact[MAX_SUPPLIER_CONTACT_LEN];

    printf("Enter new name (or press Enter to keep old: '%s'): ", suppliers[found_index].name);
    getchar(); 
    fgets(new_name, MAX_SUPPLIER_NAME_LEN, stdin);
    new_name[strcspn(new_name, "\n")] = 0; 
    if (strlen(new_name) > 0)
    {
        //on verifier si le nouveau nom existe deja 
        for (int i = 0; i < supplier_count; i++) 
		{
            if (i != found_index && strcmp(suppliers[i].name, new_name) == 0) 
			{
                printf("Supplier with this name already exists.\n");
                return 0;
            }
        }
        strcpy(suppliers[found_index].name, new_name);
    }

    printf("Enter new contact info (or press Enter to keep old: '%s'): ", suppliers[found_index].contact);
    fgets(new_contact, MAX_SUPPLIER_CONTACT_LEN, stdin);
    new_contact[strcspn(new_contact, "\n")] = 0; 
    if (strlen(new_contact) > 0)
    {
        strcpy(suppliers[found_index].contact, new_contact);
    }

    save_suppliers();
    printf("Supplier modified successfully.\n");
    return 1;
}
