#ifndef CLASSSESSION_H
#define CLASSSESSION_H

#include <string>

using namespace std;

struct CourseInfo {
    string courseId;
    string courseName;
    string lecturerId;
    string lecturerName;
};

struct ClassSession {
    int sessionNumber;
    string courseId;
    string dateStr;          // Vi du: "11 Nov 2026"
    string startTime;        // Vi du: "09:00"
    string endTime;          // Vi du: "11:00"
    string checkInOpenTime;  // Vi du: "09:00"
    string checkInEndTime;   // Vi du: "09:15"
    string autoCloseTime;    // Tu dong dong sau 15 phut, vi du: "09:30"
    string status;           // "Scheduled", "Open", "Closed"
};

#endif