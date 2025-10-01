/**
 * @file contact_helper.c
 * @author Gajavelly Sai Suraj
 * @brief Implementation of helper, validation, and user-interaction functions for the Address Book application.
 * @copyright Copyright (c) 2025 All rights Reserved
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "address_book.h"
#include "contact_helper.h"

// ========================= Utility Functions  ========================= //

/**
 * @brief Removes the newline character from a string read by fgets.
 * @param str The string to remove the newline character from.
 */
void remove_newline(char *str) 
{
    str[strcspn(str, "\n")] = '\0';
}

/**
 * @brief Generates and returns the next available unique ID.
 */
int generate_new_id(AddressBook *book) {
    return book->next_id++;
}

// ========================= Validation Functions ========================= //

/**
 * @brief Validates that a name contains only letters and spaces.
 * @param name Pointer to the null-terminated string to validate.
 * @return ValidationStatus `VALID` on success, or a specific error code on failure.
 */
ValidationStatus is_valid_name(const char *name)
{
    if(strlen(name) == 0) {
        return INVALID_EMPTY;
    }

    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && !isspace(name[i])) {
            return INVALID_CHARACTERS;
        }
    } 

    return VALID;
}

/**
 * @brief Validates that a phone number is exactly 10 digits.
 * @param phone Pointer to the null-terminated phone number string to validate.
 * @return ValidationStatus `VALID` on success, or a specific error code on failure.
 */
ValidationStatus is_valid_phone(const char *phone)
{
    if(strlen(phone) == 0) {
        return INVALID_EMPTY;
    }

    if(strlen(phone) != 10) {
        return INVALID_LENGTH;
    }

    for (int i = 0; i < 10; i++) {
        if (!isdigit(phone[i])) {
            return INVALID_CHARACTERS;
        }
    }
    return VALID;
}

/**
 * @brief Checks if a phone number already exists in the address book.
 * @param phone Pointer to the phone number string to check.
 * @param book Pointer to the AddressBook to search.
 * @return ValidationStatus `VALID` if the phone is unique, `INVALID_DUPLICATE` otherwise.
 */
ValidationStatus is_phone_duplicate(const char *phone, const AddressBook *book)
{
    const Contact *current = book->head;

    while(current != NULL) {
        if(strcmp(current->phone, phone) == 0) {
            return INVALID_DUPLICATE;
        }
        current = current->next;
    }
    return VALID;
}

/**
 * @brief Validates the format of an email address.
 * @param email Pointer to the null-terminated email string to validate.
 * @return ValidationStatus `VALID` on success, or a specific error code on failure.
 */
ValidationStatus is_valid_email(const char *email)
{
    if(strlen(email) == 0) {
        return INVALID_EMPTY;
    }

    for (int i = 0; email[i] != '\0'; i++) {
        if (isupper(email[i])) {
            return INVALID_FORMAT; 
        }
    }

    const char *at = strchr(email, '@');
    const char *dot = strrchr(email,'.');

    if (!at || !dot || dot < at) {
        return INVALID_FORMAT;
    }

    if(at == email || !isalnum(*(at - 1))) {
        return INVALID_FORMAT;
    }

    if(dot == email || !isalnum(*(dot - 1))) {
        return INVALID_FORMAT;
    }

    return VALID;
}

/**
 * @brief Checks if an email is already in the address book
 * @param email Pointer to the email string to check.
 * @param book Pointer to the AddressBook to search.
 * @return ValidationStatus `VALID` if the email is unique, `INVALID_DUPLICATE` otherwise.
 */
ValidationStatus is_email_duplicate(const char *email, const AddressBook *book)
{
    const Contact *current = book->head;

    while(current != NULL) {
        if(strcmp(current->email, email) == 0) {
            return INVALID_DUPLICATE;
        }
        current = current->next;
    }
    return VALID;
}

// ========================= User Interaction ========================= //

/**
 * @brief Prints an error message based on the validation status.
 * @param status The ValidationStatus code to report.
 */
void print_validation_error(const ValidationStatus status)
{
    printf("\nEin: *Barks.* "); // All messages are prefaced by Ein's bark
    switch(status) {
        case INVALID_EMPTY:
            printf("Hmm, data is missing. A name is needed!\n");
            break;
        case INVALID_CHARACTERS:
            printf("Woof! Only alphabetical characters, please. This cannot handle numbers and symbols in names.\n");
            break;
        case INVALID_LENGTH:
            printf("*Whines softly* That phone number isn't the right length - it should be exactly 10 digits and no characters.\n");
            break;
        case INVALID_FORMAT:
            printf("*Barks once* That email doesn't look right. Let's try a proper format like name@example.com.\n");
            break;
        case INVALID_DUPLICATE:
            printf("*Perks ears* I already have that one in my book — no duplicates allowed.\n");
            break; 
        default:
            printf("*Scratches ear* Something unexpected happened. Let's try again.\n");
            break;
    }
}

/**
 * @brief A simple and safe function to get an integer from the user.
 * NOTE: This is a simplified version. A GitHub issue can be raised to make it
 * more robust against mixed input like "42abc".
 */
int get_int_input(const char* prompt) {
    int value;
    printf("%s", prompt);

    if (scanf("%d", &value) != 1) {
        printf("\nEin: *Tilts head* That doesn't look like a number to me.\n");
        clearerr(stdin);
        value = -1; // Set to a known error code.
    }

    // This loop is CRITICAL. It cleans up everything left in the input buffer
    // after scanf, including the newline character or any junk text.
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    return value;
}

/**
 * @brief Prompts the user to retry or cancel an operation.
 * @param attempts A pointer to the number of attempts made.
 * @return Choice The user's choice: TRY_AGAIN or CANCEL.
 */
Choice handle_attempt(int *attempts) {
    (*attempts)++;

    printf("\n[ ATTEMPT %d of %d ]\n", *attempts, MAX_ATTEMPTS);
    printf("-----------------------------------------\n");

    if (*attempts >= MAX_ATTEMPTS) {
    printf("Ein: *Panting* I've sniffed every corner... no luck. Let's head back.\n");
    return CANCEL;
    }

    int retry_choice;

    do {
        printf("1. Try again\n2. Cancel\n");
        retry_choice = get_int_input("Choose: ");

        if(retry_choice == TRY_AGAIN) {
            printf("\nEin: *Wags tail* Okay, let's give it another go!\n");
            return TRY_AGAIN;
        }
        else if(retry_choice == CANCEL) {
            printf("\nEin: *Lies down* Alright, we'll leave this one for now.\n");
            return CANCEL;
        }
        else {
            printf("\nEin: *Tilts head* That's not one of the options. Don't make me chase my tail.\n");
            printf("--------------------------------------------------------------------------------\n");
        }
    } while (retry_choice != TRY_AGAIN && retry_choice != CANCEL);

    return CANCEL;

}
