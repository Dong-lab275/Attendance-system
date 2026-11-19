
#include <iostream>
#include "StudentView.h"

using namespace std;

void StudentView::displayMenu() {
    cout << "\n===== STUDENT MENU =====\n";
    cout << "1. View personal information\n";
    cout << "2. Course registration\n";
    cout << "3. Attendance check-in\n";
    cout << "4. Attendance history\n";
    cout << "5. Attendance rate\n";
    cout << "0. Exit\n";
}

void StudentView::displayMessage() {
    cout << "Function is not implemented yet.\n";
}
