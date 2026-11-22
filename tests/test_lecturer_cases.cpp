#include <gtest/gtest.h>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <set>

namespace attendance_test {

struct User {
    std::string username;
    std::string password;
    std::string name;
    std::string email;
    std::string role;
};

struct AttendanceRecord {
    std::string student_id;
    std::string student_name;
    std::string status; // "Present", "Late", "Absent"
};

struct ClassSession {
    std::string course_id;
    int session_num;
    std::string date;
    std::string class_time;
    std::string checkin_start;
    std::string checkin_end;
    std::string auto_close_time;
    std::string status; // "Scheduled", "Open", "Closed"
    std::vector<AttendanceRecord> records;
};

struct ActionResult {
    bool ok;
    std::string message;
};

struct ReportSummary {
    int total = 0;
    int present = 0;
    int late = 0;
    int absent = 0;
    double attendance_rate = 0.0;
};

class LecturerAttendanceService {
public:
    LecturerAttendanceService() {
        users_["student01"] = {"student01", "123456", "Student One", "student01@uni.edu", "Student"};
        assigned_courses_.insert("CS201");

        // Session 12 (CS201): đã Closed, có Present/Late và SV2024004 để test TC08, TC09, TC10, TC16
        ClassSession s12;
        s12.course_id = "CS201";
        s12.session_num = 12;
        s12.date = "25 Nov 2026";
        s12.class_time = "09:00 - 11:00";
        s12.checkin_start = "09:00";
        s12.checkin_end = "09:15";
        s12.auto_close_time = "09:30";
        s12.status = "Closed";
        s12.records = {
            {"SV2024001", "Nguyễn Minh Anh", "Present"},
            {"SV2024002", "Trần Văn Bình", "Present"},
            {"SV2024003", "Lê Thị Chi", "Late"},
            {"SV2024004", "Phạm Quốc Dũng", "Absent"}
        };
        sessions_[key("CS201", 12)] = s12;

        // Session 13 (CS201): đang Open, có sinh viên chưa check-in để test TC06, TC11, TC12
        ClassSession s13;
        s13.course_id = "CS201";
        s13.session_num = 13;
        s13.date = "28 Nov 2026";
        s13.class_time = "09:00 - 11:00";
        s13.checkin_start = "09:00";
        s13.checkin_end = "09:15";
        s13.auto_close_time = "09:30";
        s13.status = "Open";
        s13.records = {
            {"SV2024001", "Nguyễn Minh Anh", "Present"},
            {"SV2024002", "Trần Văn Bình", ""},
            {"SV2024003", "Lê Thị Chi", ""},
            {"SV2024004", "Phạm Quốc Dũng", ""}
        };
        sessions_[key("CS201", 13)] = s13;
    }

    // TC01, TC02
    ActionResult login(const std::string& username, const std::string& password) const {
        auto it = users_.find(username);
        if (it == users_.end() || it->second.password != password) {
            return {false, "Invalid username or password."};
        }
        return {true, "User logs in successfully"};
    }

    // TC03
    ActionResult registerAccount(const std::string& name, const std::string& email,
                                 const std::string& username, const std::string& password) {
        if (name.empty() || email.empty() || username.empty() || password.empty()) {
            return {false, "Missing required fields"};
        }
        if (users_.count(username)) {
            return {false, "Username already exists"};
        }
        users_[username] = {username, password, name, email, "Student"};
        return {true, "Registration is completed successfully"};
    }

    // TC04, TC05
    ActionResult createClassSession(const std::string& course_id, int session_num,
                                    const std::string& date, const std::string& class_time,
                                    const std::string& checkin_window) {
        if (!assigned_courses_.count(course_id)) {
            return {false, "Course is not assigned to the lecturer"};
        }
        ClassSession s;
        s.course_id = course_id;
        s.session_num = session_num;
        s.date = date;
        s.class_time = class_time;
        s.checkin_start = "09:00";
        s.checkin_end = "09:15";
        s.auto_close_time = "09:30";
        s.status = "Scheduled";
        sessions_[key(course_id, session_num)] = s;
        return {true, "Session created"};
    }

    // TC06, TC07, TC08
    ActionResult closeSession(const std::string& course_id, int session_num) {
        auto it = sessions_.find(key(course_id, session_num));
        if (it == sessions_.end()) {
            return {false, "Session not found"};
        }
        ClassSession& s = it->second;
        if (s.status == "Closed") {
            return {true, "Session remains Closed"};
        }
        s.status = "Closed";
        for (auto& rec : s.records) {
            if (rec.status.empty()) {
                rec.status = "Absent";
            }
        }
        return {true, "Session closed and unmarked students set to Absent"};
    }

    // TC09, TC10
    ActionResult updateAttendanceManual(const std::string& course_id, int session_num,
                                        const std::string& student_id, const std::string& new_status) {
        auto it = sessions_.find(key(course_id, session_num));
        if (it == sessions_.end()) {
            return {false, "Session not found"};
        }
        for (auto& rec : it->second.records) {
            if (rec.student_id == student_id) {
                rec.status = new_status;
                return {true, "Attendance updated"};
            }
        }
        return {false, "Invalid student ID"};
    }

    // TC11, TC12
    ActionResult recordCheckInAtTime(const std::string& course_id, int session_num,
                                     const std::string& student_id, const std::string& checkin_hhmm) {
        auto it = sessions_.find(key(course_id, session_num));
        if (it == sessions_.end()) {
            return {false, "Session not found"};
        }
        int actual_min = toMinutes(checkin_hhmm);
        int end_min = toMinutes(it->second.checkin_end);
        std::string eval_status = (actual_min <= end_min) ? "Present" : "Late";

        for (auto& rec : it->second.records) {
            if (rec.student_id == student_id) {
                rec.status = eval_status;
                return {true, eval_status};
            }
        }
        return {false, "Student not found"};
    }

    // TC13, TC14, TC15
    std::vector<AttendanceRecord> searchStudents(const std::string& course_id,
                                                 const std::string& name_query,
                                                 const std::string& id_query) const {
        auto it = sessions_.find(key(course_id, 12));
        if (it == sessions_.end()) return {};

        std::vector<AttendanceRecord> out;
        for (const auto& rec : it->second.records) {
            bool match_name = name_query.empty() ||
                              rec.student_name.find(name_query) != std::string::npos;
            bool match_id = id_query.empty() || rec.student_id == id_query;
            if (match_name && match_id) {
                out.push_back(rec);
            }
        }
        return out;
    }

    // TC16
    ReportSummary getSessionSummary(const std::string& course_id, int session_num) const {
        ReportSummary summary;
        auto it = sessions_.find(key(course_id, session_num));
        if (it == sessions_.end()) return summary;

        summary.total = static_cast<int>(it->second.records.size());
        for (const auto& rec : it->second.records) {
            if (rec.status == "Present") ++summary.present;
            else if (rec.status == "Late") ++summary.late;
            else if (rec.status == "Absent") ++summary.absent;
        }
        if (summary.total > 0) {
            int attended = summary.present + summary.late;
            summary.attendance_rate = (static_cast<double>(attended) / summary.total) * 100.0;
        }
        return summary;
    }

    // TC17
    double getStudentRateForCompletedSessions(int attended_sessions, int completed_sessions) const {
        if (completed_sessions <= 0) {
            return 0.0;
        }
        return (static_cast<double>(attended_sessions) / completed_sessions) * 100.0;
    }

    const ClassSession* getSession(const std::string& course_id, int session_num) const {
        auto it = sessions_.find(key(course_id, session_num));
        return (it == sessions_.end()) ? nullptr : &it->second;
    }

private:
    static std::string key(const std::string& course_id, int session_num) {
        return course_id + "#" + std::to_string(session_num);
    }

    static int toMinutes(const std::string& hhmm) {
        int h = std::stoi(hhmm.substr(0, 2));
        int m = std::stoi(hhmm.substr(3, 2));
        return h * 60 + m;
    }

    std::map<std::string, User> users_;
    std::set<std::string> assigned_courses_;
    std::map<std::string, ClassSession> sessions_;
};

class LecturerTestSuite : public ::testing::Test {
protected:
    LecturerAttendanceService svc;
};

// TC01 & TC02: Login
TEST_F(LecturerTestSuite, TC01_LoginValidCredentials) {
    auto res = svc.login("student01", "123456");
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(res.message, "User logs in successfully");
}

TEST_F(LecturerTestSuite, TC02_LoginWrongPassword) {
    auto res = svc.login("student01", "wrongpass");
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(res.message, "Invalid username or password.");
}

// TC03: Register Account
TEST_F(LecturerTestSuite, TC03_RegisterAccount) {
    LecturerAttendanceService fresh_svc;
    auto res = fresh_svc.registerAccount("John", "john@gmail.com", "john_new", "123456");
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(res.message, "Registration is completed successfully");
}

// TC04 & TC05: Create Class Session
TEST_F(LecturerTestSuite, TC04_CreateSessionAssignedCourse) {
    auto res = svc.createClassSession("CS201", 16, "02 Dec 2026", "09:00 - 11:00", "09:00 - 09:15");
    EXPECT_TRUE(res.ok);
    const auto* s = svc.getSession("CS201", 16);
    ASSERT_NE(s, nullptr);
    EXPECT_EQ(s->status, "Scheduled");
    EXPECT_EQ(s->auto_close_time, "09:30");
}

TEST_F(LecturerTestSuite, TC05_CreateSessionUnassignedCourseRejected) {
    auto res = svc.createClassSession("CS305", 1, "02 Dec 2026", "09:00 - 11:00", "09:00 - 09:15");
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(svc.getSession("CS305", 1), nullptr);
}

// TC06, TC07, TC08: Close Session & Auto-Mark Absent
TEST_F(LecturerTestSuite, TC06_CloseOpenSessionMarksAbsent) {
    auto res = svc.closeSession("CS201", 13);
    EXPECT_TRUE(res.ok);
    const auto* s = svc.getSession("CS201", 13);
    ASSERT_NE(s, nullptr);
    EXPECT_EQ(s->status, "Closed");

    auto find_status = [&](const std::string& id) {
        for (const auto& r : s->records) if (r.student_id == id) return r.status;
        return std::string{};
    };
    EXPECT_EQ(find_status("SV2024002"), "Absent");
    EXPECT_EQ(find_status("SV2024003"), "Absent");
    EXPECT_EQ(find_status("SV2024004"), "Absent");
}

TEST_F(LecturerTestSuite, TC07_CloseNonExistentSession) {
    auto res = svc.closeSession("CS201", 99);
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(res.message, "Session not found");
}

TEST_F(LecturerTestSuite, TC08_CloseAlreadyClosedSessionPreservesRecords) {
    auto res = svc.closeSession("CS201", 12);
    EXPECT_TRUE(res.ok);
    const auto* s = svc.getSession("CS201", 12);
    ASSERT_NE(s, nullptr);
    EXPECT_EQ(s->status, "Closed");
    EXPECT_EQ(s->records[0].status, "Present"); // SV2024001
    EXPECT_EQ(s->records[1].status, "Present"); // SV2024002
    EXPECT_EQ(s->records[2].status, "Late");    // SV2024003
}

// TC09, TC10, TC11, TC12: Manual Attendance Recording & Check-in Boundary
TEST_F(LecturerTestSuite, TC09_ManualUpdateStudentToPresent) {
    auto res = svc.updateAttendanceManual("CS201", 12, "SV2024004", "Present");
    EXPECT_TRUE(res.ok);
    const auto* s = svc.getSession("CS201", 12);
    EXPECT_EQ(s->records[3].status, "Present");
}

TEST_F(LecturerTestSuite, TC10_ManualUpdateInvalidStudentRejected) {
    auto res = svc.updateAttendanceManual("CS201", 12, "SV9999999", "Present");
    EXPECT_FALSE(res.ok);
    const auto* s = svc.getSession("CS201", 12);
    EXPECT_EQ(s->records[3].status, "Absent");
}

TEST_F(LecturerTestSuite, TC11_CheckInExactBoundaryIsPresent) {
    auto res = svc.recordCheckInAtTime("CS201", 13, "SV2024002", "09:15");
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(res.message, "Present");
}

TEST_F(LecturerTestSuite, TC12_CheckInOneMinuteAfterEndIsLate) {
    auto res = svc.recordCheckInAtTime("CS201", 13, "SV2024003", "09:16");
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(res.message, "Late");
}

// TC13, TC14, TC15: Student Search & Filtering
TEST_F(LecturerTestSuite, TC13_SearchByNameKeyword) {
    auto rows = svc.searchStudents("CS201", "Minh Anh", "");
    ASSERT_EQ(rows.size(), 1u);
    EXPECT_EQ(rows[0].student_id, "SV2024001");
    EXPECT_EQ(rows[0].student_name, "Nguyễn Minh Anh");
}

TEST_F(LecturerTestSuite, TC14_SearchNonExistentStudentId) {
    auto rows = svc.searchStudents("CS201", "", "SV8888888");
    EXPECT_TRUE(rows.empty());
}

TEST_F(LecturerTestSuite, TC15_SearchWithEmptyFiltersReturnsAllEnrolled) {
    auto rows = svc.searchStudents("CS201", "", "");
    EXPECT_EQ(rows.size(), 4u);
}

// TC16 & TC17: View Statistics & Export Report
TEST_F(LecturerTestSuite, TC16_Session12SummaryMetrics) {
    auto rep = svc.getSessionSummary("CS201", 12);
    EXPECT_EQ(rep.total, 4);
    EXPECT_EQ(rep.present, 2);
    EXPECT_EQ(rep.late, 1);
    EXPECT_EQ(rep.absent, 1);
    EXPECT_NEAR(rep.attendance_rate, 75.0, 1e-6);
}

TEST_F(LecturerTestSuite, TC17_ZeroCompletedSessionsSafeRate) {
    double rate = svc.getStudentRateForCompletedSessions(0, 0);
    EXPECT_DOUBLE_EQ(rate, 0.0);
}

} // namespace attendance_test