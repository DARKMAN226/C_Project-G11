#ifndef TRANSACTION_HISTORY_H
#define TRANSACTION_HISTORY_H

#include "product.h" // voila cote pour date et transaction

#define MAX_GLOBAL_TRANSACTIONS 1000 // cote du ombre max de transactions 
#define MAX_USERNAME_LEN 30 // le nom du user 

// voila un peu la structure pour une transaction 
typedef struct {
    int product_id; //ca ID du produit concerne
    char product_name[MAX_PRODUCT_NAME_LEN]; // nom du produit
    Date date; // la date de la transaction
    TransactionType type; // type de transaction (entree ou son sortie)
    int quantity; // Quantite concernee
    int category_index; //  Index de la categorie du produit
    char username[MAX_USERNAME_LEN]; //  user qui a fait la transaction
} GlobalTransaction;

//ca cest la fonctions pour gerer l'historique des transactions
void load_global_transactions(); // charge l'historique depuis le fichier
void save_global_transactions(); // save l'historique dans le fichier
void add_global_transaction(int product_id, int category_index, const char* product_name, int quantity, TransactionType type, Date date, const char* username); // ajouter une transaction 
void list_all_global_transactions(); // afficher toutes les transactions 

#endif // TRANSACTION_HISTORY_H
