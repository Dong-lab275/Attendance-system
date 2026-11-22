
cmake_minimum_required(VERSION 3.21)

project(AttendanceSystem VERSION 1.0 LANGUAGES CXX)

# ==========================================
# C++ Configuration
# ==========================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ==========================================
# MSYS2 UCRT64
# ==========================================

set(MSYS2_ROOT "C:/msys64/ucrt64")

# ==========================================
# PostgreSQL
# ==========================================

set(PostgreSQL_INCLUDE_DIR "${MSYS2_ROOT}/include")
set(PostgreSQL_LIBRARY "${MSYS2_ROOT}/lib/libpq.dll.a")

find_package(PostgreSQL REQUIRED)

# ==========================================
# OpenSSL
# ==========================================

set(OPENSSL_ROOT_DIR "${MSYS2_ROOT}")
set(OPENSSL_INCLUDE_DIR "${MSYS2_ROOT}/include")
set(OPENSSL_CRYPTO_LIBRARY "${MSYS2_ROOT}/lib/libcrypto.dll.a")
set(OPENSSL_SSL_LIBRARY "${MSYS2_ROOT}/lib/libssl.dll.a")

find_package(OpenSSL REQUIRED COMPONENTS Crypto SSL)

# ==========================================
# Threads
# ==========================================

find_package(Threads REQUIRED)

# ==========================================
# Third-party Libraries
# ==========================================

set(CPP_HTTPLIB_INCLUDE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/cpp-httplib"
)

set(NLOHMANN_JSON_INCLUDE_DIR
    "${MSYS2_ROOT}/include"
)

# ==========================================
# Main Executable
# ==========================================

add_executable(AttendanceSystem

    # Main
    src/main.cpp

    # API
    AdminUserModule/api/ApiServer.cpp

    # Database
    AdminUserModule/database/DatabaseConnection.cpp

    # Models
    AdminUserModule/models/User.cpp
    AdminUserModule/models/Administrator.cpp

    # Controllers
    AdminUserModule/controllers/UserController.cpp
    AdminUserModule/controllers/AdministratorController.cpp
)

# ==========================================
# Include Directories
# ==========================================

target_include_directories(AttendanceSystem PRIVATE

    "${CMAKE_CURRENT_SOURCE_DIR}/src"

    "${CMAKE_CURRENT_SOURCE_DIR}/AdminUserModule"

    "${CPP_HTTPLIB_INCLUDE_DIR}"

    "${NLOHMANN_JSON_INCLUDE_DIR}"
)

# ==========================================
# Link Libraries
# ==========================================

target_link_libraries(AttendanceSystem PRIVATE

    PostgreSQL::PostgreSQL

    OpenSSL::Crypto
    OpenSSL::SSL

    Threads::Threads

    ws2_32
)
