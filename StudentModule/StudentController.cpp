#include "GpsValidator.h"
#include "StudentController.h"
#include <iostream>

using namespace std;

StudentController::StudentController() {
}

void StudentController::setStudentInformation(
    string studentId,
    string fullName,
    string emailAddress,
    string phoneNumber
) {
    student = Student(
        studentId,
        fullName,
        emailAddress,
        phoneNumber
    );
}

void StudentController::updateStudentInformation(
    string fullName,
    string emailAddress,
    string phoneNumber
) {
    student.setFullName(fullName);
    student.setEmailAddress(emailAddress);
    student.setPhoneNumber(phoneNumber);
}

void StudentController::displayStudentInformation() const {
    student.displayInformation();
}

void StudentController::registerCourse(string courseId) {
    if (courseId.empty()) {
        cout << "Course ID cannot be empty!" << endl;
        return;
    }

    cout << "Course registration requested: " << courseId << endl;
    cout << "Waiting for database verification." << endl;
}

bool StudentController::canCheckIn(string sessionStatus) const {
    return sessionStatus == "Open";
}

bool StudentController::validateCheckIn(
    bool isRegistered,
    string sessionStatus,
    bool isWithinTime,
    bool isWithinGpsRadius,
    bool alreadyCheckedIn
) const {
    if (!isRegistered) {
        return false;
    }

    if (sessionStatus != "Open") {
        return false;
    }

    if (!isWithinTime) {
        return false;
    }

    if (!isWithinGpsRadius) {
        return false;
    }

    if (alreadyCheckedIn) {
        return false;
    }

    return true;
}

double StudentController::calculateAttendanceRate(
    int attendedSessions,
    int totalSessions
) const {
    if (totalSessions <= 0) {
        return 0.0;
    }

    if (attendedSessions < 0 ||
        attendedSessions > totalSessions) {
        return 0.0;
    }

    return (static_cast<double>(attendedSessions)
            / totalSessions) * 100.0;
}

void StudentController::displayAttendanceHistory(
    const string attendanceDates[],
    const string attendanceStatuses[],
    int recordCount
) const {
    if (recordCount <= 0) {
        cout << "No attendance history found." << endl;
        return;
    }

    cout << "\n===== ATTENDANCE HISTORY =====" << endl;

    for (int i = 0; i < recordCount; i++) {
        cout << "Date: " << attendanceDates[i] << endl;
        cout << "Status: " << attendanceStatuses[i] << endl;
        cout << "------------------------" << endl;
    }
}
bool StudentController::validateGpsLocation(
    double studentLatitude,
    double studentLongitude,
    double classLatitude,
    double classLongitude,
    double allowedRadius
) const {
    return GpsValidator::isWithinRadius(
        studentLatitude,
        studentLongitude,
        classLatitude,
        classLongitude,
        allowedRadius
    );
}
