
#include <iostream>
#include "StudentController.h"
#include "StudentView.h"

using namespace std;

int main() {
    StudentController controller;
    StudentView view;

    controller.setStudentInformation(
        "ST001",
        "Nguyen Van A",
        "student@gmail.com",
        "0123456789"
    );

    int choice = 0;

    do {
        view.displayMenu();
        cout << "Choose: ";

        if (!(cin >> choice)) {
            break;
        }

        if (choice == 1) {
            controller.displayStudentInformation();
        }
       
else if (choice == 2) {
    string courseId;

    cout << "Enter Course ID (C001/C002): ";
    cin >> courseId;

    controller.registerCourse(courseId);
}
else if (choice >= 3 && choice <= 5) {
    view.displayMessage();
}

        else if (choice == 0) {
            cout << "Goodbye!\n";
        }
        else {
            cout << "Invalid choice!\n";
        }

    } while (choice != 0);

    return 0;
}


