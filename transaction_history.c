// voila la partie ou on a ajouter les fournisseur et transaction

#include <stdio.h>
#include <string.h>
#include "transaction_history.h"
#include "category.h" 

//  on vaa utiliser un tableau pour garder l'historique des transactions dans la memoire
static GlobalTransaction global_transactions[MAX_GLOBAL_TRANSACTIONS];
static int global_transaction_count = 0;

// nom du fichier de sauvegarde de l'historique 
const char *global_transaction_file = "global_transactions.dat";

// chargons l'historique global depuis le fichier
void load_global_transactions()
{
    FILE *f = fopen(global_transaction_file, "rb");
    if (!f)
    {
        global_transaction_count = 0; // ah sans fichier ,on commence avec 0 transaction
        return;
    }
    fread(&global_transaction_count, sizeof(int), 1, f);
    fread(global_transactions, sizeof(GlobalTransaction), global_transaction_count, f);
    fclose(f);
}

// la on le save dans un file
void save_global_transactions()
{
    FILE *f = fopen(global_transaction_file, "wb");
    if (!f)
    {
        printf("ERROR saving global transactions\\n");
        return;
    }
    fwrite(&global_transaction_count, sizeof(int), 1, f);
    fwrite(global_transactions, sizeof(GlobalTransaction), global_transaction_count, f);
    fclose(f);
}

// on ajouter une transaction a l'historique 
void add_global_transaction(int product_id, int category_index, const char* product_name, int quantity, TransactionType type, Date date, const char* username)
{
    if (global_transaction_count >= MAX_GLOBAL_TRANSACTIONS)
    {
        printf("Too many global transactions, cannot add more.\\n");
        return;
    }

    GlobalTransaction gt;
    gt.product_id = product_id;
    strcpy(gt.product_name, product_name);
    gt.date = date;
    gt.type = type;
    gt.quantity = quantity;
    gt.category_index = category_index;
    strcpy(gt.username, username);

    global_transactions[global_transaction_count++] = gt;
    save_global_transactions(); // on save toujours apres chaque ajout
}

// afficher toutes les transactions sous forme de tableau
void list_all_global_transactions()
{
    if (global_transaction_count == 0)
    {
        printf("No global transactions recorded yet.\\n");
        return;
    }
    printf("+----------+-------------------------------------------------+------------+----------+----------+------------------------------+------------------------------+\\n");
    printf("|ProductID | Product Name                                     | Date       | Type     | Quantity | Category Name                | User                         |\\n");
    printf("+----------+-------------------------------------------------+------------+----------+----------+------------------------------+------------------------------+\\n");
    for (int i = 0; i < global_transaction_count; i++)
    {
        GlobalTransaction *gt = &global_transactions[i];
        const char* category_name = category_get_name(gt->category_index);
        printf("| %-6d | %-48s | %02d/%02d/%04d | %-8s | %-8d | %-28s | %-28s |\\n",
               gt->product_id,
               gt->product_name,
               gt->date.day, gt->date.month, gt->date.year,
               (gt->type == ENTRY) ? "ENTRY" : "EXIT",
               gt->quantity,
               category_name,
               gt->username);
    }
    printf("+--------+--------------------------------------------------+------------+----------+----------+------------------------------+------------------------------+\\n");
}
