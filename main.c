#include <stdio.h>
#include <string.h>
#include "user_management.h"
#include "category.h"
#include "product.h"
#include "transaction_history.h" 
#include "supplier.h"            
         

// c est le main, le coeur du programme, on commence par la
int main()
{
    // dabord on commence par charger tout ce qui est sauvegarde
    load_users();          // on charge les users
    load_categories();     // on charge les categories
    load_products();       // on charge les produits
    load_global_transactions(); //  on charge l\'historique des transactions
    load_suppliers();      // : on charge les fournisseurs , a la fin du code main ils seront sauvegarder
    
    

    int choice; // variable que on utilise pour stocker le choix de l utilisateur

    do {
        //  partie Bienvenu et affichage du menu
        printf("\\n WELCOME TO YOUR INVENTORY MANAGEMENT SYSTEM \\n");
        printf("\\n--- Main Menu ---\\n");
        if (!is_logged_in)
        {
            // lors du nouveau contact on s'inscrip ou on se connecte
            printf("1. Register\\n");
            printf("2. Login\\n");
            printf("0. Quit\\n");
        }
        else
        {
            // si on est connecter, on affiche le pseudo et les options
            printf("logged as: %s\\n", current_user);
            printf("3. Manage categories\\n");
            printf("4. Manage Suppliers\\n");		//option pour gerer les fournisseurs
            printf("5. Show Global Transaction History\\n"); //option pour l'historique global
            printf("6. Show Top Selling Products\\n"); // option pour les produits les plus vendus
            printf("7. Logout\\n"); 
            printf("0. Quit\\n");
        }
        printf("Your choice? "); // on demande le choix au user
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                // si on est pas inscrip dabord, on lance l'inscription
                if (!is_logged_in) register_user();
                else printf("You already logged in, chill\\n");
                break;
            case 2:
                // si on est pas connecter, on lance la connexion
                if (!is_logged_in) login_user();
                else printf("You already logged in\\n");
                break;
            case 3:
                // gestion des categories, poue ca , il faut etre connecter pour ca
                if (is_logged_in)
                {
                    int cat_choice; // choix dans le menu categories
                    do {
                        // on affiche le menu des categories
                        printf("\\n--- Categories Menu ---\\n");
                        printf("1. Show categories\\n");
                        printf("2. Add category\\n");
                        printf("3. Delete category\\n");
                        printf("4. Edit category\\n");
                        printf("5. Manage products\\n");
                        printf("0. Back\\n");
                        printf("What is your choice? ");
                        scanf("%d", &cat_choice);

                        switch (cat_choice)
                        {
                            case 1:
                                // afficher toutes les categories
                                list_categories();
                                break;
                            case 2:
                                // ajouter une nouvelle categorie
                                add_category();
                                break;
                            case 3:
                                // supprimer une categorie
                                delete_category();
                                break;
                            case 4:
                                // modifier une categorie
                                modify_category();
                                break;
                            case 5:
                            {
                                // gestion des produits dans une categorie
                                int selected_cat;
                                list_categories(); // on affiche les categories
                                printf("Pick category to manage products: ");
                                scanf("%d", &selected_cat);
                                if (selected_cat < 1 || selected_cat > get_category_count())
                                {
                                    printf("Invalid category\\n");
                                    break;
                                }
                                selected_cat -= 1; // on passe a l\'indice zero

                                int prod_choice; // choix dans menu produits
                                do
                                {
                                    // menu produits pour la categorie choisie
                                    printf("\\n--- Products menu for %s ---\\n", category_get_name(selected_cat));
                                    printf("1. Show products\\n");
                                    printf("2. Add product\\n");
                                    printf("3. Delete product\\n");
                                    printf("4. Edit product\\n");
                                    printf("5. Manage stocks\\n");
                                    printf("6. Search product by ID\\n");
                                    printf("7. Search product by name\\n");
                                    printf("0. Back\\n");
                                    printf("What is your choice? ");
                                    scanf("%d", &prod_choice);

                                    switch (prod_choice)
                                    {
                                        case 1:
                                            // afficher les produits de la categorie
                                            list_products(selected_cat);
                                            break;
                                        case 2:
                                            // ajouter un produit
                                            add_product(selected_cat);
                                            break;
                                        case 3:
                                            // supprimer un produit
                                            delete_product(selected_cat);
                                            break;
                                        case 4:
                                            // modifier un produit
                                            modify_product(selected_cat);
                                            break;
                                        case 5:
                                        {
                                            // gerer les stocks d un produit
                                            int prod_id;
                                            printf("give product ID to manage stock: ");
                                            scanf("%d", &prod_id);

                                            int prod_idx = search_product_by_id(prod_id);
                                            // on verifie que le produit est bien dans la categorie
                                            if (prod_idx == -1 || products[prod_idx].category_index != selected_cat)
                                            {
                                                printf("This product has be not found in this category\\n");
                                                break;
                                            }
                                            Product *p = &products[prod_idx];

                                            int stock_choice; // choix dans menu stock
                                            do
                                            {
                                                // menu gestion stock
                                                printf("\\n--- Stocks for %s ---\\n", p->name);
                                                printf("1. Add stock entry\\n");
                                                printf("2. Add stock exit\\n");
                                                printf("3. Show transactions\\n");
                                                printf("0. Back\\n");
                                                printf("Enter your choice : ");
                                                scanf("%d", &stock_choice);

                                                switch (stock_choice)
                                                {
                                                    case 1:
                                                    {
                                                        // ajouter une entree de stock
                                                        int qty;
                                                        Date date;
                                                        printf("How many to add? ");
                                                        scanf("%d", &qty);
                                                        printf("Enter the Date (DD MM YYYY): ");
                                                        scanf("%d %d %d", &date.day, &date.month, &date.year);
                                                        add_stock_transaction(p, qty, ENTRY, date);
                                                        // enregistrer aussi dans l'historique 
                                                        add_global_transaction(p->id, p->category_index, p->name, qty, ENTRY, date, current_user);
                                                        break;
                                                    }
                                                    case 2:
                                                    {
                                                        // ajouter une sortie de stock
                                                        int qty;
                                                        Date date;
                                                        printf("How many to remove? ");
                                                        scanf("%d", &qty);
                                                        printf(" Enter the date (DD MM YYYY): ");
                                                        scanf("%d %d %d", &date.day, &date.month, &date.year);
                                                        add_stock_transaction(p, qty, EXIT, date);
                                                        // on enregistrer aussi dans l'historique 
                                                        add_global_transaction(p->id, p->category_index, p->name, qty, EXIT, date, current_user);
                                                        break;
                                                    }
                                                    case 3:
                                                        // afficher toutes les transactions
                                                        list_stock_transactions(p);
                                                        break;
                                                    case 0:
                                                        // revenir au menu produits
                                                        break;
                                                    default:
                                                        printf("You have enter an INVALID CHOICE\\n");
                                                }
                                            } while (stock_choice != 0);
                                            break;
                                        }
                                        case 6:
                                        {
                                            // recherche produit par son ID
                                            int prod_id;
                                            printf("Can you type product ID: ");
                                            scanf("%d", &prod_id);
                                            int prod_idx = search_product_by_id(prod_id);
                                            if (prod_idx == -1 || products[prod_idx].category_index != selected_cat)
                                            {
                                                printf("This product has not be found here\\n");
                                            }
                                            else
                                            {
                                                Product *p = &products[prod_idx];
                                                printf("Product found:\\nID: %d\\nName: %s\\nPrice: %.2f\\n", p->id, p->name, p->unit_price);
                                                list_stock_transactions(p);
                                            }
                                            break;
                                        }
                                        case 7:
                                        {
                                            // recherche produit par son nom
                                            char name[MAX_PRODUCT_NAME_LEN];
                                            printf("Type product name: ");
                                            getchar();
                                            fgets(name, MAX_PRODUCT_NAME_LEN, stdin);
                                            name[strcspn(name, "\\n")] = 0;

                                            int prod_idx = search_product_by_name(name);
                                            if (prod_idx == -1 || products[prod_idx].category_index != selected_cat) 
											{
                                                printf("This product has not be found here\\n");
                                            }
                                            else
                                            {
                                                Product *p = &products[prod_idx];
                                                printf("product found:\\nID: %d\\nName: %s\\nPrice: %.2f\\n", p->id, p->name, p->unit_price);
                                                list_stock_transactions(p);
                                            }
                                            break;
                                        }
                                        case 0:
                                            // on vous renvois  au menu  des categories
                                            break;
                                        default:
                                            printf("INVALID CHOICE\\n");
                                    }
                                }
                                while (prod_choice != 0);
                                break;
                            }
                            case 0:
                                // on  repart au menu principal
                                break;
                            default:
                                printf("you have enter an INVALID CHOICE\\n");
                        }
                    }
                    while (cat_choice != 0);
                }
                else
                {
                    // si pas connecter on dit de se connecter dabord
                    printf("You need to login first\\n");
                }
                break;
            case 4: // partie pour gerer  les cas  les fournisseurs
                if (is_logged_in) {
                    int sup_choice;
                    do {
                        printf("\\n--- Suppliers Menu ---\\n");
                        printf("1. Show Suppliers\\n");
                        printf("2. Add Supplier\\n");
                        printf("3. Delete Supplier\\n");
                        printf("4. Modify Supplier\\n");
                        printf("0. Back\\n");
                        printf("What is your choice? ");
                        scanf("%d", &sup_choice);

                        switch (sup_choice) 
						{
                            case 1:
                                list_suppliers();
                                break;
                            case 2:
                                add_supplier();
                                break;
                            case 3:
                                delete_supplier();
                                break;
                            case 4:
                                modify_supplier();
                                break;
                            case 0:
                                break;
                            default:
                                printf("INVALID CHOICE\\n");
                        }
                    } while (sup_choice != 0);
                } else {
                    printf("You need to login first\\n");
                }
                break;
            case 5: // partie pour afficher l'historique global des transactions
                if (is_logged_in) 
				{
                    list_all_global_transactions();
                } 
				else 
				{
                    printf("You need to login first\\n");
                }
                break;
            case 6: // option pour les produits les plus vendus (a implementer), on va montrer je l'espere celui le plus vendu
                if (is_logged_in) {
                    printf("Top Selling Products feature coming soon!\\n");
                } else {
                    printf("You need to login first\\n");
                }
                break;
            case 7:
                // deconnexion
                if (is_logged_in) logout_user();
                else printf("You are not logged in\\n");
                break;
            case 0:
                // quitter le programme, on sauvegarde tout avant de partir; chaque donne vas etre sauvegarder 
                printf("Exiting... Saving data...\\n");
                save_users();
                save_categories();
                save_products();
                save_global_transactions(); 
                save_suppliers(); 
                break;
            default:
                printf("INVALID CHOICE\\n");
        }
    } while (choice != 0);

    return 0;
}
