#include "AttendanceRecord.h"

using namespace std;

AttendanceRecord::AttendanceRecord(const char* id, string currentStatus, string noteText) {
    for (int i = 0; i < 12; ++i) {
        attendanceId[i] = id[i];
    }
    status = currentStatus;
    notes = noteText;
}

void AttendanceRecord::updateStatus(string newStatus) {
    status = newStatus;
}

string AttendanceRecord::getStatus() const {
    return status;
}