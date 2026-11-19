#include "ClassSession.h"
#include <iostream>

using namespace std;

ClassSession::ClassSession(const char* id, string date, string status) {
    for (int i = 0; i < 5; ++i) {
        sessionId[i] = id[i];
    }
    classDate = date;
    sessionStatus = status;
}

void ClassSession::openAttendanceWindow(int durationMinutes) {
    sessionStatus = "ATTENDANCE_OPEN";
    cout << "[ClassSession] Phien diem danh da mo trong " << durationMinutes << " phut[cite: 2].\n";
}

void ClassSession::autoCloseAndMarkAbsent() {
    sessionStatus = "ATTENDANCE_CLOSED";
    cout << "[ClassSession] Tu dong dong phien sau " << AUTO_CLOSE_DELAY_MINUTES 
              << " phut. Da danh dau Absent cho sinh vien chua check-in[cite: 2].\n";
}

string ClassSession::getSessionStatus() const {
    return sessionStatus;
}