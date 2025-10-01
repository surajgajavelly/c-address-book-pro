/**
 * @file address_book.h
 * @author Gajavelly Sai Suraj
 * @brief Header defining the core data structures and function prototypes for the Address Book.
 * @copyright Copyright (c) 2025 All Rights Reserved.
 */

#ifndef ADDRESS_BOOK_H
#define ADDRESS_BOOK_H

// Maximum number of attempts for input validation
#define MAX_ATTEMPTS 4

#define MAX_NAME_LENGTH 50
#define MAX_PHONE_LENGTH 20
#define MAX_EMAIL_LENGTH 50

/**
 * @brief Options for searching for a contact.
 */
typedef enum { SEARCH_BY_NAME = 1, SEARCH_BY_PHONE, SEARCH_BY_EMAIL, SEARCH_CANCEL } SearchOption;

/**
 * @brief Options for modifying a contact.
 */
typedef enum { EDIT_NAME = 1, EDIT_PHONE, EDIT_EMAIL, EDIT_SAVE, EDIT_CANCEL } EditOption;

/**
 * @brief Represents a single contact as a node in a linked list.
 */
typedef struct ContactNode {
    int id;                       /**< Unique identifier, generated automatically. */
    char name[MAX_NAME_LENGTH];   /**< Name of the contact. */
    char phone[MAX_PHONE_LENGTH]; /**< Phone number of the contact. */
    char email[MAX_EMAIL_LENGTH]; /**< Email address of the contact. */
    struct ContactNode *next;     /**< Pointer to the next node in the linked list.  */
} Contact;

/**
 * @brief Represents the entire address book.
 */
typedef struct {
    Contact *head;     /**< Pointer to the first node in the linked list. */
    int contact_count; /**< The total number of contacts currently in the address book. */
    int next_id;       /**< The next available ID for a new contact. */
} AddressBook;

// --- Menu Functions ---
/**
 * @brief Creates a new contact and adds it to the address book.
 *
 * @param book A Pointer to the AddressBook.
 */
void create_contact(AddressBook *book);

/**
 * @brief Searches for a contact.
 *
 * @param book A pointer to the AddressBook.
 * @return Pointer to the found contact, or NULL.
 */
Contact *search_contact(AddressBook *book);

/**
 * @brief Edits an existing contact.
 *
 * @param book A pointer to the AddressBook.
 */
void edit_contact(AddressBook *book);

/**
 * @brief Deletes a contact from the address book.
 *
 * @param book A pointer to the AddressBook.
 */
void delete_contact(AddressBook *book);

/**
 * @brief prints all contacts available in the address book.
 *
 * @param book A Const pointer to the AddressBook (read-only).
 */
void list_contacts(const AddressBook *book);

// --- Persistence Functions ---
/**
 * @brief Saves all contacts to a simple CSV file.
 *
 * @param book A const pointer to the AddressBook to be saved (read-only).
 */
void save_contacts_to_file(const AddressBook *book);

/**
 * @brief Loads contacts from the CSV file into the address book.
 *
 * @param book A pointer to the AddressBook.
 */
void load_contacts_from_file(AddressBook *book);

// --- Utility Functions ---
/**
 * @brief Initializes an AddressBook to a safe, empty state.
 *
 * @param book A pointer to the AddressBook.
 */
void initialize(AddressBook *book);

/**
 * @brief Frees the memory allocated for the address book.
 *
 * @param book A pointer to the AddressBook.
 */
void free_address_book(AddressBook *book);

#endif // ADDRESS_BOOK_H
