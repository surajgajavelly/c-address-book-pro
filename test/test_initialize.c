// In test/test_initialize.c
#include <stdio.h>
#include <assert.h> // The standard C library for making test assertions
#include "../include/address_book.h"

// A unit test is a simple C program that returns 0 on success and 1 on failure.
int main() {
    printf("--> Running test: test_initialize...\n");

    // 1. ARRANGE: Set up the variable we want to test.
    AddressBook book;
    // Intentionally set members to "garbage" values to PROVE our function works.
    book.head = (void*)0xDEADBEEF; // A non-NULL garbage address
    book.contact_count = 123;
    book.next_id = 456;

    // 2. ACT: Run the function we are testing.
    initialize(&book);

    // 3. ASSERT: Check if the results are exactly what we expect.
    // assert() will crash the program if the condition is false.
    // In the context of CTest, a crash means the test has FAILED.
    assert(book.head == NULL);
    assert(book.contact_count == 0);
    assert(book.next_id == 1);

    printf("    [PASS] All checks passed for initialize().\n");
    return 0; // Return 0 to signal SUCCESS to CTest
}