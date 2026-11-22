#ifndef LECTURERCONTROLLER_H
#define LECTURERCONTROLLER_H

#include <string>
#include <vector>
#include "../Model/ClassSession.h"
#include "../Model/AttendanceRecord.h"

using namespace std;

struct AttendanceSummary {
    int totalStudents;
    int presentCount;
    int lateCount;
    int absentCount;
    int completedSessions;
    double attendanceRate; // (Present + Late) / completedSessions * 100
    double onTimeRate;     // Present / completedSessions * 100
};

class LecturerController {
private:
    string lecturerId;      // CHAR(12) in lecturers, e.g., "079082001234"
    string lecturerName;    // VARCHAR(100) in lecturers, e.g., "Nguyen Van An"
    string department;      // Mapped from departments.department_name (e.g., D0001 -> "Information Technology")
    string email;           // VARCHAR(100) in lecturers
    string phone;           // VARCHAR(15) in lecturers
    string gender;          // VARCHAR(10) in lecturers

    vector<CourseInfo> assignedCourses; // course_id CHAR(5), lecturer_id CHAR(12)
    vector<ClassSession> sessions;      // session_id CHAR(5), course_id CHAR(5), status ('Scheduled', 'Open', 'Closed', 'Completed')
    vector<StudentInfo> students;       // student_id CHAR(12), course_id CHAR(5) via course_registrations
    vector<AttendanceRecord> records;   // attendance_id CHAR(12), student_id CHAR(12), session_id CHAR(5), status ('Present', 'Absent', 'Late')

    string calculateAutoStatus(const string& checkInTime, const string& checkInEndTime) const;
    string computeAutoCloseTime(const string& periodEndTime) const;
    int timeToMinutes(const string& hhmm) const;
    string minutesToTime(int totalMinutes) const;

public:
    LecturerController();

    void seedSampleData();

    // Thong tin giang vien & khoa hoc duoc phan cong
    string getLecturerSummary() const;
    string getPersonalInfoText() const;
    vector<CourseInfo> getAssignedCourses() const;

    // Use Case: Class Session Management
    vector<ClassSession> getSessionsByCourse(const string& courseId) const;
    bool createClassSession(const string& courseId, int sessionNumber, const string& dateStr,
                            const string& startTime, const string& endTime,
                            const string& checkInOpenTime, const string& checkInEndTime,
                            string& message);
    bool updateSessionWindow(const string& courseId, int sessionNumber,
                             const string& newOpenTime, const string& newEndTime,
                             const string& newStatus, string& message);
    bool closeSessionAndMarkAbsent(const string& courseId, int sessionNumber, string& message);

    // Use Case: Manual Attendance Recording & Attendance Status Modification
    vector<AttendanceRecord> getSessionAttendance(const string& courseId, int sessionNumber) const;
    bool recordStudentCheckInOrManual(const string& courseId, int sessionNumber,
                                      const string& studentId, const string& inputTimeOrStatus,
                                      bool isManualStatus, string& message);

    // Use Case: Student Search & Filtering
    vector<StudentInfo> searchAndFilterStudents(const string& courseId, const string& keywordName,
                                                const string& keywordId) const;

    // Use Case: View Student Attendance History & View Attendance Statistics
    vector<AttendanceRecord> getStudentHistoryInCourse(const string& courseId, const string& studentId,
                                                       const string& statusFilter) const;
    AttendanceSummary calculateStudentStatistics(const string& courseId, const string& studentId) const;
    AttendanceSummary calculateSessionStatistics(const string& courseId, int sessionNumber) const;

    // Use Case: Attendance Report Export
    string exportSessionReportText(const string& courseId, int sessionNumber) const;
};

#endif