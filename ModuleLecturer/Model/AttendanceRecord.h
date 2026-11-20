#ifndef ATTENDANCERECORD_H
#define ATTENDANCERECORD_H

#include <string>

using namespace std;

struct StudentInfo {
    string studentId;
    string fullName;
    string email;
    string courseId;
};

struct AttendanceRecord {
    string studentId;
    string studentName;
    string courseId;
    int sessionNumber;
    string sessionDate;
    string classTimeRange;
    string checkInTime;   // Vi du: "09:03", hoac "" neu vang
    string status;        // "Present", "Late", "Absent"
    string lastUpdatedBy; // Phuc vu lich su thay doi trang thai (Attendance Status Modification)
    string lastUpdatedAt;
};

#endif