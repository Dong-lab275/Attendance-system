#include "AttendanceRecord.h"
#include <iostream>
#include "StudentController.h"
#include "StudentView.h"

using namespace std;

int main() {
    StudentController controller;
    AttendanceRecord attendance;
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
else if (choice == 3) {
    string courseId;

    cout << "Enter Course ID: ";
    cin >> courseId;

    attendance.checkIn("ST001", courseId);
    attendance.displayAttendance();
}


else if (choice == 4) {
    cout << "\n===== ATTENDANCE HISTORY =====\n";
    attendance.displayAttendance();
}

else if (choice == 5) {
    int totalSessions = 10;
    int attendedSessions = 8;

    double attendanceRate =
        (double)attendedSessions / totalSessions * 100;

    cout << "\n===== ATTENDANCE RATE =====\n";
    cout << "Total sessions: " << totalSessions << endl;
    cout << "Attended sessions: " << attendedSessions << endl;
    cout << "Attendance rate: " << attendanceRate << "%" << endl;
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


