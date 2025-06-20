#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "product.h"
#include "category.h"
// Voilaa en  faite cest ici que jai ajouter les nouvelles options pour le fournisseur et les transactions
#include "transaction_history.h"
#include "user_management.h" 

// ici on garde les produits en memoire, c est notre stock a nous
Product products[MAX_PRODUCTS];
int product_count = 0;

// c est le nom du fichier ou on stocke les produits, genre la sauvegarde
const char *product_file = "products.dat";

// ici on charge les produits depuis fichier, comme ca on perd rien
void load_products() 
{
    FILE *f = fopen(product_file, "rb");
    if (!f) 
	{
        product_count = 0; // si y a pas de fichier on part de zero
        return;
    }
    fread(&product_count, sizeof(int), 1, f); // on lit le nombre de produits
    fread(products, sizeof(Product), product_count, f); // on lit les produits
    fclose(f);
}

// ici on sauvegarde les produits dans le fichier, pour garder les modifs
void save_products() 
{
    FILE *f = fopen(product_file, "wb");
    if (!f) 
	{
        printf( "ERROR in saving products\n");
        return;
    }
    fwrite(&product_count, sizeof(int), 1, f); // on ecrit le nombre de produits
    fwrite(products, sizeof(Product), product_count, f); // on entre les differents produits
    fclose(f);
}

// ici on genere un ID unique pour chaque produit, pour pas  se melanger
int generate_product_id() 
{
    static int last_id = 1000;
    // on verifie dabord que le nouvel ID est unique
    if (product_count > 0) {
        for (int i = 0; i < product_count; i++) {
            if (products[i].id >= last_id) {
                last_id = products[i].id + 1;
            }
        }
    } else {
        last_id = 1000; // Premier ID si aucun produit
    }
    return last_id++;
}


// ici on ajoute un produit dans une categorie, genre on rentre du stock
int add_product(int category_index)
{
    if (product_count >= MAX_PRODUCTS)
    {
        printf("Sorry there are too many products, you cannot add more\n");
        return 0;
    }

    char name[MAX_PRODUCT_NAME_LEN];
    float price;
    int qty;
    Date date;

    // on demande le nom du produit
    printf("Type product name: ");
    scanf(" %49[^\n]", name);

    // on demande le prix unitaire
    printf("Type unit price: ");
    if (scanf("%f", &price) != 1 || price < 0) // mais verifier que le prix est positif faut pas on va nous rentrer des sommes negatives isi , un user est toujours malhonnette
    {
        printf("INVALID PRICE\n");
        while (getchar() != '\n'); 
        return 0;
    }

    // on demande la quantite de depart
    printf("Type initial quantity: ");
    if (scanf("%d", &qty) != 1 || qty < 0)
    {
        printf("INVALID QUANTITY\n");
        while (getchar() != '\n');
        return 0;
    }

    // on demande la date d'entree du stock
    printf("Type entry date (DD MM YYYY): ");
    if (scanf("%d %d %d", &date.day, &date.month, &date.year) != 3)
    {
        printf("INVALID DATE\n");
        while (getchar() != '\n');
        return 0;
    }

    // on cree le produit et on ajoute la premiere transaction qui est le stock initial)
    Product p;
    p.id = generate_product_id();
    p.category_index = category_index;
    strcpy(p.name, name);
    p.unit_price = price;
    p.transaction_count = 0;

    StockTransaction t;
    t.date = date;
    t.quantity = qty;
    t.type = ENTRY;

    p.transactions[p.transaction_count++] = t;

    products[product_count++] = p;

    printf("Product has been added with ID %d\n", p.id);
    save_products(); // ca va sauvegarder les produits mis a jour

    // cest fini on enregistre la transaction
    add_global_transaction(p.id, p.category_index, p.name, qty, ENTRY, date, current_user);

    return 1;
}


// ici on affiche tous les produits d une categorie en tableau
void list_products(int category_index) 
{
    int found_count = 0;
    printf("+-------+--------------------------------------------------+----------+----------+\n");
    printf("| ID    | Name                                             | Price    | Quantity |\n");
    printf("+-------+--------------------------------------------------+----------+----------+\n");
    for (int i = 0; i < product_count; i++) 
	{
        if (products[i].category_index == category_index) 
		{
            found_count++;
            int total_qty = 0;
            // on calcule le stock total avec toutes les transactions
            for (int j = 0; j < products[i].transaction_count; j++) 
			{
                if (products[i].transactions[j].type == ENTRY)
                    total_qty += products[i].transactions[j].quantity;
                else
                    total_qty -= products[i].transactions[j].quantity;
            }
            printf("| %-5d | %-48s | %-8.2f | %-8d |\n",
                   products[i].id, products[i].name, products[i].unit_price, total_qty);
        }
    }
    if (found_count == 0) {
        printf("| No products found in this category.                                        |\n");
    }
    printf("+-------+--------------------------------------------------+----------+----------+\n");
}

// ici on supprime un produit d'une categorie,cest l;option supprimer la 
int delete_product(int category_index) 
{
    if (get_product_count(category_index) == 0) 
	{
        printf(" There are no product to delete in this category\n");
        return 0;
    }

    list_products(category_index);
    int prod_id;
    printf("Type product ID to delete: ");
    if (scanf("%d", &prod_id) != 1) 
	{
        printf("INVALID INPUT\n");
        while (getchar() != '\n');
        return 0;
    }

    // on cherche le produit et on regarde dans le tableau pour enlever le produit
    for (int i = 0; i < product_count; i++) 
	{
        if (products[i].id == prod_id && products[i].category_index == category_index) 
		{
            for (int j = i; j < product_count - 1; j++) 
			{
                products[j] = products[j + 1];
            }
            product_count--;
            save_products();
            printf("Product deleted with success\n");
            return 1;
        }
    }
    printf("product not found in this category\n");
    return 0;
}

// ici on modifie un produit dans une categorie,  le nom ou le prix et autres la
int modify_product(int category_index) 
{
    if (get_product_count(category_index) == 0) 
	{
        printf("There are no product to modify in this category\n");
        return 0;
    }

    list_products(category_index);
    int prod_id;
    printf("Type product ID to modify: ");
    if (scanf("%d", &prod_id) != 1) 
	{
        printf("you enter an INVALID INPUT\n");
        while (getchar() != '\n');
        return 0;
    }

    // on cherche le produit et on modifie ce que on veut
    for (int i = 0; i < product_count; i++) 
	{
        if (products[i].id == prod_id && products[i].category_index == category_index) 
		{
            printf("Modifying product \t'%s\t'\n", products[i].name);

            // on peut changer le nom ici
            printf("Type new name (or press Enter to keep old: \t'%s\t'): ", products[i].name);
            char new_name[MAX_PRODUCT_NAME_LEN];
            getchar(); 
            fgets(new_name, MAX_PRODUCT_NAME_LEN, stdin);
            new_name[strcspn(new_name, "\n")] = 0; 
            if (strlen(new_name) > 0) 
			{
                strcpy(products[i].name, new_name);
            }

            // on peut changer le prix aussi si on veut
            printf("Type new price (or -1 to keep old: %.2f): ", products[i].unit_price);
            float new_price;
            if (scanf("%f", &new_price) == 1 && new_price >= 0) 
			{
                products[i].unit_price = new_price;
            } 
			else if (new_price != -1) // si le user n'entre pas -1 et que l'entree est invalide ca affiche une erreur 
			{
                printf("Invalid price input, price not changed.\n");
                while (getchar() != '\n');
            } else {
                 while (getchar() != '\n'); 
            }

            save_products();
            printf("Product modified\n");// au cas contraire ca passe creme
            return 1;
        }
    }
    printf("This product has not be found in this category\n");
    return 0;
}

// ici on ajoute une transaction de stock (entree ou sortie), ajouter ou  retirer du stock
int add_stock_transaction(Product *product, int quantity, TransactionType type, Date date) 
{
    if (product->transaction_count >= MAX_TRANSACTIONS) 
	{
        printf("Too many transactions for this product\n");// ya plu de place ici brother
        return 0;
    }
    if (quantity <= 0) 
	{
        printf("Quantity must be positive\n");// on ne veut pas de somme negative, ahh toi aussi
        return 0;
    }
    // verifier si la quantite de sortie ne depasse pas le stock actuel
    if (type == EXIT) {
        int current_stock = 0;
        for (int i = 0; i < product->transaction_count; i++) {
            if (product->transactions[i].type == ENTRY) {
                current_stock += product->transactions[i].quantity;
            } else {
                current_stock -= product->transactions[i].quantity;
            }
        }
        if (quantity > current_stock) {
            printf("Not enough stock for this exit. Current stock: %d\n", current_stock);
            return 0;
        }
    }

    StockTransaction t;
    t.date = date;
    t.quantity = quantity;
    t.type = type;

    product->transactions[product->transaction_count++] = t;
    printf("transaction added\n");
    save_products(); // sauvegarder les produits apres chaque transaction sinon on va perdre nos truc dh
    // on n'oubli pas pour  historique global aussi
    add_global_transaction(product->id, product->category_index, product->name, quantity, type, date, current_user);
    return 1;
}

// ici on liste toutes les transactions de stock d'un produit,on affoche tout l'historique
void list_stock_transactions(Product *product) 
{
    if (product->transaction_count == 0) 
	{
        printf("no transactions for this product\n");
        return;
    }
    printf("+------------+----------+----------+\n");
    printf("| Date       | Quantity | Type     |\n");
    printf("+------------+----------+----------+\n");
    for (int i = 0; i < product->transaction_count; i++) 
	{
        StockTransaction *t = &product->transactions[i];
        printf("| %02d/%02d/%04d | %-8d | %-8s |\n",
               t->date.day, t->date.month, t->date.year,
               t->quantity,
               (t->type == ENTRY) ? "Entry" : "Exit");
    }
    printf("+------------+----------+----------+\n");
}

// ici on cherche un produit par son ID, 
int search_product_by_id(int id) 
{
    for (int i = 0; i < product_count; i++) 
	{
        if (products[i].id == id) 
		{
            return i; //cest grace a ca on retourne l'index du produit dans le tableau
        }
    }
    return -1; // mais on retourne -1 si on  trouve pas
}

// ici on cherche un produit par son nom (exact), tu peux taper le nom du produit pour la recherche
int search_product_by_name(const char *name) 
{
    for (int i = 0; i < product_count; i++) 
	{
        if (strcmp(products[i].name, name) == 0) 
		{
            return i; // voila ca retourne l'index du produit dans le tableau
        }
    }
    return -1; // retourne -1 si non trouver aussi
}

// ici on compte combien y a de produits dans une categorie, pour savoir si cest vide ou pas
int get_product_count(int category_index) 
{
    int count = 0;
    for (int i = 0; i < product_count; i++) 
	{
        if (products[i].category_index == category_index) 
		{
            count++;
        }
    }
    return count;
}
