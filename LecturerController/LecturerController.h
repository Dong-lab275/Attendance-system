#ifndef LECTURER_CONTROLLER_H
#define LECTURER_CONTROLLER_H

#include <string>
#include "ClassSession.h"
#include "AttendanceRecord.h"

using std::string;

class LecturerController {
private:
    char lecturerId[12];
    string fullName;
    string emailAddress;

public:
    LecturerController(const char* id, string name, string email);
    
    void manageAndOpenSession(ClassSession& targetSession, int durationMinutes);
    void manualRecordAttendance(AttendanceRecord& targetRecord, string status, string notes);
    void modifyAttendanceStatus(AttendanceRecord& targetRecord, string newStatus);
};

#endif