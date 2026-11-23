
cmake_minimum_required(VERSION 3.20)

project(StudentAttendanceManagementSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# =========================================
# 1. Admin / User System
# =========================================

add_executable(AttendanceSystem
    src/main_admin.cpp

    AdminUserModule/controllers/UserController.cpp
    AdminUserModule/controllers/AdministratorController.cpp

    AdminUserModule/database/DatabaseConnection.cpp

    AdminUserModule/models/User.cpp
    AdminUserModule/models/Administrator.cpp

    AdminUserModule/api/ApiServer.cpp
)

target_include_directories(AttendanceSystem PRIVATE
    ${CMAKE_SOURCE_DIR}/AdminUserModule
    ${CMAKE_SOURCE_DIR}/AdminUserModule/controllers
    ${CMAKE_SOURCE_DIR}/AdminUserModule/database
    ${CMAKE_SOURCE_DIR}/AdminUserModule/models
    ${CMAKE_SOURCE_DIR}/AdminUserModule/api
    ${CMAKE_SOURCE_DIR}/StudentModule
)

# PostgreSQL
find_package(PostgreSQL REQUIRED)
find_package(OpenSSL REQUIRED)

target_link_libraries(AttendanceSystem PRIVATE
    PostgreSQL::PostgreSQL
    OpenSSL::Crypto
    ws2_32
)


# =========================================
# 2. Lecturer System
# =========================================

add_executable(LecturerSystem
    src/main_lecturer.cpp

    ModuleLecturer/Controller/LecturerController.cpp

    ModuleLecturer/Model/AttendanceRecord.cpp
    ModuleLecturer/Model/ClassSession.cpp

    ModuleLecturer/View/LecturerView.cpp
)

target_include_directories(LecturerSystem PRIVATE
    ${CMAKE_SOURCE_DIR}/ModuleLecturer
    ${CMAKE_SOURCE_DIR}/ModuleLecturer/Controller
    ${CMAKE_SOURCE_DIR}/ModuleLecturer/Model
    ${CMAKE_SOURCE_DIR}/ModuleLecturer/View
)


# =========================================
# 3. Student System
# =========================================

add_executable(StudentSystem
    src/main_student.cpp

    StudentModule/AttendanceRecord.cpp
    StudentModule/Course.cpp
    StudentModule/Student.cpp
    StudentModule/StudentController.cpp
    StudentModule/StudentView.cpp
)

target_include_directories(StudentSystem PRIVATE
    ${CMAKE_SOURCE_DIR}/StudentModule
)


# =========================================
# 4. Compiler Options
# =========================================

if(MSVC)
    target_compile_options(AttendanceSystem PRIVATE /W4)
    target_compile_options(LecturerSystem PRIVATE /W4)
    target_compile_options(StudentSystem PRIVATE /W4)
else()
    target_compile_options(AttendanceSystem PRIVATE -Wall -Wextra)
    target_compile_options(LecturerSystem PRIVATE -Wall -Wextra)
    target_compile_options(StudentSystem PRIVATE -Wall -Wextra)
endif()
