
#ifndef STUDENTCONTROLLER_H
#define STUDENTCONTROLLER_H

#include "Student.h"
#include <string>

using namespace std;

class StudentController {
private:
    Student student;

public:
    StudentController();

    void setStudentInformation(
        string studentId,
        string fullName,
        string emailAddress,
        string phoneNumber
    );

    void updateStudentInformation(
        string fullName,
        string emailAddress,
        string phoneNumber
    );

    void displayStudentInformation() const;

    void registerCourse(string courseId);

    bool canCheckIn(string sessionStatus) const;
    bool validateGpsLocation(
    double studentLatitude,
    double studentLongitude,
    double classLatitude,
    double classLongitude,
    double allowedRadius
) const;

    double calculateAttendanceRate(
        int attendedSessions,
        int totalSessions
    ) const;

    bool validateCheckIn(
        bool isRegistered,
        string sessionStatus,
        bool isWithinTime,
        bool isWithinGpsRadius,
        bool alreadyCheckedIn
    ) const;

    void displayAttendanceHistory(
        const string attendanceDates[],
        const string attendanceStatuses[],
        int recordCount
    ) const;
};

#endif
