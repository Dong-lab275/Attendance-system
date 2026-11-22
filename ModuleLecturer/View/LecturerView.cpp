#include "LecturerView.h"
#include <iostream>
#include <iomanip>

void LecturerView::showMainMenu(const string& lecturerSummary, const string& selectedCourseId) const {
    cout << "\n============================================================\n";
    cout << " STUDENT ATTENDANCE MANAGEMENT SYSTEM - LECTURER WORKSPACE\n";
    cout << " Lecturer: " << lecturerSummary << "\n";
    cout << " Active Course: " << selectedCourseId << "\n";
    cout << "============================================================\n";
    cout << "1. Hien thi danh sach khoa hoc duoc phan cong\n";
    cout << "2. Chon khoa hoc lam viec\n";
    cout << "3. Xem lich hoc & khung diem danh cua khoa hoc\n";
    cout << "4. Tao buoi hoc moi (tu dong tinh gio dong +15 phut)\n";
    cout << "5. Cap nhat khung gio diem danh / Dong phien & tu danh Absent\n";
    cout << "6. Diem danh / Sua trang thai thu cong (Present / Late / Absent)\n";
    cout << "7. Tim kiem va loc sinh vien trong lop\n";
    cout << "8. Xem lich su diem danh & thong ke cua tung sinh vien\n";
    cout << "9. Xuat bao cao diem danh buoi hoc (Report Export)\n";
    cout << "10. Xem thong tin ca nhan giang vien\n";
    cout << "0. Thoat module giang vien\n";
    cout << "------------------------------------------------------------\n";
}

void LecturerView::showAssignedCourses(const vector<CourseInfo>& courses) const {
    cout << "\n--- DANH SACH KHOA HOC DUOC PHAN CONG ---\n";
    for (size_t i = 0; i < courses.size(); ++i) {
        cout << (i + 1) << ". [" << courses[i].courseId << "] " << courses[i].courseName
             << " - GV: " << courses[i].lecturerName << "\n";
    }
}

void LecturerView::showSessions(const vector<ClassSession>& sessions) const {
    cout << "\n--- DANH SACH BUOI HOC ---\n";
    cout << left << setw(10) << "Session"
         << setw(14) << "Date"
         << setw(13) << "ClassTime"
         << setw(13) << "CheckInWin"
         << setw(12) << "AutoClose"
         << setw(12) << "Status" << "\n";
    for (size_t i = 0; i < sessions.size(); ++i) {
        cout << left << setw(10) << sessions[i].sessionNumber
             << setw(14) << sessions[i].dateStr
             << setw(13) << (sessions[i].startTime + "-" + sessions[i].endTime)
             << setw(13) << (sessions[i].checkInOpenTime + "-" + sessions[i].checkInEndTime)
             << setw(12) << sessions[i].autoCloseTime
             << setw(12) << sessions[i].status << "\n";
    }
}

void LecturerView::showSessionAttendance(const vector<AttendanceRecord>& records,
                                         const AttendanceSummary& summary) const {
    cout << "\n--- KET QUA & DIEM DANH BUOI HOC ---\n";
    cout << "Tong SV: " << summary.totalStudents
         << " | Present: " << summary.presentCount
         << " | Late: " << summary.lateCount
         << " | Absent: " << summary.absentCount << "\n";
    cout << left << setw(12) << "StudentID"
         << setw(22) << "FullName"
         << setw(12) << "CheckIn"
         << setw(12) << "Status"
         << setw(22) << "UpdatedBy" << "\n";
    for (size_t i = 0; i < records.size(); ++i) {
        cout << left << setw(12) << records[i].studentId
             << setw(22) << records[i].studentName
             << setw(12) << (records[i].checkInTime.empty() ? "-" : records[i].checkInTime)
             << setw(12) << records[i].status
             << setw(22) << records[i].lastUpdatedBy << "\n";
    }
}

void LecturerView::showStudents(const vector<StudentInfo>& students) const {
    cout << "\n--- KET QUA TIM KIEM / LOC SINH VIEN ---\n";
    if (students.empty()) {
        cout << "Khong tim thay sinh vien phu hop.\n";
        return;
    }
    for (size_t i = 0; i < students.size(); ++i) {
        cout << (i + 1) << ". " << students[i].studentId
             << " | " << students[i].fullName
             << " | " << students[i].email << "\n";
    }
}

void LecturerView::showStudentHistoryAndStats(const string& studentId,
                                            const vector<AttendanceRecord>& history,
                                            const AttendanceSummary& stats) const {
    cout << "\n--- LICH SU & THONG KE SINH VIEN: " << studentId << " ---\n";
    cout << fixed << setprecision(1);
    cout << "Da hoc: " << (stats.presentCount + stats.lateCount) << "/" << stats.completedSessions << " buoi"
         << " | Present: " << stats.presentCount
         << " | Late: " << stats.lateCount
         << " | Absent: " << stats.absentCount << "\n";
    cout << "Attendance rate: " << stats.attendanceRate << "%"
         << " | On-time rate: " << stats.onTimeRate << "%\n";

    for (size_t i = 0; i < history.size(); ++i) {
        cout << "  - Session " << history[i].sessionNumber
             << " (" << history[i].sessionDate << " " << history[i].classTimeRange << ")"
             << " | Check-in: " << (history[i].checkInTime.empty() ? "-" : history[i].checkInTime)
             << " | Status: " << history[i].status << "\n";
    }
}

void LecturerView::showPersonalInfo(const string& infoText) const {
    cout << "\n--- THONG TIN CA NHAN GIANG VIEN ---\n";
    cout << infoText << "\n";
}

void LecturerView::showMessage(const string& msg) const {
    cout << "\n[Thong bao] " << msg << "\n";
}

void LecturerView::showReportExport(const string& reportText) const {
    cout << "\n" << reportText;
}

int LecturerView::promptMenuChoice() const {
    int choice = -1;
    cout << "Nhap lua chon: ";
    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(10000, '\n');
        return -1;
    }
    cin.ignore(10000, '\n');
    return choice;
}

string LecturerView::promptLine(const string& label) const {
    string line;
    cout << label;
    getline(cin, line);
    return line;
}

int LecturerView::promptInt(const string& label) const {
    int val = 0;
    cout << label;
    if (!(cin >> val)) {
        cin.clear();
        cin.ignore(10000, '\n');
        return -1;
    }
    cin.ignore(10000, '\n');
    return val;
}