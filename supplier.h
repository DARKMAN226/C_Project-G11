#ifndef SUPPLIER_H
#define SUPPLIER_H

#define MAX_SUPPLIERS 50
#define MAX_SUPPLIER_NAME_LEN 50
#define MAX_SUPPLIER_CONTACT_LEN 100

//tructure pour un fournisseur
typedef struct {
    int id; // iD unique du fournisseur
    char name[MAX_SUPPLIER_NAME_LEN]; // le nom du fournisseur
    char contact[MAX_SUPPLIER_CONTACT_LEN]; // Manus: Informations de contact
} Supplier;

//rentrons dans la fonctions pour gerer les fournisseurs
void load_suppliers(); // harger les fournisseurs depuis le fichier
void save_suppliers(); // le save
void list_suppliers(); // afficher la liste des fournisseurs
int add_supplier(); // ajouter un nouveau fournisseur
int delete_supplier(); // supprimer un fournisseur
int modify_supplier(); // modifier un fournisseur

#endif // SUPPLIER_H
