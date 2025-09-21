# In ~/c_address_book_pro/CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
project(AddressBook C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 1. Find all our core logic source files (everything EXCEPT main.c)
file(GLOB CORE_SOURCE_FILES "src/address_book.c" "src/contact_helper.c")

# 2. Build our "engine": a reusable STATIC library with our core logic.
add_library(addressbook_lib STATIC ${CORE_SOURCE_FILES})

# 3. Tell our library where to find its public header files.
target_include_directories(addressbook_lib PUBLIC include)

# 4. Build our main application executable. It only needs main.c.
add_executable(addressbook src/main.c)

# 5. Link the "engine" (our library) into our main application.
target_link_libraries(addressbook PRIVATE addressbook_lib)

# (The Windows fix and testing setup remain the same)
if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mconsole")
endif()

enable_testing()
add_subdirectory(test)