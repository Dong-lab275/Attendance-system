#ifndef LECTURER_VIEW_H
#define LECTURER_VIEW_H

#include <string>
#include "LecturerController.h"
#include "ClassSession.h"
#include "AttendanceRecord.h"

using std::string;

class LecturerView {
private:
    LecturerController* lecturerController;
    ClassSession* currentSession;
    AttendanceRecord* currentRecord;

public:
    LecturerView(LecturerController* controller, ClassSession* session, AttendanceRecord* record);

    void displayMenu();
    void handleOpenSessionInput();
    void handleManualAttendanceInput();
    void handleModifyAttendanceInput();
};

#endif