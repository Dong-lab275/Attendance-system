#include "LecturerController.h"
#include <algorithm>
#include <cctype>
#include <cstdio>

namespace {
string toLowerStr(const string& s) {
    string res = s;
    for (size_t i = 0; i < res.size(); ++i) {
        res[i] = static_cast<char>(tolower(static_cast<unsigned char>(res[i])));
    }
    return res;
}
}

LecturerController::LecturerController() {
    lecturerId = "GV001";
    lecturerName = "Tran Quoc Huy";
    department = "Cong nghe thong tin";
    email = "quochuy.tran@university.edu.vn";
    phone = "091 234 5678";
    gender = "Male";
    seedSampleData();
}

int LecturerController::timeToMinutes(const string& hhmm) const {
    if (hhmm.size() < 5 || hhmm[2] != ':') {
        return -1;
    }
    int h = (hhmm[0] - '0') * 10 + (hhmm[1] - '0');
    int m = (hhmm[3] - '0') * 10 + (hhmm[4] - '0');
    if (h < 0 || h > 23 || m < 0 || m > 59) {
        return -1;
    }
    return h * 60 + m;
}

string LecturerController::minutesToTime(int totalMinutes) const {
    int normalized = ((totalMinutes % 1440) + 1440) % 1440;
    int h = normalized / 60;
    int m = normalized % 60;
    char buf[6];
    snprintf(buf, sizeof(buf), "%02d:%02d", h, m);
    return string(buf);
}

string LecturerController::computeAutoCloseTime(const string& periodEndTime) const {
    int endMin = timeToMinutes(periodEndTime);
    if (endMin < 0) {
        return "09:30";
    }
    // Quy dinh: tu dong dong sau 15 phut ke tu khi ket thuc thoi gian diem danh
    return minutesToTime(endMin + 15);
}

string LecturerController::calculateAutoStatus(const string& checkInTime, const string& checkInEndTime) const {
    int inMin = timeToMinutes(checkInTime);
    int endMin = timeToMinutes(checkInEndTime);
    if (inMin < 0 || endMin < 0) {
        return "Absent";
    }
    return (inMin <= endMin) ? "Present" : "Late";
}

void LecturerController::seedSampleData() {
    assignedCourses.clear();
    sessions.clear();
    students.clear();
    records.clear();

    assignedCourses.push_back({"CS201", "Co so du lieu", lecturerId, lecturerName});
    assignedCourses.push_back({"CS204", "Cau truc du lieu va giai thuat", lecturerId, lecturerName});

    sessions.push_back({12, "CS201", "04 Nov 2026", "09:00", "11:00", "09:00", "09:15", "09:30", "Closed"});
    sessions.push_back({13, "CS201", "11 Nov 2026", "09:00", "11:00", "09:00", "09:15", "09:30", "Open"});
    sessions.push_back({14, "CS201", "18 Nov 2026", "09:00", "11:00", "09:00", "09:15", "09:30", "Scheduled"});
    sessions.push_back({15, "CS201", "25 Nov 2026", "09:00", "11:00", "09:00", "09:15", "09:30", "Scheduled"});
    sessions.push_back({1,  "CS204", "14 Nov 2026", "07:00", "09:00", "07:00", "07:15", "07:30", "Scheduled"});

    students.push_back({"SV2024001", "Nguyen Minh Anh", "minhanh.nguyen@university.edu.vn", "CS201"});
    students.push_back({"SV2024002", "Tran Duc Anh",     "ducanh.tran@university.edu.vn",    "CS201"});
    students.push_back({"SV2024003", "Le Ngoc Mai",      "ngocmai.le@university.edu.vn",     "CS201"});
    students.push_back({"SV2024004", "Pham Gia Bao",     "giabao.pham@university.edu.vn",    "CS201"});
    students.push_back({"SV2024005", "Vu Khanh Linh",    "khanhlinh.vu@university.edu.vn",   "CS204"});

    // Du lieu mau Session 12 (CS201)
    records.push_back({"SV2024001", "Nguyen Minh Anh", "CS201", 12, "04 Nov 2026", "09:00-11:00", "09:03", "Present", "System", "04 Nov 2026 09:03"});
    records.push_back({"SV2024002", "Tran Duc Anh",     "CS201", 12, "04 Nov 2026", "09:00-11:00", "09:02", "Present", "System", "04 Nov 2026 09:02"});
    records.push_back({"SV2024003", "Le Ngoc Mai",      "CS201", 12, "04 Nov 2026", "09:00-11:00", "09:12", "Late",    "System", "04 Nov 2026 09:12"});
    records.push_back({"SV2024004", "Pham Gia Bao",     "CS201", 12, "04 Nov 2026", "09:00-11:00", "",      "Absent",  "System", "04 Nov 2026 09:30"});

    // Du lieu mau Session 13 (CS201 - dang Open)
    records.push_back({"SV2024001", "Nguyen Minh Anh", "CS201", 13, "11 Nov 2026", "09:00-11:00", "09:04", "Present", "System", "11 Nov 2026 09:04"});
    records.push_back({"SV2024002", "Tran Duc Anh",     "CS201", 13, "11 Nov 2026", "09:00-11:00", "",      "Absent",  "System", "11 Nov 2026 09:00"});
    records.push_back({"SV2024003", "Le Ngoc Mai",      "CS201", 13, "11 Nov 2026", "09:00-11:00", "",      "Absent",  "System", "11 Nov 2026 09:00"});
    records.push_back({"SV2024004", "Pham Gia Bao",     "CS201", 13, "11 Nov 2026", "09:00-11:00", "",      "Absent",  "System", "11 Nov 2026 09:00"});
}

string LecturerController::getLecturerSummary() const {
    return lecturerName + " (" + lecturerId + " - Lecturer)";
}

string LecturerController::getPersonalInfoText() const {
    return "Lecturer ID : " + lecturerId + "\n" +
           "Full name   : " + lecturerName + "\n" +
           "Gender      : " + gender + "\n" +
           "Department  : " + department + "\n" +
           "Email       : " + email + "\n" +
           "Phone       : " + phone;
}

vector<CourseInfo> LecturerController::getAssignedCourses() const {
    return assignedCourses;
}

vector<ClassSession> LecturerController::getSessionsByCourse(const string& courseId) const {
    vector<ClassSession> result;
    for (size_t i = 0; i < sessions.size(); ++i) {
        if (sessions[i].courseId == courseId) {
            result.push_back(sessions[i]);
        }
    }
    return result;
}

bool LecturerController::createClassSession(const string& courseId, int sessionNumber,
                                            const string& dateStr, const string& startTime,
                                            const string& endTime, const string& checkInOpenTime,
                                            const string& checkInEndTime, string& message) {
    bool isAssigned = false;
    for (size_t i = 0; i < assignedCourses.size(); ++i) {
        if (assignedCourses[i].courseId == courseId) {
            isAssigned = true;
            break;
        }
    }
    if (!isAssigned) {
        message = "Loi: Khoa hoc khong thuoc danh sach duoc phan cong cho giang vien.";
        return false;
    }
    if (sessionNumber <= 0 || dateStr.empty()) {
        message = "Loi: So buoi hoc hoac ngay hoc khong hop le.";
        return false;
    }
    for (size_t i = 0; i < sessions.size(); ++i) {
        if (sessions[i].courseId == courseId && sessions[i].sessionNumber == sessionNumber) {
            message = "Loi: Buoi hoc nay da ton tai trong khoa hoc.";
            return false;
        }
    }

    string autoClose = computeAutoCloseTime(checkInEndTime);
    ClassSession newSession{sessionNumber, courseId, dateStr, startTime, endTime,
                            checkInOpenTime, checkInEndTime, autoClose, "Scheduled"};
    sessions.push_back(newSession);

    // Khoi tao ban ghi diem danh mac dinh cho sinh vien thuoc khoa hoc
    for (size_t i = 0; i < students.size(); ++i) {
        if (students[i].courseId == courseId) {
            records.push_back({students[i].studentId, students[i].fullName, courseId,
                               sessionNumber, dateStr, startTime + "-" + endTime,
                               "", "Absent", lecturerName, dateStr + " " + startTime});
        }
    }

    message = "Tao buoi hoc thanh cong! Tu dong dong diem danh luc " + autoClose + ".";
    return true;
}

bool LecturerController::updateSessionWindow(const string& courseId, int sessionNumber,
                                           const string& newOpenTime, const string& newEndTime,
                                           const string& newStatus, string& message) {
    for (size_t i = 0; i < sessions.size(); ++i) {
        if (sessions[i].courseId == courseId && sessions[i].sessionNumber == sessionNumber) {
            sessions[i].checkInOpenTime = newOpenTime;
            sessions[i].checkInEndTime = newEndTime;
            sessions[i].autoCloseTime = computeAutoCloseTime(newEndTime);
            if (newStatus == "Scheduled" || newStatus == "Open" || newStatus == "Closed") {
                sessions[i].status = newStatus;
            }
            if (sessions[i].status == "Closed") {
                return closeSessionAndMarkAbsent(courseId, sessionNumber, message);
            }
            message = "Cap nhat khung diem danh thanh cong! Tu dong dong luc " + sessions[i].autoCloseTime + ".";
            return true;
        }
    }
    message = "Khong tim thay buoi hoc can cap nhat.";
    return false;
}

bool LecturerController::closeSessionAndMarkAbsent(const string& courseId, int sessionNumber, string& message) {
    int markedCount = 0;
    for (size_t i = 0; i < sessions.size(); ++i) {
        if (sessions[i].courseId == courseId && sessions[i].sessionNumber == sessionNumber) {
            sessions[i].status = "Closed";
            break;
        }
    }
    for (size_t i = 0; i < records.size(); ++i) {
        if (records[i].courseId == courseId && records[i].sessionNumber == sessionNumber) {
            if (records[i].checkInTime.empty() && records[i].status != "Present" && records[i].status != "Late") {
                records[i].status = "Absent";
                records[i].lastUpdatedBy = "Auto-Close System";
                markedCount++;
            } else if (records[i].checkInTime.empty() && records[i].status == "Absent") {
                markedCount++;
            }
        }
    }
    message = "Phien diem danh da dong. Da kiem tra/danh dau Absent cho " + to_string(markedCount) + " sinh vien chua diem danh.";
    return true;
}

vector<AttendanceRecord> LecturerController::getSessionAttendance(const string& courseId, int sessionNumber) const {
    vector<AttendanceRecord> result;
    for (size_t i = 0; i < records.size(); ++i) {
        if (records[i].courseId == courseId && records[i].sessionNumber == sessionNumber) {
            result.push_back(records[i]);
        }
    }
    return result;
}

bool LecturerController::recordStudentCheckInOrManual(const string& courseId, int sessionNumber,
                                                      const string& studentId, const string& inputTimeOrStatus,
                                                      bool isManualStatus, string& message) {
    string periodEnd = "09:15";
    for (size_t i = 0; i < sessions.size(); ++i) {
        if (sessions[i].courseId == courseId && sessions[i].sessionNumber == sessionNumber) {
            periodEnd = sessions[i].checkInEndTime;
            break;
        }
    }

    for (size_t i = 0; i < records.size(); ++i) {
        if (records[i].courseId == courseId &&
            records[i].sessionNumber == sessionNumber &&
            records[i].studentId == studentId) {
            if (isManualStatus) {
                if (inputTimeOrStatus != "Present" &&
                    inputTimeOrStatus != "Late" &&
                    inputTimeOrStatus != "Absent") {
                    message = "Trang thai khong hop le (chi chap nhan Present, Late, Absent).";
                    return false;
                }
                records[i].status = inputTimeOrStatus;
                if (inputTimeOrStatus == "Absent") {
                    records[i].checkInTime = "";
                } else if (records[i].checkInTime.empty()) {
                    records[i].checkInTime = "Manual";
                }
                records[i].lastUpdatedBy = lecturerName + " (Manual)";
                message = "Da cap nhat trang thai thu cong cho " + studentId + " -> " + records[i].status + ".";
                return true;
            } else {
                records[i].checkInTime = inputTimeOrStatus;
                records[i].status = calculateAutoStatus(inputTimeOrStatus, periodEnd);
                records[i].lastUpdatedBy = lecturerName + " (Auto-Rule)";
                message = "Da ghi nhan gio " + inputTimeOrStatus + " -> Trang thai tu dong: " + records[i].status + ".";
                return true;
            }
        }
    }
    message = "Khong tim thay sinh vien trong danh sach buoi hoc.";
    return false;
}

vector<StudentInfo> LecturerController::searchAndFilterStudents(const string& courseId,
                                                              const string& keywordName,
                                                              const string& keywordId) const {
    vector<StudentInfo> result;
    string lowName = toLowerStr(keywordName);
    string lowId = toLowerStr(keywordId);

    for (size_t i = 0; i < students.size(); ++i) {
        if (students[i].courseId != courseId) {
            continue;
        }
        bool matchName = lowName.empty() || (toLowerStr(students[i].fullName).find(lowName) != string::npos);
        bool matchId = lowId.empty() || (toLowerStr(students[i].studentId).find(lowId) != string::npos);
        if (matchName && matchId) {
            result.push_back(students[i]);
        }
    }
    return result;
}

vector<AttendanceRecord> LecturerController::getStudentHistoryInCourse(const string& courseId,
                                                                     const string& studentId,
                                                                     const string& statusFilter) const {
    vector<AttendanceRecord> result;
    for (size_t i = 0; i < records.size(); ++i) {
        if (records[i].courseId == courseId && records[i].studentId == studentId) {
            if (statusFilter.empty() || statusFilter == "All" || records[i].status == statusFilter) {
                result.push_back(records[i]);
            }
        }
    }
    return result;
}

AttendanceSummary LecturerController::calculateStudentStatistics(const string& courseId,
                                                               const string& studentId) const {
    AttendanceSummary summary{0, 0, 0, 0, 0, 0.0, 0.0};
    for (size_t i = 0; i < records.size(); ++i) {
        if (records[i].courseId == courseId && records[i].studentId == studentId) {
            summary.completedSessions++;
            if (records[i].status == "Present") {
                summary.presentCount++;
            } else if (records[i].status == "Late") {
                summary.lateCount++;
            } else if (records[i].status == "Absent") {
                summary.absentCount++;
            }
        }
    }
    summary.totalStudents = 1;
    if (summary.completedSessions > 0) {
        summary.attendanceRate = ((summary.presentCount + summary.lateCount) * 100.0) / summary.completedSessions;
        summary.onTimeRate = (summary.presentCount * 100.0) / summary.completedSessions;
    }
    return summary;
}

AttendanceSummary LecturerController::calculateSessionStatistics(const string& courseId,
                                                               int sessionNumber) const {
    AttendanceSummary summary{0, 0, 0, 0, 1, 0.0, 0.0};
    for (size_t i = 0; i < records.size(); ++i) {
        if (records[i].courseId == courseId && records[i].sessionNumber == sessionNumber) {
            summary.totalStudents++;
            if (records[i].status == "Present") {
                summary.presentCount++;
            } else if (records[i].status == "Late") {
                summary.lateCount++;
            } else if (records[i].status == "Absent") {
                summary.absentCount++;
            }
        }
    }
    if (summary.totalStudents > 0) {
        summary.attendanceRate = ((summary.presentCount + summary.lateCount) * 100.0) / summary.totalStudents;
        summary.onTimeRate = (summary.presentCount * 100.0) / summary.totalStudents;
    }
    return summary;
}

string LecturerController::exportSessionReportText(const string& courseId, int sessionNumber) const {
    AttendanceSummary sum = calculateSessionStatistics(courseId, sessionNumber);
    vector<AttendanceRecord> list = getSessionAttendance(courseId, sessionNumber);

    string report = "======================================================\n";
    report += "ATTENDANCE REPORT EXPORT\n";
    report += "Course: " + courseId + " | Session: " + to_string(sessionNumber) + "\n";
    report += "Lecturer: " + lecturerName + " (" + lecturerId + ")\n";
    report += "Summary -> Students: " + to_string(sum.totalStudents) +
              " | Present/Late/Absent: " + to_string(sum.presentCount) + "/" +
              to_string(sum.lateCount) + "/" + to_string(sum.absentCount) + "\n";
    report += "------------------------------------------------------\n";
    for (size_t i = 0; i < list.size(); ++i) {
        report += list[i].studentId + " | " + list[i].studentName +
                  " | Check-in: " + (list[i].checkInTime.empty() ? "-" : list[i].checkInTime) +
                  " | Status: " + list[i].status + "\n";
    }
    report += "======================================================\n";
    return report;
}