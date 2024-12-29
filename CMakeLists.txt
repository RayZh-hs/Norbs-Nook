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
        src/backend/account.cpp
        src/backend/lib/norb_strlib.cpp
        src/backend/lib/norb_blocklist_init.cpp
        src/backend/bookkeeper.cpp
        src/backend/logging.cpp
        src/backend/global.cpp
        src/backend/action.cpp
        src/backend/business_logic.cpp
        src/backend/user_interface.cpp
)

# Iterate over each test source file and create a corresponding executable
foreach (TEST_SOURCE ${TEST_SOURCES})
    # Extract the file name without the extension
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

    # Add an executable for this test
    add_executable(${TEST_NAME} ${TEST_SOURCE} ${COMMON_DEPENDENCIES})
endforeach ()

# Create the final output

set(CUI_MAIN_CPP
        src/backend/main_cui.cpp
)
set(GUI_MAIN_CPP
        src/backend/main_gui.cpp
)

add_executable(code ${CUI_MAIN_CPP} ${COMMON_DEPENDENCIES})
add_executable(gui_runtime ${GUI_MAIN_CPP} ${COMMON_DEPENDENCIES})

set_target_properties(code PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)
set_target_properties(gui_runtime PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/src/api/builds/
)