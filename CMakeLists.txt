cmake_minimum_required(VERSION 3.28)
project(Bookstore_2024)

set(CMAKE_CXX_STANDARD 17)

include_directories(src/backend)
include_directories(src/backend/lib)
include_directories(src/backend/static)


# Collect all test source files in the tests/unit-test/ directory
file(GLOB TEST_SOURCES "tests/unit-test/*.cpp")

# List common dependencies
set(COMMON_DEPENDENCIES
        #        src/backend/static/constants.hpp
        src/backend/account.cpp
        src/backend/lib/norb_blocklist_init.cpp
        #        src/backend/account.hpp
        src/backend/bookkeeper.cpp
        #        src/backend/bookkeeper.hpp
        src/backend/logging.cpp
        src/backend/global.cpp
        #        src/backend/global.hpp
        #        src/backend/logging.hpp
        #        src/backend/lib/norb_blocklist.hpp
        #        src/backend/lib/norb_utils.hpp
        #        src/backend/lib/norb_string.hpp
        #        src/backend/lib/norb_bounds.hpp
        #        src/backend/lib/norb_alias.hpp
        #        src/backend/lib/norb_naivelist.hpp
        #        src/backend/lib/norb_utils.tpp
        #        src/backend/lib/norb_bounds.tpp
        #        src/backend/lib/norb_blocklist.tpp
        #        src/backend/lib/norb_string.tpp
)

# Iterate over each test source file and create a corresponding executable
foreach (TEST_SOURCE ${TEST_SOURCES})
    # Extract the file name without the extension
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

    # Add an executable for this test
    add_executable(${TEST_NAME} ${TEST_SOURCE} ${COMMON_DEPENDENCIES})

    # Link any required libraries if needed (e.g., GTest, Boost)
    # target_link_libraries(${TEST_NAME} PRIVATE gtest gtest_main)
endforeach ()

# Create the final output

set(MAIN_CPP
        src/backend/main.cpp
)

add_executable(code ${MAIN_CPP} ${COMMON_DEPENDENCIES})

set_target_properties(code PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)