cmake_minimum_required(VERSION 3.28)
project(Bookstore_2024)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/backend)
include_directories(src/backend/lib)
include_directories(src/backend/static)

add_executable(Bookstore_2024
    src/backend/static/constants.hpp
    src/backend/account.cpp
    src/backend/account.hpp
    src/backend/bookkeeper.cpp
    src/backend/bookkeeper.hpp
    src/backend/logging.cpp
    src/backend/logging.hpp
    src/backend/main.cpp
        src/backend/lib/norb_blocklist.hpp
        src/backend/lib/norb_utils.hpp
        src/backend/lib/norb_string.hpp
        src/backend/lib/norb_bounds.hpp
        src/backend/lib/norb_alias.hpp)

add_executable(Unit_Test

        tests/unit-test/unit_test_blocklist.cpp

        src/backend/static/constants.hpp
        src/backend/account.cpp
        src/backend/account.hpp
        src/backend/bookkeeper.cpp
        src/backend/bookkeeper.hpp
        src/backend/logging.cpp
        src/backend/logging.hpp
        src/backend/lib/norb_blocklist.hpp
        src/backend/lib/norb_utils.hpp
        src/backend/lib/norb_string.hpp
        src/backend/lib/norb_bounds.hpp
        src/backend/lib/norb_alias.hpp
)