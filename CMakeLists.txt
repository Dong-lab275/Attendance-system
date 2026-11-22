
cmake_minimum_required(VERSION 3.16)
project(AttendanceManagementSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(OpenSSL REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(Threads REQUIRED)

find_path(HTTPLIB_INCLUDE_DIR
    NAMES httplib.h
    PATHS
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/cpp-httplib
        ${CMAKE_CURRENT_SOURCE_DIR}/StudentModule
)

set(CORE_SOURCES
    StudentModule/Course.cpp
    StudentModule/Student.cpp
    StudentModule/StudentController.cpp
    StudentModule/StudentView.cpp
    # Bổ sung các file .cpp của Lecturer, Admin và AttendanceRecord khi ghép đầy đủ:
    # StudentModule/AttendanceRecord.cpp
    # src/controllers/AdministratorController.cpp
    # src/controllers/UserController.cpp
)

set(CORE_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/StudentModule
    ${CMAKE_CURRENT_SOURCE_DIR}/StudentModule/StudentGUI
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/cpp-httplib
    ${CMAKE_CURRENT_SOURCE_DIR}/src/controllers
    ${CMAKE_CURRENT_SOURCE_DIR}/models
    ${CMAKE_CURRENT_SOURCE_DIR}/database
    ${PostgreSQL_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIR}
)

if(HTTPLIB_INCLUDE_DIR)
    list(APPEND CORE_INCLUDE_DIRS ${HTTPLIB_INCLUDE_DIR})
endif()

add_executable(attendance_app
    main.cpp
    ${CORE_SOURCES}
)

target_include_directories(attendance_app PRIVATE ${CORE_INCLUDE_DIRS})

target_link_libraries(attendance_app PRIVATE
    OpenSSL::Crypto
    OpenSSL::SSL
    PostgreSQL::PostgreSQL
    Threads::Threads
)

if(WIN32)
    target_link_libraries(attendance_app PRIVATE ws2_32)
endif()

# --- Google Test ---
include(FetchContent)
enable_testing()

set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(attendance_tests
    tests/test_gps.cpp
    tests/test_case.cpp
    StudentModule/Course.cpp
    StudentModule/Student.cpp
    StudentModule/StudentController.cpp
    StudentModule/StudentView.cpp
)

target_include_directories(attendance_tests PRIVATE ${CORE_INCLUDE_DIRS})

target_link_libraries(attendance_tests PRIVATE
    GTest::gtest_main
    OpenSSL::Crypto
    PostgreSQL::PostgreSQL
    Threads::Threads
)

if(WIN32)
    target_link_libraries(attendance_tests PRIVATE ws2_32)
endif()

include(GoogleTest)
gtest_discover_tests(attendance_tests)