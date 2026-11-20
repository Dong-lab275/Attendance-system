
cmake_minimum_required(VERSION 3.16)

project(StudentModule LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(StudentModuleGUI
    StudentGUI/main_gui.cpp
    StudentGUI/StudentCheckInWindow.cpp
    StudentGUI/StudentCheckInWindow.h
)

target_link_libraries(StudentModuleGUI PRIVATE Qt6::Widgets)
