#include <iostream>
#include "ClassSession.h"
#include "AttendanceRecord.h"
#include "LecturerController.h"
#include "LecturerView.h"

using namespace std;

int main() {
    ClassSession currentSession("S13", "19-11-2026", "NOT_STARTED");
    AttendanceRecord currentRecord("ATT001", "ABSENT", "Chua diem danh");
    LecturerController lecturerController("GV001", "Tran Quoc Huy", "huy.vq@university.edu.vn");
    LecturerView lecturerView(&lecturerController, &currentSession, &currentRecord);

    int menuChoice = 0;

    do {
        lecturerView.displayMenu();
        cout << "0. Thoat\n";
        cout << "Chon chuc nang: ";
        cin >> menuChoice;

        if (menuChoice == 1) {
            lecturerView.handleOpenSessionInput();
        } else if (menuChoice == 2) {
            lecturerView.handleManualAttendanceInput();
        } else if (menuChoice == 3) {
            lecturerView.handleModifyAttendanceInput();
        } else if (menuChoice == 4) {
            currentSession.autoCloseAndMarkAbsent();
        }
    } while (menuChoice != 0);

    return 0;
}