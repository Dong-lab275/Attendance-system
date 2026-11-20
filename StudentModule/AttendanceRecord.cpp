
#include "AttendanceRecord.h"
#include <iostream>
using namespace std;

AttendanceRecord::AttendanceRecord() {
    studentId = "";
    sessionId = "";
    status = "Absent";
}

void AttendanceRecord::checkIn(string studentId, string sessionId) {
    this->studentId = studentId;
    this->sessionId = sessionId;
    status = "Present";

    cout << "Check-in demo successful!" << endl;
}

void AttendanceRecord::displayAttendance() const {
    cout << "Student ID: " << studentId << endl;
    cout << "Session ID: " << sessionId << endl;
    cout << "Status: " << status << endl;
}
