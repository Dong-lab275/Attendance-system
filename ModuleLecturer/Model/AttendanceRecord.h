#ifndef ATTENDANCERECORD_H
#define ATTENDANCERECORD_H

#include <string>

using namespace std;

struct StudentInfo {
    string studentId; // CHAR(12) in students, e.g., "079205003456"
    string fullName;  // VARCHAR(100) in students, e.g., "Le Minh Khang"
    string email;     // VARCHAR(100) in students, e.g., "khang.le@student.edu.vn"
    string courseId;  // CHAR(5) in course_registrations / courses, e.g., "C0001"
};

struct AttendanceRecord {
    string attendanceId;  // CHAR(12) in attendance_records, e.g., "AT0000000001"
    string studentId;     // CHAR(12) in attendance_records, e.g., "079205003456"
    string studentName;   // Mapped from students.full_name
    string courseId;      // CHAR(5) mapped via class_sessions.course_id, e.g., "C0001"
    string sessionId;     // CHAR(5) in attendance_records / class_sessions, e.g., "SE001"
    int sessionNumber;    // Ordinal session number for UI/controller display, e.g., 1
    string sessionDate;   // DATE in class_sessions, e.g., "2026-09-07"
    string classTimeRange;// Mapped from start_time - end_time, e.g., "07:30-09:30"
    string checkInTime;   // Mapped from attendance_timestamp time ("07:25") or "" if absent
    string status;        // CHECK (status IN ('Present', 'Absent', 'Late'))
    string notes;         // TEXT in attendance_records.notes
    string lastUpdatedBy; // Application-level audit note
    string lastUpdatedAt; // Mapped from updated_at / created_at
};

#endif