
cmake_minimum_required(VERSION 3.21)

project(AttendanceSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# PostgreSQL 18
set(PostgreSQL_INCLUDE_DIR "C:/Program Files/PostgreSQL/18/include")
set(PostgreSQL_LIBRARY "C:/Program Files/PostgreSQL/18/lib/libpq.lib")
find_package(PostgreSQL REQUIRED)

# OpenSSL
set(OPENSSL_ROOT_DIR "C:/Program Files/PostgreSQL/18")
set(OPENSSL_INCLUDE_DIR "C:/msys64/ucrt64/include")
set(OPENSSL_CRYPTO_LIBRARY "C:/Program Files/PostgreSQL/18/lib/libcrypto.lib")
set(OPENSSL_SSL_LIBRARY "C:/Program Files/PostgreSQL/18/lib/libssl.lib")
find_package(OpenSSL REQUIRED COMPONENTS Crypto)

# HTTP Server and JSON
set(CPP_HTTPLIB_INCLUDE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/cpp-httplib"
)

set(NLOHMANN_JSON_INCLUDE_DIR
    "C:/msys64/ucrt64/include"
)

# Main executable
add_executable(AttendanceSystem
    src/main.cpp

    AdminUserModule/database/DatabaseConnection.cpp

    AdminUserModule/models/User.cpp
    AdminUserModule/models/Administrator.cpp

    AdminUserModule/controllers/UserController.cpp
    AdminUserModule/controllers/AdministratorController.cpp
)

target_include_directories(AttendanceSystem PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/AdminUserModule"
    "${PostgreSQL_INCLUDE_DIR}"
    "${OPENSSL_INCLUDE_DIR}"
    "${CPP_HTTPLIB_INCLUDE_DIR}"
    "${NLOHMANN_JSON_INCLUDE_DIR}"
)

target_link_libraries(AttendanceSystem PRIVATE
    PostgreSQL::PostgreSQL
    OpenSSL::Crypto
)
