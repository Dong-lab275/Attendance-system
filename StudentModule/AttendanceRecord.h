
#ifndef ATTENDANCERECORD_H
#define ATTENDANCERECORD_H

#include <string>
using namespace std;

class AttendanceRecord {
private:
    string studentId;
    string sessionId;
    string status;

public:
    AttendanceRecord();

   void checkIn(string studentId, string sessionId);
    void displayAttendance() const;
};

#endif
