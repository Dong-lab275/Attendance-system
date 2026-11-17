#ifndef CLASS_SESSION_H
#define CLASS_SESSION_H

#include <string>

using std::string;

const int AUTO_CLOSE_DELAY_MINUTES = 15;

class ClassSession {
private:
    char sessionId[5];
    string classDate;
    string sessionStatus; // "NOT_STARTED", "ATTENDANCE_OPEN", "ATTENDANCE_CLOSED"

public:
    ClassSession(const char* id, string date, string status);
    
    void openAttendanceWindow(int durationMinutes);
    void autoCloseAndMarkAbsent();
    string getSessionStatus() const;
};

#endif