/**
 * @file address_book.c
 * @author Gajavelly Sai Suraj (you@domain.com)
 * @brief Implementation of the core functions for managing the address book.
 * @copyright Copyright (c) 2025 All rights Reserved
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "address_book.h"
#include "contact_helper.h"

/**
 * @brief Initializes an AddressBook to a safe, empty state.
 * @param book A pointer to the AddressBook struct to be initialized.
 */
void initialize(AddressBook *book) 
{

    if (book == NULL) {
        printf("Ein: *Ears perk, then droop* Hmm… I can't seem to find the address book to set up.\n");
        return;
    }

    book->head = NULL;
    book->contact_count = 0;
    book->next_id = 1;
}

/**
 * @brief Frees the memory allocated for the address book.
 * 
 * @param book A pointer to the AddressBook struct.
 */
void free_address_book(AddressBook *book) 
{

    // Defensive check: ensure the AddressBook pointer is valid
    if (book == NULL) {
        printf("Ein: *Tilts head* I can't clean up what isn't here.\n");
        return;
    }

    // Check if the address book is already empty
    if (book->head == NULL) {
        return;
    }

    // Start at the beginning of the list
    Contact* current = book->head;
    Contact *temp = NULL;

    // Iterate through the linked list, freeing each contact node
    while (current != NULL) {
        temp = current->next;
        free(current);
        current = temp;
    }

    // Finally, reset the address book struct to its initial, safe state.
    book->head = NULL;
    book->contact_count = 0;
    book->next_id = 1;

}

/**
 * @brief Creates a new contact by prompting the user for details, validating the input,
 * and adding the contact to the address book's linked list.
 * @param book A pointer to the AddressBook struct where the new contact will be stored.
 */
void create_contact(AddressBook* book) {

    printf("\n<==============================| CREATE CONTACT |==============================>\n");
    //printf("\nEin: *Barks sadly.* The address book is full! Let's delete some old contacts to make space.\n");
    Contact *new_contact = (Contact *)malloc(sizeof(Contact));
    if(new_contact == NULL)
    {
        printf("Ein: *tilts head* Hmm... I couldn't fetch enough memory to store a new friend.\n");
        return;
    }
    new_contact->next = NULL;

    int attempts;
    ValidationStatus status;

    // --- Name Validation --- //
    attempts = 0;
    //printf("\nEin: Okay, let's make a new friend! What should we name them?\n");
    printf("\nEin: *Perks up ears* Oh! A new friend? Let's start with their name.\n");
    do {
        printf("Enter Name: ");
        fgets(new_contact->name, MAX_NAME_LENGTH, stdin);
        remove_newline(new_contact->name);

        status = is_valid_name(new_contact->name);

        if(status == VALID) {
            printf("Ein: Got it! I will remember %s forever or at least until you delete them.\n", new_contact->name);
            break;
        }

        print_validation_error(status);
        // printf("Ein: Let's try that name again. It needs to be a little better, don't you think?\n");

        if(handle_attempt(&attempts) == CANCEL) {
            free(new_contact);
            return;
        }

    } while(attempts < MAX_ATTEMPTS);

    if(attempts >= MAX_ATTEMPTS) {
        free(new_contact);
        return;
    }

    // --- Phone Validation --- //
    attempts = 0; // Reset attempts
    do
    {
        printf("\nEin: I've got my paws ready to dial!\n");
        printf("What's their phone number? : ");
        fgets(new_contact->phone, MAX_PHONE_LENGTH, stdin);
        remove_newline(new_contact->phone);

        status = is_valid_phone(new_contact->phone);

        if(status == VALID) {
            status = is_phone_duplicate(new_contact->phone, book);
        }

        if(status == VALID) {
            printf("Ein: Perfect! I can already imagine calling %s.\n", new_contact->phone);
            break;
        }

        print_validation_error(status);
        if(handle_attempt(&attempts) == CANCEL) {
            free(new_contact);
            return;
        }

    } while (attempts < MAX_ATTEMPTS);

    if(attempts >= MAX_ATTEMPTS) {
        free(new_contact);
        return;
    }
    
     // --- Email Validation --- //
    attempts = 0; // Reset attempts
    do
    {
        printf("\nEin: Got any treats, or maybe an email address?\n");
        printf("What's their email? : ");
        fgets(new_contact->email, MAX_EMAIL_LENGTH, stdin);
        remove_newline(new_contact->email);

        status = is_valid_email(new_contact->email);

        if(status == VALID) {
            status = is_email_duplicate(new_contact->email, book);
        }

        if(status == VALID) {

            break;
        }

        print_validation_error(status);

        if(handle_attempt(&attempts) == CANCEL) {
            free(new_contact);
            return;
        }

    } while (attempts < MAX_ATTEMPTS);

    if(attempts >= MAX_ATTEMPTS) {
        free(new_contact);
        return;
    }

    // --- ID Generation --- //
    new_contact->id = generate_new_id(book);

    // --- Add Contact to Linked List --- //
    if(book->head == NULL) {
        book->head = new_contact;
    }
    else {
        Contact *current = book->head;
        while(current->next != NULL) {
            current = current->next;
        }
        current->next = new_contact;
    }
    book->contact_count++; // Increment contact count.

    printf("\nEin: *Tail wags furiously* Yay! Found a new friend! %s is in the book. Woof!\n", new_contact->name);

}

/**
 * @brief Searches for contacts in the address book based on user-specified criteria.
 * @param book A pointer to the AddressBook struct.
 * @return A pointer to the selected Contact node, or NULL if not found or cancelled.
 */
Contact* search_contact(AddressBook *book) {
    
    printf("\n<===============================| SEARCH CONTACT |===============================>\n");
    printf("Ein: Time to put my nose to work! Let's see who we can find.\n");

    if (book->head == NULL) {
        printf("\nEin: *Ears droop* Looks like your address book is empty. Nothing to sniff out yet!\n");
        return NULL;
    }

    // The maximum possible matches is the total number of contacts.
    // We allocate an array of POINTERS on the heap.
    Contact** matched_nodes = (Contact**)malloc(sizeof(Contact*) * book->contact_count);
    if (matched_nodes == NULL) {
        printf("Ein: *Whines* I couldn't fetch the search results right now.\n");
        return NULL;
    }
    
    int attempts = 0;
    char search_query[MAX_NAME_LENGTH];
    int search_choice;

    do {
        printf("\n-------------------- SEARCH OPTIONS --------------------\n");
        printf("  %d) Search by Name\n",  SEARCH_BY_NAME);
        printf("  %d) Search by Phone\n", SEARCH_BY_PHONE);
        printf("  %d) Search by Email\n", SEARCH_BY_EMAIL);
        printf("  %d) Cancel\n",         SEARCH_CANCEL);
        printf("---------------------------------------------------------\n");

        search_choice = get_int_input("Ein: How would you like to search? ");

        if (search_choice == -1) {
            printf("\nEin: That didn't look like a valid choice.\n");
            if (handle_attempt(&attempts) == CANCEL) {
                free(matched_nodes);
                return NULL;
            }
            continue;
        }

        if(search_choice == SEARCH_CANCEL) {
            printf("Ein: Alright, search cancelled. Back to the main menu.\n");
            free(matched_nodes);
            return NULL;
        }

        if(search_choice >= SEARCH_BY_NAME && search_choice <= SEARCH_BY_EMAIL) {
            switch(search_choice) {
                case SEARCH_BY_NAME:
                    printf("Ein: Whose name should I sniff out for you?: ");
                    break;
                case SEARCH_BY_PHONE:
                    printf("Ein: What phone number should I look up?: ");
                    break;
                case SEARCH_BY_EMAIL:
                    printf("Ein: What email address should I hunt for?: ");
                    break;
            }
            fgets(search_query, MAX_NAME_LENGTH, stdin);
            remove_newline(search_query);
        }
        else {
            printf("Ein: That's not one of the options. Let's try again.\n");
            if(handle_attempt(&attempts) == CANCEL) {
                free(matched_nodes);
                return NULL;
            }
            continue;  // Skip to next iteration if the choice is invalid
        }

        
        int matched_count = 0;
        Contact *current = book->head;

        while (current != NULL) {
            if ((search_choice == SEARCH_BY_NAME && strcmp(search_query, current->name) == 0) ||
                (search_choice == SEARCH_BY_PHONE && strcmp(search_query, current->phone) == 0) ||
                (search_choice == SEARCH_BY_EMAIL && strcmp(search_query, current->email) == 0)) {
                matched_nodes[matched_count++] = current;
            }
            current = current->next;
        }

        if (matched_count == 0) {
            printf("Ein: *Sniffs around* Nope, I couldn't find anyone matching \"%s\".\n", search_query);
            if(handle_attempt(&attempts) == CANCEL) {
                free(matched_nodes);
                return NULL;
            }
            continue;
        }
        else if (matched_count == 1) {
            printf("\nEin: Found them! Here's what I've got:\n");
            printf("--------------------------------\n");
            printf("ID: %d\n", matched_nodes[0]->id);
            printf("Name: %s\n", matched_nodes[0]->name);
            printf("Phone: %s\n", matched_nodes[0]->phone);
            printf("Email: %s\n", matched_nodes[0]->email);
            printf("\n");
            Contact *result = matched_nodes[0];
            free(matched_nodes);
            return result;
        }
        else {
            printf("\nEin: I found %d matches. Take a look:\n", matched_count);

            printf("--------------------------------------------------------------------------------\n");
            printf(" No. | ID   | %-20s | %-15s | %-30s\n", "Name", "Phone", "Email");
            printf("--------------------------------------------------------------------------------\n");

            for (int i = 0; i < matched_count; i++) {
                printf(" %-3d | %-4d | %-20s | %-15s | %-30s\n",
                      i + 1,
                      matched_nodes[i]->id,
                      matched_nodes[i]->name,
                      matched_nodes[i]->phone,
                      matched_nodes[i]->email);
            }

            printf("--------------------------------------------------------------------------------\n");

            int selection = get_int_input("Ein: Which one should I fetch for you?: ");\

            if (selection < 1 || selection > matched_count) {
                printf("Ein: *Tilts head* That's not a valid choice. Let's fetch again.\n");
                if(handle_attempt(&attempts) == CANCEL) {
                    free(matched_nodes);
                    return NULL;
                }
                continue;
            }

            Contact *selected = matched_nodes[selection - 1];
            printf("\nEin: Got it! Fetching the details for you now:\n\n");
            printf("Name  : %s\n",  selected->name);
            printf("Phone : %s\n",  selected->phone);
            printf("Email : %s\n\n", selected->email);
            printf("\n");
            free(matched_nodes);
            return selected;
        }

        
    } while (attempts < MAX_ATTEMPTS);

    printf("Ein: I've tried my best, but we've reached the limit. Back to the menu.\n");
    free(matched_nodes);
    return NULL;
    
}

/**
 * @brief Allows the user to edit the details of a specific contact.
 * @param book A pointer to the AddressBook struct.
 */
void edit_contact(AddressBook *book) {

    printf("\n<===============================| EDIT CONTACT |===============================>\n");
    printf("Ein: Let's make some updates - tell me what needs changing.\n");

    if(book->head == NULL) {
        printf("Ein: *Ears droop* There's nothing to edit - your address book is empty.\n");
        return;
    }

    Contact *target = search_contact(book);
    if(target == NULL) {
        printf("\nEin: Couldn't find that contact. Let's head back to the main menu.\n");
        return;
    }

    EditOption edit_choice;
    int attempts;
    ValidationStatus status;
    bool has_changes = false;

    Contact temp_contact = *target;

    do {
        printf("\n<================== Edit Menu ====================>\n");
        printf("  %d. Edit Name\n", EDIT_NAME);
        printf("  %d. Edit Phone\n", EDIT_PHONE);
        printf("  %d. Edit Email\n", EDIT_EMAIL);
        printf("  %d. Save Changes\n", EDIT_SAVE);
        printf("  %d. Cancel Edit\n", EDIT_CANCEL);
        printf("----------------------------------------------------\n");

        edit_choice = get_int_input("Ein: What would you like to change? ");

        switch(edit_choice) {
            case EDIT_NAME:
            attempts = 0;
            printf("Ein: Let's update their name.\n");
            do
            {
                printf("Enter new name: ");
                fgets(temp_contact.name, MAX_NAME_LENGTH, stdin);
                remove_newline(temp_contact.name);

                status = is_valid_name(temp_contact.name);

                if(status == VALID) {
                    has_changes = true;
                    printf("Ein: Name updated.\n");
                    break;
                }
                else {
                    print_validation_error(status);
                    printf("Ein: That doesn't look right. Let's try again.\n");
                    if(handle_attempt(&attempts) == CANCEL) {
                        return;
                    }
                }
            } while (attempts < MAX_ATTEMPTS);
            break;

            case EDIT_PHONE:
            attempts = 0;
            printf("Ein: Let's update their phone number.\n");
            do {
                printf("Enter new phone number: ");
                fgets(temp_contact.phone, MAX_PHONE_LENGTH, stdin);
                remove_newline(temp_contact.phone);

                status = is_valid_phone(temp_contact.phone);

                if(status == VALID) {
                    status = is_phone_duplicate(temp_contact.phone, book);
                }
                if(status == VALID) {
                    has_changes = true;
                    printf("Ein: Phone number updated.\n");
                    break;
                }
                else {
                    print_validation_error(status);
                    printf("Ein: That number doesn't seem right. Try again.\n");
                    if(handle_attempt(&attempts) == CANCEL) {
                        return;
                    }
                }
            } while (attempts < MAX_ATTEMPTS);
            break;

            case EDIT_EMAIL:
            attempts = 0;
            printf("Ein: Let's update their email address.\n");
            do {
                printf("Enter new email: ");
                fgets(temp_contact.email, MAX_EMAIL_LENGTH, stdin);
                remove_newline(temp_contact.email);

                status = is_valid_email(temp_contact.email);

                if(status == VALID) {
                    status = is_email_duplicate(temp_contact.email, book);
                }
                if(status == VALID) {
                    has_changes = true;
                    printf("Ein: Email updated.\n");
                    break;
                }
                else {
                    print_validation_error(status);
                    printf("Ein: That email doesn't seem right. Try again.\n");
                    if(handle_attempt(&attempts) == CANCEL) {
                        return;
                    }
                }
            } while (attempts < MAX_ATTEMPTS);
            break;

            case EDIT_SAVE:
            if (has_changes) {
                    // On "Save", copy the temporary data back to the original contact.
                    strcpy(target->name, temp_contact.name);
                    strcpy(target->phone, temp_contact.phone);
                    strcpy(target->email, temp_contact.email);
                    printf("\nEin: All set! I've updated the details and tucked them safely back into the address book.\n");
                } 
                else {
                    printf("\nEin: Looks like nothing changed after all.\n");
                    printf("Ein: I'll leave everything just the way it was.\n");
                }
                return;

            case EDIT_CANCEL:
            printf("\nEin: Edit cancelled - no changes made.\n");
            printf("Ein: Everything stays exactly as you left it.\n");
            return;

            default:
            printf("\nEin: *Tilts head.* That's not a valid choice. Try again.\n");
            break;
        }

        printf("\nEin: Here's what I've got:\n");
        printf("-----------------------------------------------------\n");
        printf("ID    : %d\n", temp_contact.id);
        printf("Name  : %s\n", temp_contact.name);
        printf("Phone : %s\n", temp_contact.phone);
        printf("Email : %s\n", temp_contact.email);
 
    } while (edit_choice != EDIT_SAVE);
} 

/**
 * @brief Searches for and deletes a contact from the address book after user confirmation.
 * @param book A pointer to the AddressBook struct to be modified.
 */
void delete_contact(AddressBook *book) {

    printf("\n<===============================| DELETE CONTACT |===============================>\n");

    if(book->contact_count == 0 || book->head == NULL)
    {
        printf("\nEin: *Whines.* The address book is empty. Nothing to delete!\n");
        return;
    }

    Contact *target= search_contact(book);
    if(target == NULL)
    {
        printf("\nEin: *Tilts head* Couldn't find anyone to remove. Let's head back.\n");
        return;
    }

    printf("\nEin: Just to be sure, is this the contact you want me to erase?\n");
    printf("--------------------------------------------------------------\n");
    printf("Name: %s\n", target->name);
    printf("Phone: %s\n", target->phone);
    printf("Email: %s\n", target->email); 

    char delete_confirm;
    int attempts = 0;

    do {
        printf("\nEin: Are you sure you want me to erase this one from the book? (y/n): ");
        if (scanf(" %c", &delete_confirm) != 1) {
            while (getchar() != '\n'); // clear buffer
            printf("Ein: Hmm... I didn't quite catch that. Please type 'y' or 'n'.\n");
            if (handle_attempt(&attempts) == CANCEL)
            {
                return;
            }
            continue;
        }
        getchar(); // consume newline

        if (delete_confirm == 'y' || delete_confirm == 'Y') 
        {
            Contact *current = book->head;
            Contact *prev = NULL;

            while (current != NULL) {
                if (current == target) {
                    if (prev == NULL) {
                        book->head = current->next;
                    }
                    else {
                        prev->next = current->next;
                    }
                    free(current);
                    book->contact_count--;
                    printf("\nEin: *Wags tail slowly* Alright, they're gone.\n");
                    printf("Ein: I've cleaned up the record and your address book is nice and tidy now.\n");
                    return;
                }
                prev = current;
                current = current->next;
            }
        }
        else if (delete_confirm == 'n' || delete_confirm == 'N') {
            printf("\nEin: *Happy bark* Okay! I'll keep them right where they are.\n");
            printf("Ein: No changes made, your pack stays the same.\n");
            return;
        }
        else {
            printf("Ein: That's not a valid choice. Please type 'y' or 'n'.\n");
            if (handle_attempt(&attempts) == CANCEL) {
                return;
            }
        }

    } while (attempts < MAX_ATTEMPTS);

    printf("\nEin: We've tried enough times. I'll leave everything as it is.\n");
}

/**
 * @brief Traverses the linked list and prints a formatted list of all contacts.
 * @param book A const pointer to the AddressBook struct (read-only operation).
 */
void list_contacts(const AddressBook *book) {

    printf("\n<=============================| CONTACT LIST |==================================>\n");

    if (book->head == NULL) {
        printf("\n-------------------------------------------------------------------------------\n");
        printf("| %-60s |\n", "Ein: *Whines softly.* There's nothing here yet, your address book is empty!");
        printf("-------------------------------------------------------------------------------\n");
        return;
    }

    printf("Ein: Here's everyone I've got stored safely in your address book:\n");
    printf("-----------------------------------------------------------------------------\n");
    printf("| %-4s | %-20s | %-15s | %-25s |\n", "ID", "Name", "Phone", "Email");
    printf("-----------------------------------------------------------------------------\n");

    const Contact *current = book->head;

    while (current != NULL) {
        printf("| %-4d | %-20s | %-15s | %-25s |\n",
               current->id,
               current->name,
               current->phone,
               current->email);

        current = current->next; // Move to the next node in the chain
    }

    printf("-----------------------------------------------------------------------------\n");
    printf("| Total contacts: %-57d |\n", book->contact_count);
    printf("-----------------------------------------------------------------------------\n");
    printf("Ein: That's the full pack for now. All safe and sound.\n");
}


/**
 * @brief Saves the entire address book to a simple CSV file.
 * @param book A const pointer to the AddressBook to be saved (read-only).
 */
void save_contacts_to_file(const AddressBook *book) {

    printf("\n<==========================| SAVE CONTACTS TO FILE |==========================>\n");

    FILE *fptr = fopen("../contacts.csv", "w");
    if(fptr == NULL) {
        printf("Ein: *Whines softly* I couldn't open the file to save your contacts.\n");
        printf("Ein: Let's check the file location and try again later.\n");
        return;
    }

    fprintf(fptr, "%d\n", book->contact_count);

    const Contact *current = book->head;

    while(current != NULL) {
        fprintf(fptr, "%d,%s,%s,%s\n",
                current->id,
                current->name,
                current->phone,
                current->email);
        current = current->next;
    }

    fclose(fptr);

    printf("Ein: All contacts have been safely stored in my data vault.\n");
    printf("--------------------------------------------------\n");
    printf("| %-46s |\n", "Save complete!");
    printf("| Total contacts saved: %-24d |\n", book->contact_count);
    printf("--------------------------------------------------\n");
    printf("Ein: Everything's backed up, you can relax now.\n");
}

/**
 * @brief Loads contacts from the CSV file into the address book.
 * @param book A pointer to the AddressBook to be populated.
 */
void load_contacts_from_file(AddressBook *book) {

    printf("\n<=========================| LOAD CONTACTS FROM FILE |===========================>\n\n");

    FILE *fptr = fopen("../contacts.csv", "r");
    if(fptr == NULL) {
        printf("Ein: *Sniffs around the desk* Hmm I couldn't find or open 'contacts.csv'.\n");
        printf("Ein: Maybe it's not here yet, we can create it when you save your first contact.\n");
        return;
    }

    int num_contacts;
    if (fscanf(fptr, "%d\n", &num_contacts) != 1) {
        printf("Ein: *Tilts head* I couldn't read the contact count, the file might be damaged.\n");
        fclose(fptr);
        return;
    }
    
    //initialize(book);
    Contact *tail = NULL;

    // Step 4: Read each contact line
    for (int i = 0; i < num_contacts; i++) 
    {
        Contact *new_contact = malloc(sizeof(Contact));
        if (new_contact == NULL) {
            printf("Ein: *Whines softly* I ran out of space to load more contacts.\n");
            fclose(fptr);
            return;
        }

        if (fscanf(fptr,"%d,%49[^,],%19[^,],%49[^\n]\n",
                   &new_contact->id,
                   new_contact->name,
                   new_contact->phone,
                   new_contact->email) != 4)
        {
            printf("Ein: Couldn't read contact #%d properly, skipping it.\n", i + 1);
            free(new_contact);
            continue;
        }

        new_contact->next = NULL;

        // Append to the linked list
        if (book->head == NULL) {
            book->head = new_contact;
            tail = new_contact;
        }
        else {
            tail->next = new_contact;
            tail = new_contact;
        }

        book->contact_count++;
    } 

    fclose(fptr);
    printf("Ein: Successfully fetched %d contact(s) from my storage.\n", book->contact_count);
    if (book->contact_count == 0) {
        printf("Ein: Looks like the file was empty, let's get ready to start fresh!\n");
    } else if (book->contact_count == 1) {
        printf("Ein: Just one friend in here, but it's a start!\n");
    } else {
        printf("Ein: That's quite a pack you've got there. All loaded and ready!\n");
    }
}



