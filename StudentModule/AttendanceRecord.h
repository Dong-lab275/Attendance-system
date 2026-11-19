
#ifndef ATTENDANCERECORD_H
#define ATTENDANCERECORD_H

#include <string>
using namespace std;

class AttendanceRecord {
private:
    string studentId;
    string courseId;
    string status;

public:
    AttendanceRecord();

    void checkIn(string studentId, string courseId);
    void displayAttendance() const;
};

#endif
