#ifndef ATTENDANCE_RECORD_H
#define ATTENDANCE_RECORD_H

#include <string>

using std::string;

class AttendanceRecord {
private:
    char attendanceId[12];
    string attendanceTimestamp;
    string status; // "PRESENT", "ABSENT", "LATE"
    string notes;

public:
    AttendanceRecord(const char* id, string currentStatus, string noteText);
    
    void updateStatus(string newStatus);
    string getStatus() const;
};

#endif