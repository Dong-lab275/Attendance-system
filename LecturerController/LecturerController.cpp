#include "LecturerController.h"
#include <iostream>

using namespace std;

LecturerController::LecturerController(const char* id, string name, string email) {
    for (int i = 0; i < 12; ++i) {
        lecturerId[i] = id[i];
    }
    fullName = name;
    emailAddress = email;
}

void LecturerController::manageAndOpenSession(ClassSession& targetSession, int durationMinutes) {
    targetSession.openAttendanceWindow(durationMinutes);
}

void LecturerController::manualRecordAttendance(AttendanceRecord& targetRecord, string status, string notes) {
    targetRecord.updateStatus(status);
    cout << "Giang vien " << fullName << " da ghi nhan trang thai: " << status << "[cite: 2].\n";
}

void LecturerController::modifyAttendanceStatus(AttendanceRecord& targetRecord, string newStatus) {
    targetRecord.updateStatus(newStatus);
    cout << "Da cap nhat lai trang thai diem danh thanh: " << newStatus << "[cite: 2].\n";
}