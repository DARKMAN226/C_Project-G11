#ifndef PRODUCT_H
#define PRODUCT_H

#include "category.h"
#include <stdbool.h>

#define MAX_PRODUCTS 200
#define MAX_PRODUCT_NAME_LEN 50
#define MAX_TRANSACTIONS 100

// on dit si c est une entree ou sortie de stock
typedef enum { ENTRY, EXIT } TransactionType;

// date simple
typedef struct {
    int day, month, year;
} Date;

// une transaction de stock, entree ou sortie
typedef struct {
    Date date;
    int quantity;
    TransactionType type;
} StockTransaction;

// un produit avec lien vers categorie
typedef struct {
    int id;
    int category_index;  // pour savoir ou il est
    char name[MAX_PRODUCT_NAME_LEN];
    float unit_price;
    StockTransaction transactions[MAX_TRANSACTIONS];
    int transaction_count;
} Product;

// variables globales pour acceder aux produits
extern Product products[MAX_PRODUCTS];
extern int product_count;

// on load les produits depuis fichier
void load_products();
// on save les produits dans le fichier
void save_products();

// on affiche les produits d une categorie
void list_products(int category_index);
// on ajoute un produit dans une categorie
int add_product(int category_index);
// on supprime un produit dans une categorie
int delete_product(int category_index);
// on modifie un produit dans une categorie
int modify_product(int category_index);

// on ajoute une transaction de stock (entree ou sortie)
int add_stock_transaction(Product *product, int quantity, TransactionType type, Date date);
// on affiche les transactions d un produit
void list_stock_transactions(Product *product);

// on cherche un produit par ID
int search_product_by_id(int id);
// on cherche un produit par nom (exact)
int search_product_by_name(const char *name);

// on compte combien de produits dans une categorie
int get_product_count(int category_index);
// on genere un id unique pour un produit
int generate_product_id();

#endif
