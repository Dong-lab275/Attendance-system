
cmake_minimum_required(VERSION 3.21)

project(AttendanceSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# PostgreSQL configuration
set(PostgreSQL_INCLUDE_DIR
    "C:/Program Files/PostgreSQL/18/include"
)

set(PostgreSQL_LIBRARY
    "C:/Program Files/PostgreSQL/18/lib/libpq.lib"
)

find_package(PostgreSQL REQUIRED)

# OpenSSL configuration
set(OPENSSL_INCLUDE_DIR
    "C:/Program Files/PostgreSQL/18/include"
)

set(OPENSSL_CRYPTO_LIBRARY
    "C:/Program Files/PostgreSQL/18/lib/libcrypto.lib"
)

# Source files
add_executable(AttendanceSystem
    src/main.cpp
    src/database/DatabaseConnection.cpp
    src/database/DatabaseConnection.h
    src/models/User.cpp
    src/controllers/UserController.h
    src/controllers/UserController.cpp
    src/controllers/AdministratorController.cpp
)

# Include directories
target_include_directories(AttendanceSystem PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${PostgreSQL_INCLUDE_DIR}"
    "${OPENSSL_INCLUDE_DIR}"
)

# Link libraries
target_link_libraries(AttendanceSystem PRIVATE
    PostgreSQL::PostgreSQL
    "${OPENSSL_CRYPTO_LIBRARY}"
)
