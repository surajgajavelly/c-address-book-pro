# In ~/c_address_book_pro/test/CMakeLists.txt

add_executable(test_initialize test_initialize.c)

# FIX: Link against the LIBRARY, not the executable.
target_link_libraries(test_initialize PRIVATE addressbook_lib)

add_test(NAME InitializeTest COMMAND test_initialize)