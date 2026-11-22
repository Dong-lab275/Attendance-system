#include <gtest/gtest.h>

#include <algorithm>
#include <cctype>
#include <cmath>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace attendance_full_test {

struct Result {
    bool ok = false;
    std::string message;
};

struct UserAccount {
    std::string username;
    std::string password;
    std::string full_name;
    std::string email;
    std::string phone;
    std::string address;
    std::string role;
    std::string status = "Active";
    bool has_academic_records = false;
};

struct CourseRecord {
    std::string id;
    std::string name;
    int credits = 3;
    bool assigned_to_lecturer = false;
    bool open_for_registration = true;
    bool has_related_records = false;
};

struct SessionRecord {
    std::string id;
    std::string course_id;
    int session_number = 0;
    std::string date;
    std::string class_time;
    std::string checkin_start;
    std::string checkin_end;
    std::string auto_close_time;
    std::string room;
    std::string status = "Open"; // "Scheduled", "Open", "Closed"
};

struct AttendanceEntry {
    std::string student_id;
    std::string student_name;
    std::string course_id;
    int session_number = 0;
    std::string session_id;
    std::string date;
    std::string status; // "Present", "Late", "Absent", "Excused"
    std::string checkin_time;
};

struct AdminCourseItem {
    std::string code;
    std::string name;
    int credits = 0;
    bool open_for_registration = true;
    bool has_related_records = false;
};

struct AdminRegistrationItem {
    std::string student_id;
    std::string course_code;
};

struct AdminAssignmentItem {
    std::string course_code;
    std::string lecturer_id;
};

struct DepartmentItem {
    std::string code;
    std::string name;
};

struct AdminClassSessionItem {
    std::string session_id;
    std::string course_code;
    std::string date;
    std::string start_time;
    std::string end_time;
    std::string room;
};

struct AttendanceReportRow {
    std::string student_id;
    std::string course_code;
    std::string date;
    std::string status;
};

struct ChangeHistoryItem {
    std::string student_id;
    std::string session_id;
    std::string previous_status;
    std::string new_status;
    std::string timestamp;
};

struct RateSummary {
    double rate = 0.0;
    bool division_by_zero_safe = true;
    bool empty_state = false;
    std::string message;
};

class AttendanceSystemEngine {
public:
    AttendanceSystemEngine() {
        users_["student01"] = {"student01", "123456", "Student One", "student01@attendance.edu.vn", "0900000001", "District 1, HCMC", "Student", "Active", true};
        users_["ST001"] = {"ST001", "pass001", "Tran Minh ST001", "st001@attendance.edu.vn", "0901111111", "District 5, HCMC", "Student", "Active", true};
        users_["ST002"] = {"ST002", "pass002", "Le Hoang ST002", "st002@attendance.edu.vn", "0902222222", "District 10, HCMC", "Student", "Active", false};
        users_["SV2024001"] = {"SV2024001", "sv2024001", "Nguyễn Minh Anh", "sv2024001@attendance.edu.vn", "0910000001", "HCMC", "Student", "Active", true};
        users_["SV2024002"] = {"SV2024002", "sv2024002", "Pham Bao SV2", "sv2024002@attendance.edu.vn", "0910000002", "HCMC", "Student", "Active", true};
        users_["SV2024003"] = {"SV2024003", "sv2024003", "Do Gia SV3", "sv2024003@attendance.edu.vn", "0910000003", "HCMC", "Student", "Active", true};
        users_["SV2024004"] = {"SV2024004", "sv2024004", "Vo Lan SV4", "sv2024004@attendance.edu.vn", "0910000004", "HCMC", "Student", "Active", true};
        users_["SV2024005"] = {"SV2024005", "sv2024005", "Hoang Nam SV5", "sv2024005@attendance.edu.vn", "0910000005", "HCMC", "Student", "Active", false};

        admin_accounts_["khang.le"] = {"khang.le", "Admin@123", "Le Minh Khang", "khang.le@attendance.edu.vn", "", "", "Administrator", "Active", false};
        admin_accounts_["student01"] = {"student01", "123456", "Nguyen Van Student", "student01@attendance.edu.vn", "", "", "Student", "Active", true};
        admin_accounts_["test.student01"] = {"test.student01", "Pass@123", "Test Student One", "test.student01@attendance.edu.vn", "", "", "Student", "Active", false};
        admin_accounts_["test.student02"] = {"test.student02", "Pass@123", "Test Student Two", "test.student02@attendance.edu.vn", "", "", "Student", "Active", false};
        admin_accounts_["012345678902"] = {"012345678902", "Lec@123", "Lecturer Assigned", "lec02@attendance.edu.vn", "", "", "Lecturer", "Active", true};

        courses_["CS201"] = {"CS201", "Object-Oriented Programming", 3, true, true, true};
        courses_["CS202"] = {"CS202", "Database Systems", 3, true, true, false};
        courses_["CS305"] = {"CS305", "Distributed Systems", 3, false, true, false};
        courses_["CS204"] = {"CS204", "Operating Systems", 3, true, true, false};
        courses_["PHI205"] = {"PHI205", "Philosophy of Science", 2, true, true, true};

        admin_courses_["CS101"] = {"CS101", "Introduction to Programming", 3, true, true};
        admin_courses_["CS303"] = {"CS303", "Discrete Structures", 3, true, false};
        admin_courses_["CS999"] = {"CS999", "Closed Elective", 3, false, false};

        sessions_.push_back({"S_CS201_12", "CS201", 12, "2026-11-25", "09:00 - 11:00", "09:00", "09:15", "09:30", "B201", "Closed"});
        sessions_.push_back({"S_CS201_13", "CS201", 13, "2026-12-02", "09:00 - 11:00", "09:00", "09:15", "09:30", "B201", "Open"});

        admin_sessions_.push_back({"202612010001", "CS101", "2026-12-01", "08:00", "10:00", "A101"});

        enrolled_by_course_["CS201"] = {"SV2024001", "SV2024002", "SV2024003", "SV2024004"};
        student_registrations_["ST001"].insert("PHI205");

        attendance_.push_back({"SV2024001", "Nguyễn Minh Anh", "CS201", 12, "S_CS201_12", "2026-11-25", "Present", "09:02"});
        attendance_.push_back({"SV2024002", "Pham Bao SV2", "CS201", 12, "S_CS201_12", "2026-11-25", "Present", "09:05"});
        attendance_.push_back({"SV2024003", "Do Gia SV3", "CS201", 12, "S_CS201_12", "2026-11-25", "Late", "09:16"});
        attendance_.push_back({"SV2024004", "Vo Lan SV4", "CS201", 12, "S_CS201_12", "2026-11-25", "Absent", ""});

        attendance_.push_back({"ST001", "Tran Minh ST001", "PHI205", 1, "S_PHI205_1", "2026-11-18", "Present", "08:01"});

        reports_.push_back({"012345678901", "CS101", "2026-12-01", "Present"});
        reports_.push_back({"012345678901", "CS101", "2026-12-15", "Late"});

        history_.push_back({"012345678901", "202612010001", "Absent", "Present", "2026-12-01 08:05:00"});
    }

    static bool isValidEmail(const std::string& email) {
        auto at_pos = email.find('@');
        if (at_pos == std::string::npos || at_pos == 0) {
            return false;
        }
        auto dot_pos = email.find('.', at_pos + 1);
        return dot_pos != std::string::npos && dot_pos + 1 < email.size();
    }

    static bool isBlankOrWhitespace(const std::string& s) {
        if (s.empty()) {
            return true;
        }
        return std::all_of(s.begin(), s.end(), [](unsigned char ch) {
            return std::isspace(ch) != 0;
        });
    }

    static std::string toLower(const std::string& s) {
        std::string out = s;
        for (char& c : out) {
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        }
        return out;
    }

    static int timeToMinutes(const std::string& hhmm) {
        if (hhmm.size() != 5 || hhmm[2] != ':') {
            return -1;
        }
        try {
            int h = std::stoi(hhmm.substr(0, 2));
            int m = std::stoi(hhmm.substr(3, 2));
            if (h < 0 || h > 23 || m < 0 || m > 59) {
                return -1;
            }
            return h * 60 + m;
        } catch (...) {
            return -1;
        }
    }

    static std::string minutesToTime(int mins) {
        int wrapped = ((mins % 1440) + 1440) % 1440;
        int h = wrapped / 60;
        int m = wrapped % 60;
        std::string hh = (h < 10 ? "0" : "") + std::to_string(h);
        std::string mm = (m < 10 ? "0" : "") + std::to_string(m);
        return hh + ":" + mm;
    }

    // TC01, TC02
    Result login(const std::string& username, const std::string& password) const {
        auto it = users_.find(username);
        if (it == users_.end() || it->second.password != password) {
            return {false, "Invalid username or password."};
        }
        return {true, "User logs in successfully"};
    }

    // TC03
    Result registerAccount(const std::string& name,
                           const std::string& email,
                           const std::string& username,
                           const std::string& password) {
        if (isBlankOrWhitespace(name) || isBlankOrWhitespace(email) ||
            isBlankOrWhitespace(username) || isBlankOrWhitespace(password)) {
            return {false, "Required fields are missing."};
        }
        users_[username] = {username, password, name, email, "", "", "Student", "Active", false};
        return {true, "Registration is completed successfully"};
    }

    // TC04, TC05
    Result createLecturerSession(const std::string& course_id,
                                 int session_number,
                                 const std::string& date,
                                 const std::string& class_time,
                                 const std::string& checkin_start,
                                 const std::string& checkin_end,
                                 std::string* out_status = nullptr,
                                 std::string* out_auto_close = nullptr) {
        auto c_it = courses_.find(course_id);
        if (c_it == courses_.end() || !c_it->second.assigned_to_lecturer) {
            return {false, "Session creation is rejected because the course is not assigned to the lecturer."};
        }
        int end_min = timeToMinutes(checkin_end);
        std::string auto_close = (end_min >= 0) ? minutesToTime(end_min + 15) : "09:30";
        SessionRecord rec{
            "S_" + course_id + "_" + std::to_string(session_number),
            course_id,
            session_number,
            date,
            class_time,
            checkin_start,
            checkin_end,
            auto_close,
            "B201",
            "Scheduled"
        };
        sessions_.push_back(rec);
        if (out_status) {
            *out_status = rec.status;
        }
        if (out_auto_close) {
            *out_auto_close = rec.auto_close_time;
        }
        return {true, "Session is created successfully with status 'Scheduled'"};
    }

    // TC06, TC07, TC08
    Result closeSession(const std::string& course_id, int session_number) {
        for (auto& s : sessions_) {
            if (s.course_id == course_id && s.session_number == session_number) {
                if (s.status == "Closed") {
                    return {true, "Session remains 'Closed' and existing records are preserved."};
                }
                s.status = "Closed";
                auto enrolled_it = enrolled_by_course_.find(course_id);
                if (enrolled_it != enrolled_by_course_.end()) {
                    for (const auto& sid : enrolled_it->second) {
                        bool found = false;
                        for (auto& a : attendance_) {
                            if (a.course_id == course_id && a.session_number == session_number && a.student_id == sid) {
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            std::string sname = users_.count(sid) ? users_[sid].full_name : sid;
                            attendance_.push_back({sid, sname, course_id, session_number, s.id, s.date, "Absent", ""});
                        }
                    }
                }
                return {true, "Session status changes to 'Closed'"};
            }
        }
        return {false, "Session not found"};
    }

    // TC09, TC10
    Result manualUpdateAttendance(const std::string& course_id,
                                  int session_number,
                                  const std::string& student_id,
                                  const std::string& new_status) {
        if (users_.find(student_id) == users_.end()) {
            return {false, "Invalid student ID."};
        }
        for (auto& a : attendance_) {
            if (a.course_id == course_id && a.session_number == session_number && a.student_id == student_id) {
                a.status = new_status;
                return {true, "Student attendance status is updated successfully"};
            }
        }
        std::string sname = users_[student_id].full_name;
        attendance_.push_back({student_id, sname, course_id, session_number, "S_" + course_id + "_" + std::to_string(session_number), "2026-11-25", new_status, ""});
        return {true, "Student attendance status is updated successfully"};
    }

    // TC11, TC12
    Result recordCheckInByBoundary(const std::string& course_id,
                                   int session_number,
                                   const std::string& student_id,
                                   const std::string& checkin_time,
                                   const std::string& checkin_end,
                                   std::string* out_evaluated_status = nullptr) {
        int actual_m = timeToMinutes(checkin_time);
        int end_m = timeToMinutes(checkin_end);
        std::string eval_status = (actual_m <= end_m) ? "Present" : "Late";
        std::string sname = users_.count(student_id) ? users_[student_id].full_name : student_id;
        attendance_.push_back({student_id, sname, course_id, session_number, "S_" + course_id + "_" + std::to_string(session_number), "2026-12-02", eval_status, checkin_time});
        if (out_evaluated_status) {
            *out_evaluated_status = eval_status;
        }
        return {true, eval_status};
    }

    // TC13, TC14, TC15
    Result searchStudentsInCourse(const std::string& course_id,
                                  const std::string& name_keyword,
                                  const std::string& student_id_filter,
                                  std::vector<std::string>& out_matches) const {
        out_matches.clear();
        auto it = enrolled_by_course_.find(course_id);
        if (it == enrolled_by_course_.end()) {
            return {false, "No matching students found"};
        }
        std::string lower_name = toLower(name_keyword);
        for (const auto& sid : it->second) {
            std::string full_name = users_.count(sid) ? users_.at(sid).full_name : sid;
            bool match_name = name_keyword.empty() || (toLower(full_name).find(lower_name) != std::string::npos);
            bool match_id = student_id_filter.empty() || (sid == student_id_filter);
            if (match_name && match_id) {
                out_matches.push_back(full_name + " (" + sid + ")");
            }
        }
        if (out_matches.empty()) {
            return {false, "No matching students found"};
        }
        return {true, "Matching students found"};
    }

    // TC16
    void getSessionSummary(const std::string& course_id,
                           int session_number,
                           int& total,
                           int& present,
                           int& late,
                           int& absent,
                           double& rate) const {
        total = 0;
        present = 0;
        late = 0;
        absent = 0;
        for (const auto& a : attendance_) {
            if (a.course_id == course_id && a.session_number == session_number) {
                ++total;
                if (a.status == "Present") {
                    ++present;
                } else if (a.status == "Late") {
                    ++late;
                } else if (a.status == "Absent") {
                    ++absent;
                }
            }
        }
        rate = (total > 0) ? (static_cast<double>(present + late) / static_cast<double>(total)) * 100.0 : 0.0;
    }

    // TC17
    std::pair<double, double> getStudentRatesSafe(const std::string& course_id,
                                                   const std::string& student_id) const {
        int completed = 0;
        int attended = 0;
        int on_time = 0;
        for (const auto& a : attendance_) {
            if (a.course_id == course_id && a.student_id == student_id) {
                ++completed;
                if (a.status == "Present" || a.status == "Late") {
                    ++attended;
                }
                if (a.status == "Present") {
                    ++on_time;
                }
            }
        }
        if (completed == 0) {
            return {0.0, 0.0};
        }
        double att_rate = (static_cast<double>(attended) / static_cast<double>(completed)) * 100.0;
        double ontime_rate = (static_cast<double>(on_time) / static_cast<double>(completed)) * 100.0;
        return {att_rate, ontime_rate};
    }

    // TC18, TC19, TC20
    Result forgotPassword(const std::string& email) const {
        if (!isValidEmail(email)) {
            return {false, "Invalid email format."};
        }
        for (const auto& kv : admin_accounts_) {
            if (kv.second.email == email) {
                return {true, "Password-reset instructions have been sent to the registered email address."};
            }
        }
        return {false, "If the email is registered, reset instructions have been sent."};
    }

    // TC21, TC22, TC23
    Result changePassword(const std::string& username,
                          const std::string& current_password,
                          const std::string& new_password,
                          const std::string& confirm_password) {
        auto it = admin_accounts_.find(username);
        if (it == admin_accounts_.end() || it->second.password != current_password) {
            return {false, "Current password is incorrect."};
        }
        if (new_password != confirm_password) {
            return {false, "Passwords do not match."};
        }
        it->second.password = new_password;
        return {true, "Password updated successfully."};
    }

    // TC24, TC25, TC26
    Result createAdminAccount(const std::string& full_name,
                              const std::string& username,
                              const std::string& email,
                              const std::string& role,
                              const std::string& password) {
        if (isBlankOrWhitespace(full_name) || isBlankOrWhitespace(username) ||
            isBlankOrWhitespace(email) || isBlankOrWhitespace(role) || isBlankOrWhitespace(password)) {
            return {false, "Required field is empty."};
        }
        if (admin_accounts_.count(username) > 0) {
            return {false, "Username already exists."};
        }
        admin_accounts_[username] = {username, password, full_name, email, "", "", role, "Active", false};
        return {true, "Account created successfully."};
    }

    // TC27, TC28, TC29
    std::vector<UserAccount> searchAdminAccounts(const std::string& keyword) const {
        std::vector<UserAccount> out;
        for (const auto& kv : admin_accounts_) {
            if (keyword.empty() || kv.first.find(keyword) != std::string::npos) {
                out.push_back(kv.second);
            }
        }
        return out;
    }

    // TC30, TC31, TC32
    Result updateAdminAccount(const std::string& username,
                              const std::string& new_full_name,
                              const std::string& new_email) {
        if (isBlankOrWhitespace(username) || isBlankOrWhitespace(new_full_name)) {
            return {false, "Required field is empty."};
        }
        if (!isValidEmail(new_email)) {
            return {false, "Invalid email format."};
        }
        auto it = admin_accounts_.find(username);
        if (it == admin_accounts_.end()) {
            return {false, "Account not found."};
        }
        it->second.full_name = new_full_name;
        it->second.email = new_email;
        return {true, "Account updated successfully."};
    }

    // TC33, TC34, TC35
    Result deleteAdminAccount(const std::string& username, bool confirm) {
        if (!confirm) {
            return {false, "Deletion cancelled."};
        }
        auto it = admin_accounts_.find(username);
        if (it == admin_accounts_.end()) {
            return {false, "Account not found."};
        }
        if (it->second.has_academic_records) {
            it->second.status = "Inactive";
            return {false, "Cannot delete account with related academic records; deactivated instead."};
        }
        admin_accounts_.erase(it);
        return {true, "Account deleted successfully."};
    }

    // TC36, TC37, TC38
    Result createAdminCourse(const std::string& code,
                             const std::string& name,
                             int credits) {
        if (admin_courses_.count(code) > 0) {
            return {false, "Course code already exists."};
        }
        if (credits <= 0) {
            return {false, "Credits must be a positive number."};
        }
        if (isBlankOrWhitespace(code) || isBlankOrWhitespace(name)) {
            return {false, "Required field is empty."};
        }
        admin_courses_[code] = {code, name, credits, true, false};
        return {true, "Course created successfully."};
    }

    // TC39, TC40, TC41
    Result updateAdminCourse(const std::string& code, const std::string& new_name) {
        auto it = admin_courses_.find(code);
        if (it == admin_courses_.end()) {
            return {false, "Course not found."};
        }
        if (isBlankOrWhitespace(new_name)) {
            return {false, "Course name is required."};
        }
        it->second.name = new_name;
        return {true, "Course updated successfully."};
    }

    // TC42, TC43, TC44
    Result deleteAdminCourse(const std::string& code, bool confirm) {
        if (!confirm) {
            return {false, "Deletion cancelled."};
        }
        auto it = admin_courses_.find(code);
        if (it == admin_courses_.end()) {
            return {false, "Course not found."};
        }
        if (it->second.has_related_records) {
            return {false, "Cannot delete course because it has related registrations and sessions."};
        }
        admin_courses_.erase(it);
        return {true, "Course deleted successfully."};
    }

    // TC45, TC46, TC47
    Result addAdminCourseRegistration(const std::string& student_id,
                                      const std::string& course_code) {
        auto it = admin_courses_.find(course_code);
        if (it == admin_courses_.end() || !it->second.open_for_registration) {
            return {false, "Course is closed or unavailable for registration."};
        }
        for (const auto& r : admin_registrations_) {
            if (r.student_id == student_id && r.course_code == course_code) {
                return {false, "Duplicate course registration is not allowed."};
            }
        }
        admin_registrations_.push_back({student_id, course_code});
        it->second.has_related_records = true;
        return {true, "Course registration created successfully."};
    }

    // TC48, TC49, TC50
    Result assignLecturer(const std::string& course_code,
                          const std::string& lecturer_id) {
        if (admin_courses_.find(course_code) == admin_courses_.end()) {
            return {false, "Course not found."};
        }
        auto acc_it = admin_accounts_.find(lecturer_id);
        if (acc_it == admin_accounts_.end() || acc_it->second.role != "Lecturer" || acc_it->second.status != "Active") {
            return {false, "Lecturer does not exist or is inactive."};
        }
        for (const auto& item : admin_assignments_) {
            if (item.course_code == course_code && item.lecturer_id == lecturer_id) {
                return {false, "Duplicate lecturer assignment is not allowed."};
            }
        }
        admin_assignments_.push_back({course_code, lecturer_id});
        return {true, "Lecturer assigned to course successfully."};
    }

    // TC51, TC52, TC53
    Result createDepartment(const std::string& code, const std::string& name) {
        if (departments_.count(code) > 0) {
            return {false, "Department code already exists."};
        }
        if (isBlankOrWhitespace(name)) {
            return {false, "Invalid department name."};
        }
        departments_[code] = {code, name};
        return {true, "Department created successfully."};
    }

    // TC54, TC55, TC56
    Result createAdminClassSession(const std::string& course_code,
                                   const std::string& date,
                                   const std::string& start_time,
                                   const std::string& end_time,
                                   const std::string& room) {
        if (admin_courses_.find(course_code) == admin_courses_.end()) {
            return {false, "Course not found."};
        }
        int s_min = timeToMinutes(start_time);
        int e_min = timeToMinutes(end_time);
        if (s_min < 0 || e_min < 0 || e_min <= s_min) {
            return {false, "End time must be later than start time."};
        }
        for (const auto& s : admin_sessions_) {
            if (s.room == room && s.date == date) {
                int existing_s = timeToMinutes(s.start_time);
                int existing_e = timeToMinutes(s.end_time);
                if (s_min < existing_e && e_min > existing_s) {
                    return {false, "Schedule conflict in the same room."};
                }
            }
        }
        std::string new_id = date + "_" + room + "_" + std::to_string(admin_sessions_.size() + 1);
        admin_sessions_.push_back({new_id, course_code, date, start_time, end_time, room});
        return {true, "Class session created successfully."};
    }

    // TC57, TC58, TC59
    Result viewAttendanceReports(const std::string& course_code,
                               const std::string& from_date,
                               const std::string& to_date,
                               std::vector<AttendanceReportRow>& out_rows) const {
        out_rows.clear();
        if (from_date > to_date) {
            return {false, "From date must not be later than to date."};
        }
        for (const auto& r : reports_) {
            if (r.course_code == course_code && r.date >= from_date && r.date <= to_date) {
                out_rows.push_back(r);
            }
        }
        if (out_rows.empty()) {
            return {true, "No attendance records found."};
        }
        return {true, "Attendance report generated successfully."};
    }

    // TC60, TC61, TC62
    Result searchChangeHistoryById(const std::string& id_filter,
                                   std::vector<ChangeHistoryItem>& out_items) const {
        out_items.clear();
        for (const auto& h : history_) {
            if (h.student_id == id_filter || h.session_id == id_filter) {
                out_items.push_back(h);
            }
        }
        if (out_items.empty()) {
            return {false, "No records found."};
        }
        return {true, "Change history records found."};
    }

    Result searchChangeHistoryByDateRange(const std::string& from_date,
                                        const std::string& to_date,
                                        std::vector<ChangeHistoryItem>& out_items) const {
        out_items.clear();
        if (from_date > to_date) {
            return {false, "Invalid date range: start date is later than end date."};
        }
        for (const auto& h : history_) {
            std::string d = h.timestamp.substr(0, 10);
            if (d >= from_date && d <= to_date) {
                out_items.push_back(h);
            }
        }
        return {true, "Change history filtered by date range."};
    }

    // TC63, TC64, TC65
    Result viewStudentProfile(const std::string& student_id,
                              bool backend_available,
                              UserAccount* out_profile = nullptr) const {
        if (!backend_available) {
            return {false, "Backend unavailable: failed to load student profile."};
        }
        auto it = users_.find(student_id);
        if (it == users_.end()) {
            return {false, "Student not found."};
        }
        if (out_profile) {
            *out_profile = it->second;
        }
        return {true, "Student profile loaded."};
    }

    // TC66, TC67, TC68, TC69
    Result updateStudentProfile(const std::string& student_id,
                                const std::string& full_name,
                                const std::string& email,
                                const std::string& phone,
                                const std::string& address) {
        auto it = users_.find(student_id);
        if (it == users_.end()) {
            return {false, "Student not found."};
        }
        if (isBlankOrWhitespace(full_name)) {
            return {false, "Full name is required."};
        }
        if (!isValidEmail(email)) {
            return {false, "Invalid email format."};
        }
        it->second.full_name = full_name;
        it->second.email = email;
        it->second.phone = phone;
        it->second.address = address;
        return {true, "Student profile updated successfully."};
    }

    // TC70..TC76
    Result registerStudentCourse(const std::string& student_id,
                                 const std::string& course_id,
                                 const std::string& current_date = "2026-11-15",
                                 const std::string& registration_end = "2026-11-20") {
        if (current_date > registration_end) {
            return {false, "Registration period has ended."};
        }
        if (courses_.find(course_id) == courses_.end()) {
            return {false, "Invalid or unavailable course ID."};
        }
        auto& reg_set = student_registrations_[student_id];
        if (reg_set.count(course_id) > 0) {
            return {false, "Course is already registered."};
        }
        reg_set.insert(course_id);
        return {true, "Course registered successfully."};
    }

    std::vector<std::string> getRegisteredCourses(const std::string& student_id,
                                                  std::string* out_empty_message = nullptr) const {
        std::vector<std::string> list;
        auto it = student_registrations_.find(student_id);
        if (it == student_registrations_.end() || it->second.empty()) {
            if (out_empty_message) {
                *out_empty_message = "No registered courses found.";
            }
            return list;
        }
        for (const auto& cid : it->second) {
            list.push_back(cid);
        }
        return list;
    }

    // TC77, TC78, TC79
    Result unregisterStudentCourse(const std::string& student_id,
                                   const std::string& course_id,
                                   bool confirmed = true) {
        if (!confirmed) {
            return {false, "Unregistration cancelled."};
        }
        auto it = student_registrations_.find(student_id);
        if (it == student_registrations_.end() || it->second.erase(course_id) == 0) {
            return {false, "Course registration does not exist."};
        }
        return {true, "Course unregistered successfully."};
    }

    // TC80..TC85
    Result studentCheckIn(const std::string& student_id,
                          const std::string& course_id,
                          const std::string& session_id,
                          bool gps_permission_granted,
                          bool within_gps_radius,
                          bool within_allowed_time,
                          const std::string& session_status) {
        if (!gps_permission_granted) {
            return {false, "Location permission denied."};
        }
        if (!within_gps_radius) {
            return {false, "Outside permitted GPS radius."};
        }
        if (session_status == "CLOSED") {
            return {false, "Session is closed."};
        }
        if (!within_allowed_time) {
            return {false, "Outside allowed check-in time."};
        }
        auto reg_it = student_registrations_.find(student_id);
        if (reg_it == student_registrations_.end() || reg_it->second.count(course_id) == 0) {
            return {false, "Course is not registered."};
        }
        for (const auto& a : attendance_) {
            if (a.student_id == student_id && a.session_id == session_id) {
                return {false, "Duplicate attendance is prevented."};
            }
        }
        attendance_.push_back({student_id, users_[student_id].full_name, course_id, 1, session_id, "2026-11-22", "Present", "08:05"});
        return {true, "Attendance recorded once."};
    }

    // TC86, TC87, TC88
    Result getStudentAttendanceHistory(const std::string& requester_id,
                                     const std::string& target_student_id,
                                     std::vector<AttendanceEntry>& out_records) const {
        out_records.clear();
        if (requester_id != target_student_id) {
            return {false, "Access denied: cannot view another student's attendance records."};
        }
        for (const auto& a : attendance_) {
            if (a.student_id == target_student_id) {
                out_records.push_back(a);
            }
        }
        if (out_records.empty()) {
            return {true, "No attendance records found for this student."};
        }
        return {true, "Attendance history retrieved."};
    }

    // TC89, TC90, TC91
    static RateSummary calculateAttendanceRate(int attended, int total_sessions) {
        if (total_sessions <= 0) {
            return {0.0, true, true, "No scheduled sessions; attendance rate is 0.0%."};
        }
        double pct = (static_cast<double>(attended) / static_cast<double>(total_sessions)) * 100.0;
        return {pct, true, false, "Calculated attendance rate."};
    }

    static RateSummary calculatePolicyAttendanceRate(int present, int absent, int excused) {
        int total = present + absent + excused;
        if (total <= 0) {
            return {0.0, true, true, "No sessions."};
        }
        // Policy: Present + Excused count toward attendance over total sessions
        double pct = (static_cast<double>(present + excused) / static_cast<double>(total)) * 100.0;
        return {pct, true, false, "Policy attendance rate calculated."};
    }

    bool hasAdminCourse(const std::string& code) const {
        return admin_courses_.count(code) > 0;
    }

    std::string getAdminCourseName(const std::string& code) const {
        auto it = admin_courses_.find(code);
        return (it != admin_courses_.end()) ? it->second.name : "";
    }

    std::string getAdminAccountFullName(const std::string& username) const {
        auto it = admin_accounts_.find(username);
        return (it != admin_accounts_.end()) ? it->second.full_name : "";
    }

    std::string getAdminAccountStatus(const std::string& username) const {
        auto it = admin_accounts_.find(username);
        return (it != admin_accounts_.end()) ? it->second.status : "";
    }

    std::string getAdminAccountPassword(const std::string& username) const {
        auto it = admin_accounts_.find(username);
        return (it != admin_accounts_.end()) ? it->second.password : "";
    }

private:
    std::map<std::string, UserAccount> users_;
    std::map<std::string, UserAccount> admin_accounts_;
    std::map<std::string, CourseRecord> courses_;
    std::map<std::string, AdminCourseItem> admin_courses_;
    std::vector<SessionRecord> sessions_;
    std::vector<AdminClassSessionItem> admin_sessions_;
    std::map<std::string, std::vector<std::string>> enrolled_by_course_;
    std::map<std::string, std::set<std::string>> student_registrations_;
    std::vector<AttendanceEntry> attendance_;
    std::vector<AdminRegistrationItem> admin_registrations_;
    std::vector<AdminAssignmentItem> admin_assignments_;
    std::map<std::string, DepartmentItem> departments_;
    std::vector<AttendanceReportRow> reports_;
    std::vector<ChangeHistoryItem> history_;
};

class FullAttendanceTestSuite : public ::testing::Test {
protected:
    AttendanceSystemEngine svc;
};

// TC01: Login with valid credentials
TEST_F(FullAttendanceTestSuite, TC01_LoginValidCredentials) {
    auto res = svc.login("student01", "123456");
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(res.message, "User logs in successfully");
}

// TC02: Login with wrong password
TEST_F(FullAttendanceTestSuite, TC02_LoginWrongPassword) {
    auto res = svc.login("student01", "wrongpass");
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(res.message, "Invalid username or password.");
}

// TC03: Register Account
TEST_F(FullAttendanceTestSuite, TC03_RegisterAccount) {
    auto res = svc.registerAccount("John", "john@gmail.com", "student01", "123456");
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(res.message, "Registration is completed successfully");
}

// TC04: Create Class Session (assigned course)
TEST_F(FullAttendanceTestSuite, TC04_CreateClassSessionAssignedCourse) {
    std::string status;
    std::string auto_close;
    auto res = svc.createLecturerSession("CS201", 16, "02 Dec 2026", "09:00 - 11:00", "09:00", "09:15", &status, &auto_close);
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(status, "Scheduled");
    EXPECT_EQ(auto_close, "09:30");
}

// TC05: Create Class Session (unassigned course)
TEST_F(FullAttendanceTestSuite, TC05_CreateClassSessionUnassignedCourseRejected) {
    auto res = svc.createLecturerSession("CS305", 1, "02 Dec 2026", "09:00 - 11:00", "09:00", "09:15");
    EXPECT_FALSE(res.ok);
    EXPECT_FALSE(res.message.empty());
}

// TC06: Close Session & Auto-Mark Absent
TEST_F(FullAttendanceTestSuite, TC06_CloseSessionAutoMarkAbsent) {
    auto res = svc.closeSession("CS201", 13);
    EXPECT_TRUE(res.ok);
    std::vector<AttendanceEntry> recs;
    svc.getStudentAttendanceHistory("SV2024002", "SV2024002", recs);
    bool found_sv2 = false;
    for (const auto& r : recs) {
        if (r.course_id == "CS201" && r.session_number == 13) {
            found_sv2 = true;
            EXPECT_EQ(r.status, "Absent");
        }
    }
    EXPECT_TRUE(found_sv2);
}

// TC07: Close Session & Auto-Mark Absent (non-existent session)
TEST_F(FullAttendanceTestSuite, TC07_CloseNonExistentSession) {
    auto res = svc.closeSession("CS201", 99);
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(res.message, "Session not found");
}

// TC08: Close Session & Auto-Mark Absent (already closed session preserves records)
TEST_F(FullAttendanceTestSuite, TC08_CloseAlreadyClosedSessionPreservesRecords) {
    auto res = svc.closeSession("CS201", 12);
    EXPECT_TRUE(res.ok);
    std::vector<AttendanceEntry> recs;
    svc.getStudentAttendanceHistory("SV2024001", "SV2024001", recs);
    ASSERT_FALSE(recs.empty());
    EXPECT_EQ(recs[0].status, "Present");
}

// TC09: Manual Attendance Recording (valid update to Present)
TEST_F(FullAttendanceTestSuite, TC09_ManualAttendanceUpdateToPresent) {
    auto res = svc.manualUpdateAttendance("CS201", 12, "SV2024004", "Present");
    EXPECT_TRUE(res.ok);
    std::vector<AttendanceEntry> recs;
    svc.getStudentAttendanceHistory("SV2024004", "SV2024004", recs);
    bool updated = false;
    for (const auto& r : recs) {
        if (r.course_id == "CS201" && r.session_number == 12) {
            updated = true;
            EXPECT_EQ(r.status, "Present");
        }
    }
    EXPECT_TRUE(updated);
}

// TC10: Manual Attendance Recording (invalid student ID rejected)
TEST_F(FullAttendanceTestSuite, TC10_ManualAttendanceInvalidStudentRejected) {
    auto res = svc.manualUpdateAttendance("CS201", 12, "SV9999999", "Present");
    EXPECT_FALSE(res.ok);
}

// TC11: Manual Attendance Recording (exact boundary 09:15 -> Present)
TEST_F(FullAttendanceTestSuite, TC11_CheckInExactBoundaryIsPresent) {
    std::string eval;
    auto res = svc.recordCheckInByBoundary("CS201", 13, "SV2024002", "09:15", "09:15", &eval);
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(eval, "Present");
}

// TC12: Manual Attendance Recording (1 minute after end 09:16 -> Late)
TEST_F(FullAttendanceTestSuite, TC12_CheckInOneMinuteAfterEndIsLate) {
    std::string eval;
    auto res = svc.recordCheckInByBoundary("CS201", 13, "SV2024003", "09:16", "09:15", &eval);
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(eval, "Late");
}

// TC13: Student Search & Filtering (matching name keyword)
TEST_F(FullAttendanceTestSuite, TC13_SearchByNameKeyword) {
    std::vector<std::string> matches;
    auto res = svc.searchStudentsInCourse("CS201", "Minh Anh", "", matches);
    EXPECT_TRUE(res.ok);
    ASSERT_EQ(matches.size(), 1u);
    EXPECT_EQ(matches[0], "Nguyễn Minh Anh (SV2024001)");
}

// TC14: Student Search & Filtering (non-existent student ID)
TEST_F(FullAttendanceTestSuite, TC14_SearchNonExistentStudentId) {
    std::vector<std::string> matches;
    auto res = svc.searchStudentsInCourse("CS201", "", "SV8888888", matches);
    EXPECT_FALSE(res.ok);
    EXPECT_TRUE(matches.empty());
    EXPECT_EQ(res.message, "No matching students found");
}

// TC15: Student Search & Filtering (both filters empty returns all enrolled in CS201)
TEST_F(FullAttendanceTestSuite, TC15_SearchEmptyFiltersReturnsAllEnrolled) {
    std::vector<std::string> matches;
    auto res = svc.searchStudentsInCourse("CS201", "", "", matches);
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(matches.size(), 4u);
}

// TC16: View Statistics & Export Report (Total: 4, Present: 2, Late: 1, Absent: 1, Rate: 75.0%)
TEST_F(FullAttendanceTestSuite, TC16_Session12SummaryMetrics) {
    int total = 0, present = 0, late = 0, absent = 0;
    double rate = 0.0;
    svc.getSessionSummary("CS201", 12, total, present, late, absent, rate);
    EXPECT_EQ(total, 4);
    EXPECT_EQ(present, 2);
    EXPECT_EQ(late, 1);
    EXPECT_EQ(absent, 1);
    EXPECT_NEAR(rate, 75.0, 1e-6);
}

// TC17: View Statistics & Export Report (0 completed sessions -> 0.0% safely)
TEST_F(FullAttendanceTestSuite, TC17_ZeroCompletedSessionsSafeRate) {
    auto rates = svc.getStudentRatesSafe("CS204", "SV2024005");
    EXPECT_NEAR(rates.first, 0.0, 1e-6);
    EXPECT_NEAR(rates.second, 0.0, 1e-6);
}

// TC18: Forgot Password (registered email)
TEST_F(FullAttendanceTestSuite, TC18_ForgotPasswordRegisteredEmail) {
    auto res = svc.forgotPassword("khang.le@attendance.edu.vn");
    EXPECT_TRUE(res.ok);
    EXPECT_FALSE(res.message.empty());
}

// TC19: Forgot Password (unregistered email)
TEST_F(FullAttendanceTestSuite, TC19_ForgotPasswordUnregisteredEmail) {
    auto res = svc.forgotPassword("unknown@example.com");
    EXPECT_FALSE(res.ok);
    EXPECT_FALSE(res.message.empty());
}

// TC20: Forgot Password (invalid email format)
TEST_F(FullAttendanceTestSuite, TC20_ForgotPasswordInvalidEmailFormat) {
    auto res = svc.forgotPassword("admin-email");
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(res.message, "Invalid email format.");
}

// TC21: Change Password (valid current and matching new password)
TEST_F(FullAttendanceTestSuite, TC21_ChangePasswordSuccess) {
    auto res = svc.changePassword("khang.le", "Admin@123", "NewAdmin@123", "NewAdmin@123");
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(svc.getAdminAccountPassword("khang.le"), "NewAdmin@123");
}

// TC22: Change Password (incorrect current password)
TEST_F(FullAttendanceTestSuite, TC22_ChangePasswordIncorrectCurrent) {
    auto res = svc.changePassword("khang.le", "Wrong123", "NewAdmin@123", "NewAdmin@123");
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(svc.getAdminAccountPassword("khang.le"), "Admin@123");
}

// TC23: Change Password (new password and confirm password mismatch)
TEST_F(FullAttendanceTestSuite, TC23_ChangePasswordMismatch) {
    auto res = svc.changePassword("khang.le", "Admin@123", "NewAdmin@123", "Different123");
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(res.message, "Passwords do not match.");
    EXPECT_EQ(svc.getAdminAccountPassword("khang.le"), "Admin@123");
}

// TC24: Create Account (valid Student account)
TEST_F(FullAttendanceTestSuite, TC24_CreateAccountStudentSuccess) {
    auto res = svc.createAdminAccount("Nguyen Van An", "an.nguyen", "an.nguyen@gmail.com", "Student", "An@123456");
    EXPECT_TRUE(res.ok);
    auto found = svc.searchAdminAccounts("an.nguyen");
    ASSERT_EQ(found.size(), 1u);
    EXPECT_EQ(found[0].role, "Student");
}

// TC25: Create Account (duplicate username)
TEST_F(FullAttendanceTestSuite, TC25_CreateAccountDuplicateUsername) {
    ASSERT_TRUE(svc.createAdminAccount("Nguyen Van An", "an.nguyen", "an.nguyen@gmail.com", "Student", "An@123456").ok);
    auto res = svc.createAdminAccount("Tran Van B", "an.nguyen", "b.tran@gmail.com", "Lecturer", "B@123456");
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(res.message, "Username already exists.");
}

// TC26: Create Account (empty required field username)
TEST_F(FullAttendanceTestSuite, TC26_CreateAccountEmptyUsernameRejected) {
    auto res = svc.createAdminAccount("Nguyen Van C", "", "c.nguyen@gmail.com", "Student", "C@123456");
    EXPECT_FALSE(res.ok);
    EXPECT_TRUE(svc.searchAdminAccounts("c.nguyen").empty());
}

// TC27: View/Search Account (existing username keyword)
TEST_F(FullAttendanceTestSuite, TC27_SearchAccountExistingKeyword) {
    ASSERT_TRUE(svc.createAdminAccount("Nguyen Van An", "an.nguyen", "an.nguyen@gmail.com", "Student", "An@123456").ok);
    auto list = svc.searchAdminAccounts("an.nguyen");
    ASSERT_EQ(list.size(), 1u);
    EXPECT_EQ(list[0].full_name, "Nguyen Van An");
    EXPECT_EQ(list[0].email, "an.nguyen@gmail.com");
}

// TC28: View/Search Account (non-existent keyword)
TEST_F(FullAttendanceTestSuite, TC28_SearchAccountNoMatchingFound) {
    auto list = svc.searchAdminAccounts("unknown999");
    EXPECT_TRUE(list.empty());
}

// TC29: View/Search Account (empty keyword returns all accounts)
TEST_F(FullAttendanceTestSuite, TC29_SearchAccountEmptyReturnsAll) {
    auto all = svc.searchAdminAccounts("");
    EXPECT_GE(all.size(), 5u);
}

// TC30: Update Account (valid full name update)
TEST_F(FullAttendanceTestSuite, TC30_UpdateAccountFullNameSuccess) {
    ASSERT_TRUE(svc.createAdminAccount("Nguyen Van An", "an.nguyen", "an.nguyen@gmail.com", "Student", "An@123456").ok);
    auto res = svc.updateAdminAccount("an.nguyen", "Nguyen Van An Updated", "an.nguyen@gmail.com");
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(svc.getAdminAccountFullName("an.nguyen"), "Nguyen Van An Updated");
}

// TC31: Update Account (invalid email format)
TEST_F(FullAttendanceTestSuite, TC31_UpdateAccountInvalidEmailRejected) {
    ASSERT_TRUE(svc.createAdminAccount("Nguyen Van An", "an.nguyen", "an.nguyen@gmail.com", "Student", "An@123456").ok);
    auto res = svc.updateAdminAccount("an.nguyen", "Nguyen Van An", "invalid-email");
    EXPECT_FALSE(res.ok);
    auto list = svc.searchAdminAccounts("an.nguyen");
    ASSERT_EQ(list.size(), 1u);
    EXPECT_EQ(list[0].email, "an.nguyen@gmail.com");
}

// TC32: Update Account (empty required field)
TEST_F(FullAttendanceTestSuite, TC32_UpdateAccountEmptyUsernameRejected) {
    ASSERT_TRUE(svc.createAdminAccount("Nguyen Van An", "an.nguyen", "an.nguyen@gmail.com", "Student", "An@123456").ok);
    auto res = svc.updateAdminAccount("", "Nguyen Van An", "an.nguyen@gmail.com");
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(svc.getAdminAccountFullName("an.nguyen"), "Nguyen Van An");
}

// TC33: Delete Account (confirm deletion of account without related records)
TEST_F(FullAttendanceTestSuite, TC33_DeleteAccountConfirmedSuccess) {
    auto res = svc.deleteAdminAccount("test.student01", true);
    EXPECT_TRUE(res.ok);
    EXPECT_TRUE(svc.searchAdminAccounts("test.student01").empty());
}

// TC34: Delete Account (cancel confirmation dialog)
TEST_F(FullAttendanceTestSuite, TC34_DeleteAccountCancelledKeepsAccount) {
    auto res = svc.deleteAdminAccount("test.student02", false);
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(svc.searchAdminAccounts("test.student02").size(), 1u);
}

// TC35: Delete Account (account with existing academic records protected)
TEST_F(FullAttendanceTestSuite, TC35_DeleteAccountWithAcademicRecordsProtected) {
    auto res = svc.deleteAdminAccount("student01", true);
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(svc.getAdminAccountStatus("student01"), "Inactive");
}

// TC36: Create Course (valid course)
TEST_F(FullAttendanceTestSuite, TC36_CreateCourseSuccess) {
    auto res = svc.createAdminCourse("CS102", "Introduction to Programming", 3);
    EXPECT_TRUE(res.ok);
    EXPECT_TRUE(svc.hasAdminCourse("CS102"));
}

// TC37: Create Course (duplicate course code)
TEST_F(FullAttendanceTestSuite, TC37_CreateCourseDuplicateCodeRejected) {
    auto res = svc.createAdminCourse("CS101", "Advanced Programming", 3);
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(res.message, "Course code already exists.");
}

// TC38: Create Course (negative credits rejected)
TEST_F(FullAttendanceTestSuite, TC38_CreateCourseNegativeCreditsRejected) {
    auto res = svc.createAdminCourse("CS202", "Database Systems", -1);
    EXPECT_FALSE(res.ok);
    EXPECT_FALSE(svc.hasAdminCourse("CS202"));
}

// TC39: Update Course (valid course name update)
TEST_F(FullAttendanceTestSuite, TC39_UpdateCourseNameSuccess) {
    auto res = svc.updateAdminCourse("CS101", "Programming Fundamentals");
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(svc.getAdminCourseName("CS101"), "Programming Fundamentals");
}

// TC40: Update Course (empty course name rejected)
TEST_F(FullAttendanceTestSuite, TC40_UpdateCourseEmptyNameRejected) {
    auto res = svc.updateAdminCourse("CS101", "");
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(svc.getAdminCourseName("CS101"), "Introduction to Programming");
}

// TC41: Update Course (maximum length 255-character course name)
TEST_F(FullAttendanceTestSuite, TC41_UpdateCourseMaxLength255Chars) {
    std::string max_name(255, 'A');
    auto res = svc.updateAdminCourse("CS101", max_name);
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(svc.getAdminCourseName("CS101").size(), 255u);
}

// TC42: Delete Course (course without related records)
TEST_F(FullAttendanceTestSuite, TC42_DeleteCourseWithoutRelatedRecordsSuccess) {
    auto res = svc.deleteAdminCourse("CS303", true);
    EXPECT_TRUE(res.ok);
    EXPECT_FALSE(svc.hasAdminCourse("CS303"));
}

// TC43: Delete Course (cancel deletion)
TEST_F(FullAttendanceTestSuite, TC43_DeleteCourseCancelledKeepsCourse) {
    auto res = svc.deleteAdminCourse("CS101", false);
    EXPECT_FALSE(res.ok);
    EXPECT_TRUE(svc.hasAdminCourse("CS101"));
}

// TC44: Delete Course (course with existing registrations and sessions protected)
TEST_F(FullAttendanceTestSuite, TC44_DeleteCourseWithRelatedRecordsProtected) {
    auto res = svc.deleteAdminCourse("CS101", true);
    EXPECT_FALSE(res.ok);
    EXPECT_TRUE(svc.hasAdminCourse("CS101"));
}

// TC45: Add Course Registration (valid student and open course)
TEST_F(FullAttendanceTestSuite, TC45_AddCourseRegistrationSuccess) {
    auto res = svc.addAdminCourseRegistration("012345678901", "CS101");
    EXPECT_TRUE(res.ok);
}

// TC46: Add Course Registration (duplicate registration rejected)
TEST_F(FullAttendanceTestSuite, TC46_AddCourseRegistrationDuplicateRejected) {
    ASSERT_TRUE(svc.addAdminCourseRegistration("012345678901", "CS101").ok);
    auto res = svc.addAdminCourseRegistration("012345678901", "CS101");
    EXPECT_FALSE(res.ok);
}

// TC47: Add Course Registration (closed course rejected)
TEST_F(FullAttendanceTestSuite, TC47_AddCourseRegistrationClosedCourseRejected) {
    auto res = svc.addAdminCourseRegistration("012345678901", "CS999");
    EXPECT_FALSE(res.ok);
}

// TC48: Assign Lecturer (valid course and active lecturer)
TEST_F(FullAttendanceTestSuite, TC48_AssignLecturerSuccess) {
    auto res = svc.assignLecturer("CS101", "012345678902");
    EXPECT_TRUE(res.ok);
}

// TC49: Assign Lecturer (non-existent lecturer ID rejected)
TEST_F(FullAttendanceTestSuite, TC49_AssignLecturerNonExistentRejected) {
    auto res = svc.assignLecturer("CS101", "999999999999");
    EXPECT_FALSE(res.ok);
}

// TC50: Assign Lecturer (duplicate lecturer assignment rejected)
TEST_F(FullAttendanceTestSuite, TC50_AssignLecturerDuplicateRejected) {
    ASSERT_TRUE(svc.assignLecturer("CS101", "012345678902").ok);
    auto res = svc.assignLecturer("CS101", "012345678902");
    EXPECT_FALSE(res.ok);
}

// TC51: Create Department (valid unique code and name)
TEST_F(FullAttendanceTestSuite, TC51_CreateDepartmentSuccess) {
    auto res = svc.createDepartment("IT", "Information Technology");
    EXPECT_TRUE(res.ok);
}

// TC52: Create Department (duplicate department code rejected)
TEST_F(FullAttendanceTestSuite, TC52_CreateDepartmentDuplicateCodeRejected) {
    ASSERT_TRUE(svc.createDepartment("IT", "Information Technology").ok);
    auto res = svc.createDepartment("IT", "Information Systems");
    EXPECT_FALSE(res.ok);
}

// TC53: Create Department (whitespace-only department name rejected)
TEST_F(FullAttendanceTestSuite, TC53_CreateDepartmentSpacesOnlyRejected) {
    auto res = svc.createDepartment("HR", "   ");
    EXPECT_FALSE(res.ok);
}

// TC54: Create Class Session (valid schedule)
TEST_F(FullAttendanceTestSuite, TC54_CreateAdminClassSessionSuccess) {
    auto res = svc.createAdminClassSession("CS101", "2026-12-01", "10:00", "12:00", "A101");
    EXPECT_TRUE(res.ok);
}

// TC55: Create Class Session (overlapping session in same room rejected)
TEST_F(FullAttendanceTestSuite, TC55_CreateAdminClassSessionOverlapRejected) {
    auto res = svc.createAdminClassSession("CS101", "2026-12-01", "09:00", "11:00", "A101");
    EXPECT_FALSE(res.ok);
}

// TC56: Create Class Session (end time earlier than start time rejected)
TEST_F(FullAttendanceTestSuite, TC56_CreateAdminClassSessionEndBeforeStartRejected) {
    auto res = svc.createAdminClassSession("CS101", "2026-12-01", "10:00", "08:00", "A101");
    EXPECT_FALSE(res.ok);
}

// TC57: View Attendance Reports (valid course and date range with records)
TEST_F(FullAttendanceTestSuite, TC57_ViewAttendanceReportsWithRecords) {
    std::vector<AttendanceReportRow> rows;
    auto res = svc.viewAttendanceReports("CS101", "2026-12-01", "2026-12-31", rows);
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(rows.size(), 2u);
}

// TC58: View Attendance Reports (date range with no records)
TEST_F(FullAttendanceTestSuite, TC58_ViewAttendanceReportsEmptyRange) {
    std::vector<AttendanceReportRow> rows;
    auto res = svc.viewAttendanceReports("CS101", "2027-01-01", "2027-01-02", rows);
    EXPECT_TRUE(res.ok);
    EXPECT_TRUE(rows.empty());
}

// TC59: View Attendance Reports (start date later than end date rejected)
TEST_F(FullAttendanceTestSuite, TC59_ViewAttendanceReportsInvalidRangeRejected) {
    std::vector<AttendanceReportRow> rows;
    auto res = svc.viewAttendanceReports("CS101", "2026-12-31", "2026-12-01", rows);
    EXPECT_FALSE(res.ok);
}

// TC60: View Attendance Change History (existing student ID / session ID)
TEST_F(FullAttendanceTestSuite, TC60_ViewChangeHistoryMatchingId) {
    std::vector<ChangeHistoryItem> by_student;
    auto res_student = svc.searchChangeHistoryById("012345678901", by_student);
    EXPECT_TRUE(res_student.ok);
    ASSERT_EQ(by_student.size(), 1u);
    EXPECT_EQ(by_student[0].session_id, "202612010001");
    EXPECT_EQ(by_student[0].previous_status, "Absent");
    EXPECT_EQ(by_student[0].new_status, "Present");
    EXPECT_FALSE(by_student[0].timestamp.empty());

    std::vector<ChangeHistoryItem> by_session;
    auto res_session = svc.searchChangeHistoryById("202612010001", by_session);
    EXPECT_TRUE(res_session.ok);
    ASSERT_EQ(by_session.size(), 1u);
}

// TC61: View Attendance Change History (non-existent ID returns no records)
TEST_F(FullAttendanceTestSuite, TC61_ViewChangeHistoryNoMatchingId) {
    std::vector<ChangeHistoryItem> items;
    auto res = svc.searchChangeHistoryById("999999999999", items);
    EXPECT_FALSE(res.ok);
    EXPECT_TRUE(items.empty());
}

// TC62: View Attendance Change History (start date later than end date rejected)
TEST_F(FullAttendanceTestSuite, TC62_ViewChangeHistoryInvalidDateRangeRejected) {
    std::vector<ChangeHistoryItem> items;
    auto res = svc.searchChangeHistoryByDateRange("2026-12-31", "2026-12-01", items);
    EXPECT_FALSE(res.ok);
}

// TC63: View Student Profile (existing student ST001)
TEST_F(FullAttendanceTestSuite, TC63_ViewStudentProfileExisting) {
    UserAccount profile;
    auto res = svc.viewStudentProfile("ST001", true, &profile);
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(profile.username, "ST001");
}

// TC64: View Student Profile (unknown student ID)
TEST_F(FullAttendanceTestSuite, TC64_ViewStudentProfileUnknownRejected) {
    UserAccount profile;
    auto res = svc.viewStudentProfile("UNKNOWN999", true, &profile);
    EXPECT_FALSE(res.ok);
}

// TC65: View Student Profile (backend unavailable)
TEST_F(FullAttendanceTestSuite, TC65_ViewStudentProfileBackendUnavailable) {
    UserAccount profile;
    auto res = svc.viewStudentProfile("ST001", false, &profile);
    EXPECT_FALSE(res.ok);
    EXPECT_FALSE(res.message.empty());
}

// TC66: Update Student Profile (valid details persist)
TEST_F(FullAttendanceTestSuite, TC66_UpdateStudentProfileSuccess) {
    auto res = svc.updateStudentProfile("ST001", "Tran Minh ST001", "student@example.com", "0330000000", "District 5, HCMC");
    EXPECT_TRUE(res.ok);
    UserAccount reloaded;
    ASSERT_TRUE(svc.viewStudentProfile("ST001", true, &reloaded).ok);
    EXPECT_EQ(reloaded.email, "student@example.com");
    EXPECT_EQ(reloaded.phone, "0330000000");
}

// TC67: Update Student Profile (invalid email rejected)
TEST_F(FullAttendanceTestSuite, TC67_UpdateStudentProfileInvalidEmailRejected) {
    auto res = svc.updateStudentProfile("ST001", "Tran Minh ST001", "invalid-email", "0330000000", "District 5, HCMC");
    EXPECT_FALSE(res.ok);
}

// TC68: Update Student Profile (blank full name rejected)
TEST_F(FullAttendanceTestSuite, TC68_UpdateStudentProfileBlankNameRejected) {
    UserAccount before;
    ASSERT_TRUE(svc.viewStudentProfile("ST001", true, &before).ok);
    auto res = svc.updateStudentProfile("ST001", "   ", "student@example.com", "0330000000", "District 5, HCMC");
    EXPECT_FALSE(res.ok);
    UserAccount after;
    ASSERT_TRUE(svc.viewStudentProfile("ST001", true, &after).ok);
    EXPECT_EQ(after.full_name, before.full_name);
}

// TC69: Update Student Profile (HTML/script payload stored safely as plain text)
TEST_F(FullAttendanceTestSuite, TC69_UpdateStudentProfileScriptTreatedAsData) {
    std::string payload = "<script>alert(1)</script>";
    auto res = svc.updateStudentProfile("ST001", "Tran Minh ST001", "st001@attendance.edu.vn", "0901111111", payload);
    EXPECT_TRUE(res.ok);
    UserAccount reloaded;
    ASSERT_TRUE(svc.viewStudentProfile("ST001", true, &reloaded).ok);
    EXPECT_EQ(reloaded.address, payload);
}

// TC70: Course Registration (valid registration for ST001 in CS201)
TEST_F(FullAttendanceTestSuite, TC70_CourseRegistrationSuccess) {
    auto res = svc.registerStudentCourse("ST001", "CS201");
    EXPECT_TRUE(res.ok);
    auto list = svc.getRegisteredCourses("ST001");
    EXPECT_NE(std::find(list.begin(), list.end(), "CS201"), list.end());
}

// TC71: Course Registration (duplicate registration rejected)
TEST_F(FullAttendanceTestSuite, TC71_CourseRegistrationDuplicateRejected) {
    ASSERT_TRUE(svc.registerStudentCourse("ST001", "CS201").ok);
    auto res = svc.registerStudentCourse("ST001", "CS201");
    EXPECT_FALSE(res.ok);
    auto list = svc.getRegisteredCourses("ST001");
    EXPECT_EQ(std::count(list.begin(), list.end(), "CS201"), 1);
}

// TC72: Course Registration (outside registration window rejected)
TEST_F(FullAttendanceTestSuite, TC72_CourseRegistrationOutsideWindowRejected) {
    auto res = svc.registerStudentCourse("ST001", "CS201", "2026-11-22", "2026-11-20");
    EXPECT_FALSE(res.ok);
}

// TC73: Course Registration (invalid course ID rejected)
TEST_F(FullAttendanceTestSuite, TC73_CourseRegistrationInvalidCourseRejected) {
    auto res = svc.registerStudentCourse("ST001", "INVALID");
    EXPECT_FALSE(res.ok);
}

// TC74: Course Registration (existing stored course PHI205 listed for ST001)
TEST_F(FullAttendanceTestSuite, TC74_CourseRegistrationStoredCourseVisible) {
    auto list = svc.getRegisteredCourses("ST001");
    EXPECT_NE(std::find(list.begin(), list.end(), "PHI205"), list.end());
}

// TC75: Course Registration (ST002 with no registrations shows empty state)
TEST_F(FullAttendanceTestSuite, TC75_CourseRegistrationEmptyStateForST002) {
    std::string empty_msg;
    auto list = svc.getRegisteredCourses("ST002", &empty_msg);
    EXPECT_TRUE(list.empty());
    EXPECT_FALSE(empty_msg.empty());
}

// TC76: Course Registration (CS202 registration persists)
TEST_F(FullAttendanceTestSuite, TC76_CourseRegistrationPersistsCS202) {
    auto res = svc.registerStudentCourse("ST001", "CS202");
    EXPECT_TRUE(res.ok);
    auto list = svc.getRegisteredCourses("ST001");
    EXPECT_NE(std::find(list.begin(), list.end(), "CS202"), list.end());
}

// TC77: Unregister Course (existing registered course removed)
TEST_F(FullAttendanceTestSuite, TC77_UnregisterCourseSuccess) {
    ASSERT_TRUE(svc.registerStudentCourse("ST001", "CS201").ok);
    auto res = svc.unregisterStudentCourse("ST001", "CS201", true);
    EXPECT_TRUE(res.ok);
    auto list = svc.getRegisteredCourses("ST001");
    EXPECT_EQ(std::find(list.begin(), list.end(), "CS201"), list.end());
}

// TC78: Unregister Course (course not registered rejected)
TEST_F(FullAttendanceTestSuite, TC78_UnregisterCourseNotRegisteredRejected) {
    auto res = svc.unregisterStudentCourse("ST001", "CS305", true);
    EXPECT_FALSE(res.ok);
}

// TC79: Unregister Course (cancelled confirmation keeps registration)
TEST_F(FullAttendanceTestSuite, TC79_UnregisterCourseCancelledPreserves) {
    ASSERT_TRUE(svc.registerStudentCourse("ST001", "CS201").ok);
    auto res = svc.unregisterStudentCourse("ST001", "CS201", false);
    EXPECT_FALSE(res.ok);
    auto list = svc.getRegisteredCourses("ST001");
    EXPECT_NE(std::find(list.begin(), list.end(), "CS201"), list.end());
}

// TC80: Attendance Check-in (registered course, open session, valid GPS & time)
TEST_F(FullAttendanceTestSuite, TC80_AttendanceCheckInValidSuccess) {
    ASSERT_TRUE(svc.registerStudentCourse("ST001", "CS201").ok);
    auto res = svc.studentCheckIn("ST001", "CS201", "S_CS201_13", true, true, true, "OPEN");
    EXPECT_TRUE(res.ok);
}

// TC81: Attendance Check-in (course not registered rejected)
TEST_F(FullAttendanceTestSuite, TC81_AttendanceCheckInUnregisteredCourseRejected) {
    auto res = svc.studentCheckIn("ST001", "CS202", "S_CS202_1", true, true, true, "OPEN");
    EXPECT_FALSE(res.ok);
}

// TC82: Attendance Check-in (outside permitted GPS radius rejected)
TEST_F(FullAttendanceTestSuite, TC82_AttendanceCheckInOutsideGpsRejected) {
    ASSERT_TRUE(svc.registerStudentCourse("ST001", "CS201").ok);
    auto res = svc.studentCheckIn("ST001", "CS201", "S_CS201_13", true, false, true, "OPEN");
    EXPECT_FALSE(res.ok);
}

// TC83: Attendance Check-in (closed session rejected)
TEST_F(FullAttendanceTestSuite, TC83_AttendanceCheckInClosedSessionRejected) {
    ASSERT_TRUE(svc.registerStudentCourse("ST001", "CS201").ok);
    auto res = svc.studentCheckIn("ST001", "CS201", "S_CS201_12", true, true, true, "CLOSED");
    EXPECT_FALSE(res.ok);
}

// TC84: Attendance Check-in (duplicate check-in for same student & session prevented)
TEST_F(FullAttendanceTestSuite, TC84_AttendanceCheckInDuplicatePrevented) {
    ASSERT_TRUE(svc.registerStudentCourse("ST001", "CS201").ok);
    ASSERT_TRUE(svc.studentCheckIn("ST001", "CS201", "S_CS201_13", true, true, true, "OPEN").ok);
    auto res = svc.studentCheckIn("ST001", "CS201", "S_CS201_13", true, true, true, "OPEN");
    EXPECT_FALSE(res.ok);
}

// TC85: Attendance Check-in (GPS permission denied rejected)
TEST_F(FullAttendanceTestSuite, TC85_AttendanceCheckInGpsPermissionDenied) {
    ASSERT_TRUE(svc.registerStudentCourse("ST001", "CS201").ok);
    auto res = svc.studentCheckIn("ST001", "CS201", "S_CS201_13", false, true, true, "OPEN");
    EXPECT_FALSE(res.ok);
}

// TC86: Attendance History (student with existing records)
TEST_F(FullAttendanceTestSuite, TC86_AttendanceHistoryExistingRecords) {
    std::vector<AttendanceEntry> recs;
    auto res = svc.getStudentAttendanceHistory("ST001", "ST001", recs);
    EXPECT_TRUE(res.ok);
    ASSERT_FALSE(recs.empty());
    EXPECT_EQ(recs[0].course_id, "PHI205");
}

// TC87: Attendance History (student with no records shows empty state)
TEST_F(FullAttendanceTestSuite, TC87_AttendanceHistoryNoRecordsEmptyState) {
    std::vector<AttendanceEntry> recs;
    auto res = svc.getStudentAttendanceHistory("ST002", "ST002", recs);
    EXPECT_TRUE(res.ok);
    EXPECT_TRUE(recs.empty());
}

// TC88: Attendance History (unauthorized access to another student's records denied)
TEST_F(FullAttendanceTestSuite, TC88_AttendanceHistoryUnauthorizedDenied) {
    std::vector<AttendanceEntry> recs;
    auto res = svc.getStudentAttendanceHistory("ST001", "ST002", recs);
    EXPECT_FALSE(res.ok);
    EXPECT_TRUE(recs.empty());
}

// TC89: Attendance Rate (8 attended out of 10 total sessions = 80.0%)
TEST_F(FullAttendanceTestSuite, TC89_AttendanceRate80Percent) {
    auto summary = svc.calculateAttendanceRate(8, 10);
    EXPECT_TRUE(summary.division_by_zero_safe);
    EXPECT_FALSE(summary.empty_state);
    EXPECT_NEAR(summary.rate, 80.0, 1e-6);
}

// TC90: Attendance Rate (0 attended out of 0 total sessions -> safe zero/empty state)
TEST_F(FullAttendanceTestSuite, TC90_AttendanceRateZeroSessionsSafe) {
    auto summary = svc.calculateAttendanceRate(0, 0);
    EXPECT_TRUE(summary.division_by_zero_safe);
    EXPECT_TRUE(summary.empty_state);
    EXPECT_NEAR(summary.rate, 0.0, 1e-6);
}

// TC91: Attendance Rate (mixed statuses Present: 6, Absent: 3, Excused: 1)
TEST_F(FullAttendanceTestSuite, TC91_AttendanceRateMixedStatusesPolicy) {
    auto summary = svc.calculatePolicyAttendanceRate(6, 3, 1);
    EXPECT_TRUE(summary.division_by_zero_safe);
    EXPECT_FALSE(summary.empty_state);
    EXPECT_NEAR(summary.rate, 70.0, 1e-6);
}

} // namespace attendance_full_test