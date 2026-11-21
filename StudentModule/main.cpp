
#include "AttendanceRecord.h"
#include "StudentController.h"
#include "StudentView.h"
#include <iostream>
#include <string>

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

            cout << "Enter Course ID: ";
            cin >> courseId;

            controller.registerCourse(courseId);
        }
        else if (choice == 3) {
            string sessionId;
            string sessionStatus = "Completed";

            cout << "Enter Session ID: ";
            cin >> sessionId;

            // Demo values only.
            // Real values must come from the backend and database.
            bool isRegistered = false;
            bool isWithinTime = false;
            bool isWithinGpsRadius = false;
            bool alreadyCheckedIn = false;

            bool isAllowed = controller.validateCheckIn(
                isRegistered,
                sessionStatus,
                isWithinTime,
                isWithinGpsRadius,
                alreadyCheckedIn
            );

            if (isAllowed) {
                attendance.checkIn("ST001", sessionId);
                attendance.displayAttendance();
            }
            else {
                cout << "Check-in denied!" << endl;
            }
        }
        else if (choice == 4) {
            string attendanceDates[] = {
                "2026-11-10",
                "2026-11-12",
                "2026-11-14"
            };

            string attendanceStatuses[] = {
                "Present",
                "Absent",
                "Present"
            };

            controller.displayAttendanceHistory(
                attendanceDates,
                attendanceStatuses,
                3
            );
        }
        else if (choice == 5) {
            int totalSessions = 10;
            int attendedSessions = 8;

            double attendanceRate =
                controller.calculateAttendanceRate(
                    attendedSessions,
                    totalSessions
                );

            cout << "\n===== ATTENDANCE RATE =====\n";
            cout << "Total sessions: " << totalSessions << endl;
            cout << "Attended sessions: " << attendedSessions << endl;
            cout << "Attendance rate: "
                 << attendanceRate << "%" << endl;
        }
        else if (choice == 0) {
            cout << "Goodbye!" << endl;
        }
        else {
            cout << "Invalid choice!" << endl;
        }

    } while (choice != 0);

    return 0;
}

