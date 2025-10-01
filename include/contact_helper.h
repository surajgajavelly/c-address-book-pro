/**
 * @file contact_helper.h
 * @author Gajavelly Sai Suraj
 * @brief Helper, validation, and user-interaction functions for the Address Book.
 * @copyright Copyright (c) 2025 All rights Reserved
 */

#ifndef CONTACT_HELPER_H
#define CONTACT_HELPER_H

#include "address_book.h"

/**
 * @brief Enum to represent the specific result of a validation check.
 *
 */
typedef enum {
    VALID,              /**< Input is valid. */
    INVALID_EMPTY,      /**< Input is empty. */
    INVALID_CHARACTERS, /**< Contains invalid characters. */
    INVALID_FORMAT,     /**< Format does not match expected pattern. */
    INVALID_LENGTH,     /**< Length is outside allowed range. */
    INVALID_DUPLICATE   /**< Value already exists in the address book. */
} ValidationStatus;

/**
 * @brief Enum to represent the user's choice to try again or cancel.
 *
 */
typedef enum {
    TRY_AGAIN = 1, /**< Retry the operation. */
    CANCEL = 2     /**< Cancel the operation. */
} Choice;

// -----Function Prototypes for Contact validation----- //

/**
 * @brief Validates that a name contains only letters and is not empty.
 * @param name The name string to validate.
 * @return ValidationStatus indicating the result of the validation.
 */
ValidationStatus is_valid_name(const char *name);

/**
 * @brief Validates that a phone number contains only digits and meets length requirements.
 * @param phone The phone number string to validate.
 * @return ValidationStatus indicating the result of the validation.
 */
ValidationStatus is_valid_phone(const char *phone);

/**
 * @brief Checks if a phone number already exists in the address book.
 * @param phone The phone number string to check.
 * @param book Pointer to the AddressBook.
 * @return ValidationStatus indicating whether the phone number is a duplicate.
 */
ValidationStatus is_phone_duplicate(const char *phone, const AddressBook *book);

/**
 * @brief Validates the format of an email address.
 * @param email The email string to validate.
 * @return ValidationStatus indicating the result of the validation.
 */
ValidationStatus is_valid_email(const char *email);

/**
 * @brief Checks if an email address already exists in the address book.
 * @param email The email string to check.
 * @param book Pointer to the AddressBook.
 * @return ValidationStatus indicating whether the email is a duplicate.
 */
ValidationStatus is_email_duplicate(const char *email, const AddressBook *book);

/**
 * @brief Prints an error message based on the validation status.
 * @param status The validation status code.
 */
void print_validation_error(const ValidationStatus status);

/**
 * @brief Prompts the user to retry or cancel an operation.
 * @param attempts Pointer to the current attempt counter (incremented if retrying).
 * @return Choice indicating the user's decision.
 */
Choice handle_attempt(int *attempts);

/**
 * @brief Generates a new unique contact ID.
 * @param book Pointer to the AddressBook.
 * @return A new integer ID not currently in use.
 */
int generate_new_id(AddressBook *book);

/**
 * @brief Removes the trailing newline character from a string.
 * @param str The string to modify.
 */
void remove_newline(char *str);

/**
 * @brief Gets an integer input from the user with validation.
 * @param prompt The message to display.
 * @return The integer entered, or -1 on failure.
 */
int get_int_input(const char *prompt);

#endif // CONTACT_HELPER_H
