/**
 * @file main.c
 * @author Gajavelly Sai Suraj 
 * @brief  Entry point for the Address Book application.
 * Handles the main menu loop and delegates actions to feature functions.
 * @bug  None known
 * @copyright Copyright (c) 2025
 */
 
#include <stdio.h> 
#include "address_book.h"
#include "contact_helper.h"

typedef enum {
    CREATE = 1,
    SEARCH,
    EDIT,
    DELETE,
    LIST,
    SAVE,
    EXIT
} MenuOption;

int main(void) {
    printf("\n================================================================================\n");
    printf("||                                                                            ||\n");
    printf("||                       ADDRESS BOOK - YOUR CONTACT VAULT                    ||\n");
    printf("||                                                                            ||\n");
    printf("================================================================================\n");
    printf("Woof! Woof! Hello there, human.\n");
    printf("I am Ein, the data dog.\n");
    printf("Let's get this address book running smoothly.\n");

    AddressBook book;

    printf("\n<=========================| INITIALIZING ADDRESS BOOK |=========================>\n");
    initialize(&book); // Initialize the address book
    printf("Ein: All set! Your address book is fresh and ready for new contacts.\n");

    load_contacts_from_file(&book); // Load contacts from the file

    MenuOption menu_choice = 0;

    do {
        printf("\n<================================| MAIN MENU |==================================>\n");
        printf("  %d. Create contact\n", CREATE);
        printf("  %d. Search contact\n", SEARCH);
        printf("  %d. Edit contact\n", EDIT);
        printf("  %d. Delete contact\n", DELETE);
        printf("  %d. List all contacts\n", LIST);
        printf("  %d. Save contacts to file\n", SAVE);
        printf("  %d. Exit\n", EXIT);
        printf("--------------------------------------------------------------------------------\n");

        menu_choice = get_int_input("Ein: What would you like to do?:  ");
        
        switch (menu_choice) {
            case CREATE:
                create_contact(&book);
                break;
            case SEARCH:
                search_contact(&book);
                break;
            case EDIT:
                edit_contact(&book);
                break;
            case DELETE:
                delete_contact(&book);
                break;
            case LIST:         
                list_contacts(&book);
                break;
            case SAVE:
                printf("\nEin: Just finished storing everything securely. Woof!\n");
                save_contacts_to_file(&book);
                break;
            case EXIT:
                printf("\n<================================| EXIT |======================================>\n");
                printf("Ein: Woof! Woof! Woof! Woof!\n");
                printf("\nEin: Goodbye for now... but if you forget me, I'll chew your cables.");
                printf("\n================================================================================\n");
                break;
            default: 
                printf("\nEin: Hmm, that doesn't compute. Pick a number from the menu.\n");
        }
    } while (menu_choice != EXIT);
    
    free_address_book(&book); // Free the memory allocated for the address book.

    return 0;
}  