
#include "AttendanceRecord.h"
#include <iostream>

using namespace std;

AttendanceRecord::AttendanceRecord() {
    studentId = "";
    courseId = "";
    status = "ABSENT";
}

void AttendanceRecord::checkIn(string studentId, string courseId) {
    this->studentId = studentId;
    this->courseId = courseId;
    status = "PRESENT";

    cout << "Check-in successful!" << endl;
}

void AttendanceRecord::displayAttendance() const {
    cout << "Student ID: " << studentId << endl;
    cout << "Course ID: " << courseId << endl;
    cout << "Status: " << status << endl;
}
