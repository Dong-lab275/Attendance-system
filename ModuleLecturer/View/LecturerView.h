#ifndef LECTURERVIEW_H
#define LECTURERVIEW_H

#include <string>
#include <vector>
#include "../Model/ClassSession.h"
#include "../Model/AttendanceRecord.h"
#include "../Controller/LecturerController.h"

using namespace std;

class LecturerView {
public:
    void showMainMenu(const string& lecturerSummary, const string& selectedCourseId) const;
    void showAssignedCourses(const vector<CourseInfo>& courses) const;
    void showSessions(const vector<ClassSession>& sessions) const;
    void showSessionAttendance(const vector<AttendanceRecord>& records, const AttendanceSummary& summary) const;
    void showStudents(const vector<StudentInfo>& students) const;
    void showStudentHistoryAndStats(const string& studentId, const vector<AttendanceRecord>& history,
                                    const AttendanceSummary& stats) const;
    void showPersonalInfo(const string& infoText) const;
    void showMessage(const string& msg) const;
    void showReportExport(const string& reportText) const;

    int promptMenuChoice() const;
    string promptLine(const string& label) const;
    int promptInt(const string& label) const;
};

#endif