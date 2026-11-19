#include "LecturerView.h"
#include <iostream>

using namespace std;

LecturerView::LecturerView(LecturerController* controller, ClassSession* session, AttendanceRecord* record) {
    lecturerController = controller;
    currentSession = session;
    currentRecord = record;
}

void LecturerView::displayMenu() {
    cout << "=== LECTURER VIEW ===\n";
    cout << "1. Mo phien diem danh\n";
    cout << "2. Diem danh thu cong\n";
    cout << "3. Sua trang thai diem danh\n";
}

void LecturerView::handleOpenSessionInput() {
    int selectedDuration = 0;
    cout << "Nhap so phut mo diem danh: ";
    cin >> selectedDuration;

    if (lecturerController != nullptr && currentSession != nullptr) {
        lecturerController->manageAndOpenSession(*currentSession, selectedDuration);
    }
}

void LecturerView::handleManualAttendanceInput() {
    string selectedStatus;
    string noteText;
    cout << "Nhap trang thai (PRESENT / LATE / ABSENT): ";
    cin >> selectedStatus;
    cout << "Nhap ghi chu: ";
    cin >> noteText;

    if (lecturerController != nullptr && currentRecord != nullptr) {
        lecturerController->manualRecordAttendance(*currentRecord, selectedStatus, noteText);
    }
}

void LecturerView::handleModifyAttendanceInput() {
    string newStatus;
    cout << "Nhap trang thai moi (PRESENT / LATE / ABSENT): ";
    cin >> newStatus;

    if (lecturerController != nullptr && currentRecord != nullptr) {
        lecturerController->modifyAttendanceStatus(*currentRecord, newStatus);
    }
}