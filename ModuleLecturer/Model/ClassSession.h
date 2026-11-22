#ifndef CLASSSESSION_H
#define CLASSSESSION_H

#include <string>

using namespace std;

struct CourseInfo {
    string courseId;     // CHAR(5) in courses, e.g., "C0001"
    string courseName;   // VARCHAR(100) in courses, e.g., "Database Systems"
    string lecturerId;   // CHAR(12) in courses, e.g., "079082001234"
    string lecturerName; // Mapped from lecturers.full_name, e.g., "Nguyen Van An"
};

struct ClassSession {
    string sessionId;        // CHAR(5) in class_sessions, e.g., "SE001"
    int sessionNumber;       // Ordinal session number for UI/controller display, e.g., 1
    string courseId;         // CHAR(5) in class_sessions, e.g., "C0001"
    string dateStr;          // DATE in class_sessions, e.g., "2026-09-07"
    string startTime;        // TIME in class_sessions, e.g., "07:30"
    string endTime;          // TIME in class_sessions, e.g., "09:30"
    string classroom;        // VARCHAR(50) in class_sessions, e.g., "A.301"
    string checkInOpenTime;  // Mapped from attendance_start_time, e.g., "07:15"
    string checkInEndTime;   // Mapped from attendance_end_time, e.g., "07:45"
    string autoCloseTime;    // Computed auto-close time (+15 mins after checkInEndTime), e.g., "08:00"
    string status;           // Session status: "Scheduled", "Open", "Closed", "Completed"
};

#endif