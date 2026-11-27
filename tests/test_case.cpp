#include <gtest/gtest.h>

#include <algorithm>
#include <cctype>
#include <cmath>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

namespace attendance_full_test {

// --- Các class dữ liệu ---
class Result {
public:
    bool ok = false;
    string message;

    Result() = default;
    Result(bool isOk, string msg) : ok(isOk), message(move(msg)) {}
};

class UserAccount {
public:
    string username;
    string password;
    string full_name;
    string email;
    string phone;
    string address;
    string role;
    string status = "Active";
    bool has_academic_records = false;

    UserAccount() = default;
    UserAccount(string u, string p, string name, string mail,
                string ph, string addr, string r, string st = "Active", bool hasRec = false)
        : username(move(u)), password(move(p)), full_name(move(name)),
          email(move(mail)), phone(move(ph)), address(move(addr)),
          role(move(r)), status(move(st)), has_academic_records(hasRec) {}
};

class CourseRecord {
public:
    string id;
    string name;
    int credits = 3;
    bool assigned_to_lecturer = false;
    bool open_for_registration = true;
    bool has_related_records = false;

    CourseRecord() = default;
    CourseRecord(string cid, string cname, int cr, bool assigned, bool openReg, bool hasRec)
        : id(move(cid)), name(move(cname)), credits(cr),
          assigned_to_lecturer(assigned), open_for_registration(openReg),
          has_related_records(hasRec) {}
};

class SessionRecord {
public:
    string id;
    string course_id;
    int session_number = 0;
    string date;
    string class_time;
    string checkin_start;
    string checkin_end;
    string auto_close_time;
    string room;
    string status = "Open";

    SessionRecord() = default;
    SessionRecord(string sid, string cid, int snum, string d, string ctime,
                  string cstart, string cend, string autoclose, string rm, string st)
        : id(move(sid)), course_id(move(cid)), session_number(snum),
          date(move(d)), class_time(move(ctime)), checkin_start(move(cstart)),
          checkin_end(move(cend)), auto_close_time(move(autoclose)),
          room(move(rm)), status(move(st)) {}
};

class AttendanceEntry {
public:
    string student_id;
    string student_name;
    string course_id;
    int session_number = 0;
    string session_id;
    string date;
    string status;
    string checkin_time;

    AttendanceEntry() = default;
    AttendanceEntry(string stId, string stName, string cid, int snum,
                    string sid, string d, string st, string ctime)
        : student_id(move(stId)), student_name(move(stName)), course_id(move(cid)),
          session_number(snum), session_id(move(sid)), date(move(d)),
          status(move(st)), checkin_time(move(ctime)) {}
};

class AdminCourseItem {
public:
    string code;
    string name;
    int credits = 0;
    bool open_for_registration = true;
    bool has_related_records = false;

    AdminCourseItem() = default;
    AdminCourseItem(string c, string n, int cr, bool openReg, bool hasRec)
        : code(move(c)), name(move(n)), credits(cr),
          open_for_registration(openReg), has_related_records(hasRec) {}
};

class AdminRegistrationItem {
public:
    string student_id;
    string course_code;

    AdminRegistrationItem() = default;
    AdminRegistrationItem(string stId, string cCode)
        : student_id(move(stId)), course_code(move(cCode)) {}
};

class AdminAssignmentItem {
public:
    string course_code;
    string lecturer_id;

    AdminAssignmentItem() = default;
    AdminAssignmentItem(string cCode, string lecId)
        : course_code(move(cCode)), lecturer_id(move(lecId)) {}
};

class DepartmentItem {
public:
    string code;
    string name;

    DepartmentItem() = default;
    DepartmentItem(string c, string n) : code(move(c)), name(move(n)) {}
};

class AdminClassSessionItem {
public:
    string session_id;
    string course_code;
    string date;
    string start_time;
    string end_time;
    string room;

    AdminClassSessionItem() = default;
    AdminClassSessionItem(string sid, string cCode, string d, string sTime, string eTime, string rm)
        : session_id(move(sid)), course_code(move(cCode)), date(move(d)),
          start_time(move(sTime)), end_time(move(eTime)), room(move(rm)) {}
};

class AttendanceReportRow {
public:
    string student_id;
    string course_code;
    string date;
    string status;

    AttendanceReportRow() = default;
    AttendanceReportRow(string stId, string cCode, string d, string st)
        : student_id(move(stId)), course_code(move(cCode)),
          date(move(d)), status(move(st)) {}
};

class ChangeHistoryItem {
public:
    string student_id;
    string session_id;
    string previous_status;
    string new_status;
    string timestamp;

    ChangeHistoryItem() = default;
    ChangeHistoryItem(string stId, string sid, string prevSt, string newSt, string ts)
        : student_id(move(stId)), session_id(move(sid)),
          previous_status(move(prevSt)), new_status(move(newSt)),
          timestamp(move(ts)) {}
};

class RateSummary {
public:
    double rate = 0.0;
    bool division_by_zero_safe = true;
    bool empty_state = false;
    string message;

    RateSummary() = default;
    RateSummary(double r, bool safeDiv, bool isEmpty, string msg)
        : rate(r), division_by_zero_safe(safeDiv),
          empty_state(isEmpty), message(move(msg)) {}
};

// --- Class xử lý nghiệp vụ chính ---
class AttendanceSystemEngine {
public:
    AttendanceSystemEngine() {
        seedInitialData();
    }

    static bool isValidEmail(const string& email) {
        auto at_pos = email.find('@');
        if (at_pos == string::npos || at_pos == 0) {
            return false;
        }
        auto dot_pos = email.find('.', at_pos + 1);
        return dot_pos != string::npos && dot_pos + 1 < email.size();
    }

    static bool isBlankOrWhitespace(const string& s) {
        if (s.empty()) {
            return true;
        }
        return all_of(s.begin(), s.end(), [](unsigned char ch) {
            return isspace(ch) != 0;
        });
    }

    static string toLower(const string& s) {
        string out = s;
        for (char& c : out) {
            c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
        }
        return out;
    }

    static int timeToMinutes(const string& hhmm) {
        if (hhmm.size() != 5 || hhmm[2] != ':') {
            return -1;
        }
        try {
            int h = stoi(hhmm.substr(0, 2));
            int m = stoi(hhmm.substr(3, 2));
            if (h < 0 || h > 23 || m < 0 || m > 59) {
                return -1;
            }
            return h * 60 + m;
        } catch (...) {
            return -1;
        }
    }

    static string minutesToTime(int mins) {
        int wrapped = ((mins % 1440) + 1440) % 1440;
        int h = wrapped / 60;
        int m = wrapped % 60;
        string hh = (h < 10 ? "0" : "") + to_string(h);
        string mm = (m < 10 ? "0" : "") + to_string(m);
        return hh + ":" + mm;
    }

    Result login(const string& username, const string& password) const {
        auto it = users_.find(username);
        if (it == users_.end() || it->second.password != password) {
            return Result(false, "Invalid username or password.");
        }
        return Result(true, "User logs in successfully");
    }

    Result registerAccount(const string& name,
                           const string& email,
                           const string& username,
                           const string& password) {
        if (isBlankOrWhitespace(name) || isBlankOrWhitespace(email) ||
            isBlankOrWhitespace(username) || isBlankOrWhitespace(password)) {
            return Result(false, "Required fields are missing.");
        }
        users_[username] = UserAccount(username, password, name, email, "", "", "Student", "Active", false);
        return Result(true, "Registration is completed successfully");
    }

    Result createLecturerSession(const string& course_id,
                                 int session_number,
                                 const string& date,
                                 const string& class_time,
                                 const string& checkin_start,
                                 const string& checkin_end,
                                 string* out_status = nullptr,
                                 string* out_auto_close = nullptr) {
        auto c_it = courses_.find(course_id);
        if (c_it == courses_.end() || !c_it->second.assigned_to_lecturer) {
            return Result(false, "Session creation is rejected because the course is not assigned to the lecturer.");
        }
        int end_min = timeToMinutes(checkin_end);
        string auto_close = (end_min >= 0) ? minutesToTime(end_min + 15) : "09:30";
        SessionRecord rec("S_" + course_id + "_" + to_string(session_number),
                            course_id, session_number, date, class_time,
                            checkin_start, checkin_end, auto_close, "B201", "Scheduled");
        sessions_.push_back(rec);
        if (out_status) {
            *out_status = rec.status;
        }
        if (out_auto_close) {
            *out_auto_close = rec.auto_close_time;
        }
        return Result(true, "Session is created successfully with status 'Scheduled'");
    }

    Result closeSession(const string& course_id, int session_number) {
        for (auto& s : sessions_) {
            if (s.course_id == course_id && s.session_number == session_number) {
                if (s.status == "Closed") {
                    return Result(true, "Session remains 'Closed' and existing records are preserved.");
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
                            string sname = users_.count(sid) ? users_[sid].full_name : sid;
                            attendance_.push_back(AttendanceEntry(sid, sname, course_id, session_number, s.id, s.date, "Absent", ""));
                        }
                    }
                }
                return Result(true, "Session status changes to 'Closed'");
            }
        }
        return Result(false, "Session not found");
    }

    Result manualUpdateAttendance(const string& course_id,
                                  int session_number,
                                  const string& student_id,
                                  const string& new_status) {
        if (users_.find(student_id) == users_.end()) {
            return Result(false, "Invalid student ID.");
        }
        for (auto& a : attendance_) {
            if (a.course_id == course_id && a.session_number == session_number && a.student_id == student_id) {
                a.status = new_status;
                return Result(true, "Student attendance status is updated successfully");
            }
        }
        string sname = users_[student_id].full_name;
        attendance_.push_back(AttendanceEntry(student_id, sname, course_id, session_number,
                                              "S_" + course_id + "_" + to_string(session_number),
                                              "2026-11-25", new_status, ""));
        return Result(true, "Student attendance status is updated successfully");
    }

    Result recordCheckInByBoundary(const string& course_id,
                                   int session_number,
                                   const string& student_id,
                                   const string& checkin_time,
                                   const string& checkin_end,
                                   string* out_evaluated_status = nullptr) {
        int actual_m = timeToMinutes(checkin_time);
        int end_m = timeToMinutes(checkin_end);
        string eval_status = (actual_m <= end_m) ? "Present" : "Late";
        string sname = users_.count(student_id) ? users_[student_id].full_name : student_id;
        attendance_.push_back(AttendanceEntry(student_id, sname, course_id, session_number,
                                              "S_" + course_id + "_" + to_string(session_number),
                                              "2026-12-02", eval_status, checkin_time));
        if (out_evaluated_status) {
            *out_evaluated_status = eval_status;
        }
        return Result(true, eval_status);
    }

    Result searchStudentsInCourse(const string& course_id,
                                  const string& name_keyword,
                                  const string& student_id_filter,
                                  vector<string>& out_matches) const {
        out_matches.clear();
        auto it = enrolled_by_course_.find(course_id);
        if (it == enrolled_by_course_.end()) {
            return Result(false, "No matching students found");
        }
        string lower_name = toLower(name_keyword);
        for (const auto& sid : it->second) {
            string full_name = users_.count(sid) ? users_.at(sid).full_name : sid;
            bool match_name = name_keyword.empty() || (toLower(full_name).find(lower_name) != string::npos);
            bool match_id = student_id_filter.empty() || (sid == student_id_filter);
            if (match_name && match_id) {
                out_matches.push_back(full_name + " (" + sid + ")");
            }
        }
        if (out_matches.empty()) {
            return Result(false, "No matching students found");
        }
        return Result(true, "Matching students found");
    }

    void getSessionSummary(const string& course_id,
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

    pair<double, double> getStudentRatesSafe(const string& course_id,
                                             const string& student_id) const {
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

    Result forgotPassword(const string& email) const {
        if (!isValidEmail(email)) {
            return Result(false, "Invalid email format.");
        }
        for (const auto& kv : admin_accounts_) {
            if (kv.second.email == email) {
                return Result(true, "Password-reset instructions have been sent to the registered email address.");
            }
        }
        return Result(false, "If the email is registered, reset instructions have been sent.");
    }

    Result changePassword(const string& username,
                          const string& current_password,
                          const string& new_password,
                          const string& confirm_password) {
        auto it = admin_accounts_.find(username);
        if (it == admin_accounts_.end() || it->second.password != current_password) {
            return Result(false, "Current password is incorrect.");
        }
        if (new_password != confirm_password) {
            return Result(false, "Passwords do not match.");
        }
        it->second.password = new_password;
        return Result(true, "Password updated successfully.");
    }

    Result createAdminAccount(const string& full_name,
                              const string& username,
                              const string& email,
                              const string& role,
                              const string& password) {
        if (isBlankOrWhitespace(full_name) || isBlankOrWhitespace(username) ||
            isBlankOrWhitespace(email) || isBlankOrWhitespace(role) || isBlankOrWhitespace(password)) {
            return Result(false, "Required field is empty.");
        }
        if (admin_accounts_.count(username) > 0) {
            return Result(false, "Username already exists.");
        }
        admin_accounts_[username] = UserAccount(username, password, full_name, email, "", "", role, "Active", false);
        return Result(true, "Account created successfully.");
    }

    vector<UserAccount> searchAdminAccounts(const string& keyword) const {
        vector<UserAccount> out;
        for (const auto& kv : admin_accounts_) {
            if (keyword.empty() || kv.first.find(keyword) != string::npos) {
                out.push_back(kv.second);
            }
        }
        return out;
    }

    Result updateAdminAccount(const string& username,
                              const string& new_full_name,
                              const string& new_email) {
        if (isBlankOrWhitespace(username) || isBlankOrWhitespace(new_full_name)) {
            return Result(false, "Required field is empty.");
        }
        if (!isValidEmail(new_email)) {
            return Result(false, "Invalid email format.");
        }
        auto it = admin_accounts_.find(username);
        if (it == admin_accounts_.end()) {
            return Result(false, "Account not found.");
        }
        it->second.full_name = new_full_name;
        it->second.email = new_email;
        return Result(true, "Account updated successfully.");
    }

    Result deleteAdminAccount(const string& username, bool confirm) {
        if (!confirm) {
            return Result(false, "Deletion cancelled.");
        }
        auto it = admin_accounts_.find(username);
        if (it == admin_accounts_.end()) {
            return Result(false, "Account not found.");
        }
        if (it->second.has_academic_records) {
            it->second.status = "Inactive";
            return Result(false, "Cannot delete account with related academic records; deactivated instead.");
        }
        admin_accounts_.erase(it);
        return Result(true, "Account deleted successfully.");
    }

    Result createAdminCourse(const string& code,
                             const string& name,
                             int credits) {
        if (admin_courses_.count(code) > 0) {
            return Result(false, "Course code already exists.");
        }
        if (credits <= 0) {
            return Result(false, "Credits must be a positive number.");
        }
        if (isBlankOrWhitespace(code) || isBlankOrWhitespace(name)) {
            return Result(false, "Required field is empty.");
        }
        admin_courses_[code] = AdminCourseItem(code, name, credits, true, false);
        return Result(true, "Course created successfully.");
    }

    Result updateAdminCourse(const string& code, const string& new_name) {
        auto it = admin_courses_.find(code);
        if (it == admin_courses_.end()) {
            return Result(false, "Course not found.");
        }
        if (isBlankOrWhitespace(new_name)) {
            return Result(false, "Course name is required.");
        }
        it->second.name = new_name;
        return Result(true, "Course updated successfully.");
    }

    Result deleteAdminCourse(const string& code, bool confirm) {
        if (!confirm) {
            return Result(false, "Deletion cancelled.");
        }
        auto it = admin_courses_.find(code);
        if (it == admin_courses_.end()) {
            return Result(false, "Course not found.");
        }
        if (it->second.has_related_records) {
            return Result(false, "Cannot delete course because it has related registrations and sessions.");
        }
        admin_courses_.erase(it);
        return Result(true, "Course deleted successfully.");
    }

    Result addAdminCourseRegistration(const string& student_id,
                                      const string& course_code) {
        auto it = admin_courses_.find(course_code);
        if (it == admin_courses_.end() || !it->second.open_for_registration) {
            return Result(false, "Course is closed or unavailable for registration.");
        }
        for (const auto& r : admin_registrations_) {
            if (r.student_id == student_id && r.course_code == course_code) {
                return Result(false, "Duplicate course registration is not allowed.");
            }
        }
        admin_registrations_.push_back(AdminRegistrationItem(student_id, course_code));
        it->second.has_related_records = true;
        return Result(true, "Course registration created successfully.");
    }

    Result assignLecturer(const string& course_code,
                          const string& lecturer_id) {
        if (admin_courses_.find(course_code) == admin_courses_.end()) {
            return Result(false, "Course not found.");
        }
        auto acc_it = admin_accounts_.find(lecturer_id);
        if (acc_it == admin_accounts_.end() || acc_it->second.role != "Lecturer" || acc_it->second.status != "Active") {
            return Result(false, "Lecturer does not exist or is inactive.");
        }
        for (const auto& item : admin_assignments_) {
            if (item.course_code == course_code && item.lecturer_id == lecturer_id) {
                return Result(false, "Duplicate lecturer assignment is not allowed.");
            }
        }
        admin_assignments_.push_back(AdminAssignmentItem(course_code, lecturer_id));
        return Result(true, "Lecturer assigned to course successfully.");
    }

    Result createDepartment(const string& code, const string& name) {
        if (departments_.count(code) > 0) {
            return Result(false, "Department code already exists.");
        }
        if (isBlankOrWhitespace(name)) {
            return Result(false, "Invalid department name.");
        }
        departments_[code] = DepartmentItem(code, name);
        return Result(true, "Department created successfully.");
    }

    Result createAdminClassSession(const string& course_code,
                                   const string& date,
                                   const string& start_time,
                                   const string& end_time,
                                   const string& room) {
        if (admin_courses_.find(course_code) == admin_courses_.end()) {
            return Result(false, "Course not found.");
        }
        int s_min = timeToMinutes(start_time);
        int e_min = timeToMinutes(end_time);
        if (s_min < 0 || e_min < 0 || e_min <= s_min) {
            return Result(false, "End time must be later than start time.");
        }
        for (const auto& s : admin_sessions_) {
            if (s.room == room && s.date == date) {
                int existing_s = timeToMinutes(s.start_time);
                int existing_e = timeToMinutes(s.end_time);
                if (s_min < existing_e && e_min > existing_s) {
                    return Result(false, "Schedule conflict in the same room.");
                }
            }
        }
        string new_id = date + "_" + room + "_" + to_string(admin_sessions_.size() + 1);
        admin_sessions_.push_back(AdminClassSessionItem(new_id, course_code, date, start_time, end_time, room));
        return Result(true, "Class session created successfully.");
    }

    Result viewAttendanceReports(const string& course_code,
                               const string& from_date,
                               const string& to_date,
                               vector<AttendanceReportRow>& out_rows) const {
        out_rows.clear();
        if (from_date > to_date) {
            return Result(false, "From date must not be later than to date.");
        }
        for (const auto& r : reports_) {
            if (r.course_code == course_code && r.date >= from_date && r.date <= to_date) {
                out_rows.push_back(r);
            }
        }
        if (out_rows.empty()) {
            return Result(true, "No attendance records found.");
        }
        return Result(true, "Attendance report generated successfully.");
    }

    Result searchChangeHistoryById(const string& id_filter,
                                   vector<ChangeHistoryItem>& out_items) const {
        out_items.clear();
        for (const auto& h : history_) {
            if (h.student_id == id_filter || h.session_id == id_filter) {
                out_items.push_back(h);
            }
        }
        if (out_items.empty()) {
            return Result(false, "No records found.");
        }
        return Result(true, "Change history records found.");
    }

    Result searchChangeHistoryByDateRange(const string& from_date,
                                        const string& to_date,
                                        vector<ChangeHistoryItem>& out_items) const {
        out_items.clear();
        if (from_date > to_date) {
            return Result(false, "Invalid date range: start date is later than end date.");
        }
        for (const auto& h : history_) {
            string d = h.timestamp.substr(0, 10);
            if (d >= from_date && d <= to_date) {
                out_items.push_back(h);
            }
        }
        return Result(true, "Change history filtered by date range.");
    }

    Result viewStudentProfile(const string& student_id,
                              bool backend_available,
                              UserAccount* out_profile = nullptr) const {
        if (!backend_available) {
            return Result(false, "Backend unavailable: failed to load student profile.");
        }
        auto it = users_.find(student_id);
        if (it == users_.end()) {
            return Result(false, "Student not found.");
        }
        if (out_profile) {
            *out_profile = it->second;
        }
        return Result(true, "Student profile loaded.");
    }

    Result updateStudentProfile(const string& student_id,
                                const string& full_name,
                                const string& email,
                                const string& phone,
                                const string& address) {
        auto it = users_.find(student_id);
        if (it == users_.end()) {
            return Result(false, "Student not found.");
        }
        if (isBlankOrWhitespace(full_name)) {
            return Result(false, "Full name is required.");
        }
        if (!isValidEmail(email)) {
            return Result(false, "Invalid email format.");
        }
        it->second.full_name = full_name;
        it->second.email = email;
        it->second.phone = phone;
        it->second.address = address;
        return Result(true, "Student profile updated successfully.");
    }

    Result registerStudentCourse(const string& student_id,
                                 const string& course_id,
                                 const string& current_date = "2026-11-15",
                                 const string& registration_end = "2026-11-20") {
        if (current_date > registration_end) {
            return Result(false, "Registration period has ended.");
        }
        if (courses_.find(course_id) == courses_.end()) {
            return Result(false, "Invalid or unavailable course ID.");
        }
        auto& reg_set = student_registrations_[student_id];
        if (reg_set.count(course_id) > 0) {
            return Result(false, "Course is already registered.");
        }
        reg_set.insert(course_id);
        return Result(true, "Course registered successfully.");
    }

    vector<string> getRegisteredCourses(const string& student_id,
                                        string* out_empty_message = nullptr) const {
        vector<string> list;
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

    Result unregisterStudentCourse(const string& student_id,
                                   const string& course_id,
                                   bool confirmed = true) {
        if (!confirmed) {
            return Result(false, "Unregistration cancelled.");
        }
        auto it = student_registrations_.find(student_id);
        if (it == student_registrations_.end() || it->second.erase(course_id) == 0) {
            return Result(false, "Course registration does not exist.");
        }
        return Result(true, "Course unregistered successfully.");
    }

    Result studentCheckIn(const string& student_id,
                          const string& course_id,
                          const string& session_id,
                          bool gps_permission_granted,
                          bool within_gps_radius,
                          bool within_allowed_time,
                          const string& session_status) {
        if (!gps_permission_granted) {
            return Result(false, "Location permission denied.");
        }
        if (!within_gps_radius) {
            return Result(false, "Outside permitted GPS radius.");
        }
        if (session_status == "CLOSED") {
            return Result(false, "Session is closed.");
        }
        if (!within_allowed_time) {
            return Result(false, "Outside allowed check-in time.");
        }
        auto reg_it = student_registrations_.find(student_id);
        if (reg_it == student_registrations_.end() || reg_it->second.count(course_id) == 0) {
            return Result(false, "Course is not registered.");
        }
        for (const auto& a : attendance_) {
            if (a.student_id == student_id && a.session_id == session_id) {
                return Result(false, "Duplicate attendance is prevented.");
            }
        }
        attendance_.push_back(AttendanceEntry(student_id, users_[student_id].full_name, course_id,
                                              1, session_id, "2026-11-22", "Present", "08:05"));
        return Result(true, "Attendance recorded once.");
    }

    Result getStudentAttendanceHistory(const string& requester_id,
                                     const string& target_student_id,
                                     vector<AttendanceEntry>& out_records) const {
        out_records.clear();
        if (requester_id != target_student_id) {
            return Result(false, "Access denied: cannot view another student's attendance records.");
        }
        for (const auto& a : attendance_) {
            if (a.student_id == target_student_id) {
                out_records.push_back(a);
            }
        }
        if (out_records.empty()) {
            return Result(true, "No attendance records found for this student.");
        }
        return Result(true, "Attendance history retrieved.");
    }

    static RateSummary calculateAttendanceRate(int attended, int total_sessions) {
        if (total_sessions <= 0) {
            return RateSummary(0.0, true, true, "No scheduled sessions; attendance rate is 0.0%.");
        }
        double pct = (static_cast<double>(attended) / static_cast<double>(total_sessions)) * 100.0;
        return RateSummary(pct, true, false, "Calculated attendance rate.");
    }

    static RateSummary calculatePolicyAttendanceRate(int present, int absent, int excused) {
        int total = present + absent + excused;
        if (total <= 0) {
            return RateSummary(0.0, true, true, "No sessions.");
        }
        double pct = (static_cast<double>(present + excused) / static_cast<double>(total)) * 100.0;
        return RateSummary(pct, true, false, "Policy attendance rate calculated.");
    }

    bool hasAdminCourse(const string& code) const {
        return admin_courses_.count(code) > 0;
    }

    string getAdminCourseName(const string& code) const {
        auto it = admin_courses_.find(code);
        return (it != admin_courses_.end()) ? it->second.name : "";
    }

    string getAdminAccountFullName(const string& username) const {
        auto it = admin_accounts_.find(username);
        return (it != admin_accounts_.end()) ? it->second.full_name : "";
    }

    string getAdminAccountStatus(const string& username) const {
        auto it = admin_accounts_.find(username);
        return (it != admin_accounts_.end()) ? it->second.status : "";
    }

    string getAdminAccountPassword(const string& username) const {
        auto it = admin_accounts_.find(username);
        return (it != admin_accounts_.end()) ? it->second.password : "";
    }

private:
    void seedInitialData() {
        users_["student01"] = UserAccount("student01", "123456", "Student One", "student01@attendance.edu.vn", "0900000001", "District 1, HCMC", "Student", "Active", true);
        users_["ST001"] = UserAccount("ST001", "pass001", "Tran Minh ST001", "st001@attendance.edu.vn", "0901111111", "District 5, HCMC", "Student", "Active", true);
        users_["ST002"] = UserAccount("ST002", "pass002", "Le Hoang ST002", "st002@attendance.edu.vn", "0902222222", "District 10, HCMC", "Student", "Active", false);
        users_["SV2024001"] = UserAccount("SV2024001", "sv2024001", "Nguyễn Minh Anh", "sv2024001@attendance.edu.vn", "0910000001", "HCMC", "Student", "Active", true);
        users_["SV2024002"] = UserAccount("SV2024002", "sv2024002", "Pham Bao SV2", "sv2024002@attendance.edu.vn", "0910000002", "HCMC", "Student", "Active", true);
        users_["SV2024003"] = UserAccount("SV2024003", "sv2024003", "Do Gia SV3", "sv2024003@attendance.edu.vn", "0910000003", "HCMC", "Student", "Active", true);
        users_["SV2024004"] = UserAccount("SV2024004", "sv2024004", "Vo Lan SV4", "sv2024004@attendance.edu.vn", "0910000004", "HCMC", "Student", "Active", true);
        users_["SV2024005"] = UserAccount("SV2024005", "sv2024005", "Hoang Nam SV5", "sv2024005@attendance.edu.vn", "0910000005", "HCMC", "Student", "Active", false);

        admin_accounts_["khang.le"] = UserAccount("khang.le", "Admin@123", "Le Minh Khang", "khang.le@attendance.edu.vn", "", "", "Administrator", "Active", false);
        admin_accounts_["student01"] = UserAccount("student01", "123456", "Nguyen Van Student", "student01@attendance.edu.vn", "", "", "Student", "Active", true);
        admin_accounts_["test.student01"] = UserAccount("test.student01", "Pass@123", "Test Student One", "test.student01@attendance.edu.vn", "", "", "Student", "Active", false);
        admin_accounts_["test.student02"] = UserAccount("test.student02", "Pass@123", "Test Student Two", "test.student02@attendance.edu.vn", "", "", "Student", "Active", false);
        admin_accounts_["012345678902"] = UserAccount("012345678902", "Lec@123", "Lecturer Assigned", "lec02@attendance.edu.vn", "", "", "Lecturer", "Active", true);

        courses_["CS201"] = CourseRecord("CS201", "Object-Oriented Programming", 3, true, true, true);
        courses_["CS202"] = CourseRecord("CS202", "Database Systems", 3, true, true, false);
        courses_["CS305"] = CourseRecord("CS305", "Distributed Systems", 3, false, true, false);
        courses_["CS204"] = CourseRecord("CS204", "Operating Systems", 3, true, true, false);
        courses_["PHI205"] = CourseRecord("PHI205", "Philosophy of Science", 2, true, true, true);

        admin_courses_["CS101"] = AdminCourseItem("CS101", "Introduction to Programming", 3, true, true);
        admin_courses_["CS303"] = AdminCourseItem("CS303", "Discrete Structures", 3, true, false);
        admin_courses_["CS999"] = AdminCourseItem("CS999", "Closed Elective", 3, false, false);

        sessions_.push_back(SessionRecord("S_CS201_12", "CS201", 12, "2026-11-25", "09:00 - 11:00", "09:00", "09:15", "09:30", "B201", "Closed"));
        sessions_.push_back(SessionRecord("S_CS201_13", "CS201", 13, "2026-12-02", "09:00 - 11:00", "09:00", "09:15", "09:30", "B201", "Open"));

        admin_sessions_.push_back(AdminClassSessionItem("202612010001", "CS101", "2026-12-01", "08:00", "10:00", "A101"));

        enrolled_by_course_["CS201"] = {"SV2024001", "SV2024002", "SV2024003", "SV2024004"};
        student_registrations_["ST001"].insert("PHI205");

        attendance_.push_back(AttendanceEntry("SV2024001", "Nguyễn Minh Anh", "CS201", 12, "S_CS201_12", "2026-11-25", "Present", "09:02"));
        attendance_.push_back(AttendanceEntry("SV2024002", "Pham Bao SV2", "CS201", 12, "S_CS201_12", "2026-11-25", "Present", "09:05"));
        attendance_.push_back(AttendanceEntry("SV2024003", "Do Gia SV3", "CS201", 12, "S_CS201_12", "2026-11-25", "Late", "09:16"));
        attendance_.push_back(AttendanceEntry("SV2024004", "Vo Lan SV4", "CS201", 12, "S_CS201_12", "2026-11-25", "Absent", ""));
        attendance_.push_back(AttendanceEntry("ST001", "Tran Minh ST001", "PHI205", 1, "S_PHI205_1", "2026-11-18", "Present", "08:01"));

        reports_.push_back(AttendanceReportRow("012345678901", "CS101", "2026-12-01", "Present"));
        reports_.push_back(AttendanceReportRow("012345678901", "CS101", "2026-12-15", "Late"));

        history_.push_back(ChangeHistoryItem("012345678901", "202612010001", "Absent", "Present", "2026-12-01 08:05:00"));
    }

    map<string, UserAccount> users_;
    map<string, UserAccount> admin_accounts_;
    map<string, CourseRecord> courses_;
    map<string, AdminCourseItem> admin_courses_;
    vector<SessionRecord> sessions_;
    vector<AdminClassSessionItem> admin_sessions_;
    map<string, vector<string>> enrolled_by_course_;
    map<string, set<string>> student_registrations_;
    vector<AttendanceEntry> attendance_;
    vector<AdminRegistrationItem> admin_registrations_;
    vector<AdminAssignmentItem> admin_assignments_;
    map<string, DepartmentItem> departments_;
    vector<AttendanceReportRow> reports_;
    vector<ChangeHistoryItem> history_;
};

// --- Test Fixture Class (TC01 -> TC91) ---
class FullAttendanceTestSuite : public ::testing::Test {
protected:
    AttendanceSystemEngine svc;
};

// TC01..TC91
TEST_F(FullAttendanceTestSuite, TC01_LoginValidCredentials) {
    auto res = svc.login("student01", "123456");
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(res.message, "User logs in successfully");
}

TEST_F(FullAttendanceTestSuite, TC02_LoginWrongPassword) {
    auto res = svc.login("student01", "wrongpass");
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(res.message, "Invalid username or password.");
}

TEST_F(FullAttendanceTestSuite, TC03_RegisterAccount) {
    auto res = svc.registerAccount("John", "john@gmail.com", "student01", "123456");
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(res.message, "Registration is completed successfully");
}

TEST_F(FullAttendanceTestSuite, TC04_CreateClassSessionAssignedCourse) {
    string status;
    string auto_close;
    auto res = svc.createLecturerSession("CS201", 16, "02 Dec 2026", "09:00 - 11:00", "09:00", "09:15", &status, &auto_close);
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(status, "Scheduled");
    EXPECT_EQ(auto_close, "09:30");
}

TEST_F(FullAttendanceTestSuite, TC05_CreateClassSessionUnassignedCourseRejected) {
    auto res = svc.createLecturerSession("CS305", 1, "02 Dec 2026", "09:00 - 11:00", "09:00", "09:15");
    EXPECT_FALSE(res.ok);
    EXPECT_FALSE(res.message.empty());
}

TEST_F(FullAttendanceTestSuite, TC06_CloseSessionAutoMarkAbsent) {
    auto res = svc.closeSession("CS201", 13);
    EXPECT_TRUE(res.ok);
    vector<AttendanceEntry> recs;
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

TEST_F(FullAttendanceTestSuite, TC07_CloseNonExistentSession) {
    auto res = svc.closeSession("CS201", 99);
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(res.message, "Session not found");
}

TEST_F(FullAttendanceTestSuite, TC08_CloseAlreadyClosedSessionPreservesRecords) {
    auto res = svc.closeSession("CS201", 12);
    EXPECT_TRUE(res.ok);
    vector<AttendanceEntry> recs;
    svc.getStudentAttendanceHistory("SV2024001", "SV2024001", recs);
    ASSERT_FALSE(recs.empty());
    EXPECT_EQ(recs[0].status, "Present");
}

TEST_F(FullAttendanceTestSuite, TC09_ManualAttendanceUpdateToPresent) {
    auto res = svc.manualUpdateAttendance("CS201", 12, "SV2024004", "Present");
    EXPECT_TRUE(res.ok);
    vector<AttendanceEntry> recs;
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

TEST_F(FullAttendanceTestSuite, TC10_ManualAttendanceInvalidStudentRejected) {
    auto res = svc.manualUpdateAttendance("CS201", 12, "SV9999999", "Present");
    EXPECT_FALSE(res.ok);
}

TEST_F(FullAttendanceTestSuite, TC11_CheckInExactBoundaryIsPresent) {
    string eval;
    auto res = svc.recordCheckInByBoundary("CS201", 13, "SV2024002", "09:15", "09:15", &eval);
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(eval, "Present");
}

TEST_F(FullAttendanceTestSuite, TC12_CheckInOneMinuteAfterEndIsLate) {
    string eval;
    auto res = svc.recordCheckInByBoundary("CS201", 13, "SV2024003", "09:16", "09:15", &eval);
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(eval, "Late");
}

TEST_F(FullAttendanceTestSuite, TC13_SearchByNameKeyword) {
    vector<string> matches;
    auto res = svc.searchStudentsInCourse("CS201", "Minh Anh", "", matches);
    EXPECT_TRUE(res.ok);
    ASSERT_EQ(matches.size(), 1u);
    EXPECT_EQ(matches[0], "Nguyễn Minh Anh (SV2024001)");
}

TEST_F(FullAttendanceTestSuite, TC14_SearchNonExistentStudentId) {
    vector<string> matches;
    auto res = svc.searchStudentsInCourse("CS201", "", "SV8888888", matches);
    EXPECT_FALSE(res.ok);
    EXPECT_TRUE(matches.empty());
    EXPECT_EQ(res.message, "No matching students found");
}

TEST_F(FullAttendanceTestSuite, TC15_SearchEmptyFiltersReturnsAllEnrolled) {
    vector<string> matches;
    auto res = svc.searchStudentsInCourse("CS201", "", "", matches);
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(matches.size(), 4u);
}

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

TEST_F(FullAttendanceTestSuite, TC17_ZeroCompletedSessionsSafeRate) {
    auto rates = svc.getStudentRatesSafe("CS204", "SV2024005");
    EXPECT_NEAR(rates.first, 0.0, 1e-6);
    EXPECT_NEAR(rates.second, 0.0, 1e-6);
}

TEST_F(FullAttendanceTestSuite, TC18_ForgotPasswordRegisteredEmail) {
    auto res = svc.forgotPassword("khang.le@attendance.edu.vn");
    EXPECT_TRUE(res.ok);
    EXPECT_FALSE(res.message.empty());
}

TEST_F(FullAttendanceTestSuite, TC19_ForgotPasswordUnregisteredEmail) {
    auto res = svc.forgotPassword("unknown@example.com");
    EXPECT_FALSE(res.ok);
    EXPECT_FALSE(res.message.empty());
}

TEST_F(FullAttendanceTestSuite, TC20_ForgotPasswordInvalidEmailFormat) {
    auto res = svc.forgotPassword("admin-email");
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(res.message, "Invalid email format.");
}

TEST_F(FullAttendanceTestSuite, TC21_ChangePasswordSuccess) {
    auto res = svc.changePassword("khang.le", "Admin@123", "NewAdmin@123", "NewAdmin@123");
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(svc.getAdminAccountPassword("khang.le"), "NewAdmin@123");
}

TEST_F(FullAttendanceTestSuite, TC22_ChangePasswordIncorrectCurrent) {
    auto res = svc.changePassword("khang.le", "Wrong123", "NewAdmin@123", "NewAdmin@123");
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(svc.getAdminAccountPassword("khang.le"), "Admin@123");
}

TEST_F(FullAttendanceTestSuite, TC23_ChangePasswordMismatch) {
    auto res = svc.changePassword("khang.le", "Admin@123", "NewAdmin@123", "Different123");
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(res.message, "Passwords do not match.");
    EXPECT_EQ(svc.getAdminAccountPassword("khang.le"), "Admin@123");
}

TEST_F(FullAttendanceTestSuite, TC24_CreateAccountStudentSuccess) {
    auto res = svc.createAdminAccount("Nguyen Van An", "an.nguyen", "an.nguyen@gmail.com", "Student", "An@123456");
    EXPECT_TRUE(res.ok);
    auto found = svc.searchAdminAccounts("an.nguyen");
    ASSERT_EQ(found.size(), 1u);
    EXPECT_EQ(found[0].role, "Student");
}

TEST_F(FullAttendanceTestSuite, TC25_CreateAccountDuplicateUsername) {
    ASSERT_TRUE(svc.createAdminAccount("Nguyen Van An", "an.nguyen", "an.nguyen@gmail.com", "Student", "An@123456").ok);
    auto res = svc.createAdminAccount("Tran Van B", "an.nguyen", "b.tran@gmail.com", "Lecturer", "B@123456");
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(res.message, "Username already exists.");
}

TEST_F(FullAttendanceTestSuite, TC26_CreateAccountEmptyUsernameRejected) {
    auto res = svc.createAdminAccount("Nguyen Van C", "", "c.nguyen@gmail.com", "Student", "C@123456");
    EXPECT_FALSE(res.ok);
    EXPECT_TRUE(svc.searchAdminAccounts("c.nguyen").empty());
}

TEST_F(FullAttendanceTestSuite, TC27_SearchAccountExistingKeyword) {
    ASSERT_TRUE(svc.createAdminAccount("Nguyen Van An", "an.nguyen", "an.nguyen@gmail.com", "Student", "An@123456").ok);
    auto list = svc.searchAdminAccounts("an.nguyen");
    ASSERT_EQ(list.size(), 1u);
    EXPECT_EQ(list[0].full_name, "Nguyen Van An");
    EXPECT_EQ(list[0].email, "an.nguyen@gmail.com");
}

TEST_F(FullAttendanceTestSuite, TC28_SearchAccountNoMatchingFound) {
    auto list = svc.searchAdminAccounts("unknown999");
    EXPECT_TRUE(list.empty());
}

TEST_F(FullAttendanceTestSuite, TC29_SearchAccountEmptyReturnsAll) {
    auto all = svc.searchAdminAccounts("");
    EXPECT_GE(all.size(), 5u);
}

TEST_F(FullAttendanceTestSuite, TC30_UpdateAccountFullNameSuccess) {
    ASSERT_TRUE(svc.createAdminAccount("Nguyen Van An", "an.nguyen", "an.nguyen@gmail.com", "Student", "An@123456").ok);
    auto res = svc.updateAdminAccount("an.nguyen", "Nguyen Van An Updated", "an.nguyen@gmail.com");
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(svc.getAdminAccountFullName("an.nguyen"), "Nguyen Van An Updated");
}

TEST_F(FullAttendanceTestSuite, TC31_UpdateAccountInvalidEmailRejected) {
    ASSERT_TRUE(svc.createAdminAccount("Nguyen Van An", "an.nguyen", "an.nguyen@gmail.com", "Student", "An@123456").ok);
    auto res = svc.updateAdminAccount("an.nguyen", "Nguyen Van An", "invalid-email");
    EXPECT_FALSE(res.ok);
    auto list = svc.searchAdminAccounts("an.nguyen");
    ASSERT_EQ(list.size(), 1u);
    EXPECT_EQ(list[0].email, "an.nguyen@gmail.com");
}

TEST_F(FullAttendanceTestSuite, TC32_UpdateAccountEmptyUsernameRejected) {
    ASSERT_TRUE(svc.createAdminAccount("Nguyen Van An", "an.nguyen", "an.nguyen@gmail.com", "Student", "An@123456").ok);
    auto res = svc.updateAdminAccount("", "Nguyen Van An", "an.nguyen@gmail.com");
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(svc.getAdminAccountFullName("an.nguyen"), "Nguyen Van An");
}

TEST_F(FullAttendanceTestSuite, TC33_DeleteAccountConfirmedSuccess) {
    auto res = svc.deleteAdminAccount("test.student01", true);
    EXPECT_TRUE(res.ok);
    EXPECT_TRUE(svc.searchAdminAccounts("test.student01").empty());
}

TEST_F(FullAttendanceTestSuite, TC34_DeleteAccountCancelledKeepsAccount) {
    auto res = svc.deleteAdminAccount("test.student02", false);
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(svc.searchAdminAccounts("test.student02").size(), 1u);
}

TEST_F(FullAttendanceTestSuite, TC35_DeleteAccountWithAcademicRecordsProtected) {
    auto res = svc.deleteAdminAccount("student01", true);
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(svc.getAdminAccountStatus("student01"), "Inactive");
}

TEST_F(FullAttendanceTestSuite, TC36_CreateCourseSuccess) {
    auto res = svc.createAdminCourse("CS102", "Introduction to Programming", 3);
    EXPECT_TRUE(res.ok);
    EXPECT_TRUE(svc.hasAdminCourse("CS102"));
}

TEST_F(FullAttendanceTestSuite, TC37_CreateCourseDuplicateCodeRejected) {
    auto res = svc.createAdminCourse("CS101", "Advanced Programming", 3);
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(res.message, "Course code already exists.");
}

TEST_F(FullAttendanceTestSuite, TC38_CreateCourseNegativeCreditsRejected) {
    auto res = svc.createAdminCourse("CS202", "Database Systems", -1);
    EXPECT_FALSE(res.ok);
    EXPECT_FALSE(svc.hasAdminCourse("CS202"));
}

TEST_F(FullAttendanceTestSuite, TC39_UpdateCourseNameSuccess) {
    auto res = svc.updateAdminCourse("CS101", "Programming Fundamentals");
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(svc.getAdminCourseName("CS101"), "Programming Fundamentals");
}

TEST_F(FullAttendanceTestSuite, TC40_UpdateCourseEmptyNameRejected) {
    auto res = svc.updateAdminCourse("CS101", "");
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(svc.getAdminCourseName("CS101"), "Introduction to Programming");
}

TEST_F(FullAttendanceTestSuite, TC41_UpdateCourseMaxLength255Chars) {
    string max_name(255, 'A');
    auto res = svc.updateAdminCourse("CS101", max_name);
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(svc.getAdminCourseName("CS101").size(), 255u);
}

TEST_F(FullAttendanceTestSuite, TC42_DeleteCourseWithoutRelatedRecordsSuccess) {
    auto res = svc.deleteAdminCourse("CS303", true);
    EXPECT_TRUE(res.ok);
    EXPECT_FALSE(svc.hasAdminCourse("CS303"));
}

TEST_F(FullAttendanceTestSuite, TC43_DeleteCourseCancelledKeepsCourse) {
    auto res = svc.deleteAdminCourse("CS101", false);
    EXPECT_FALSE(res.ok);
    EXPECT_TRUE(svc.hasAdminCourse("CS101"));
}

TEST_F(FullAttendanceTestSuite, TC44_DeleteCourseWithRelatedRecordsProtected) {
    auto res = svc.deleteAdminCourse("CS101", true);
    EXPECT_FALSE(res.ok);
    EXPECT_TRUE(svc.hasAdminCourse("CS101"));
}

TEST_F(FullAttendanceTestSuite, TC45_AddCourseRegistrationSuccess) {
    auto res = svc.addAdminCourseRegistration("012345678901", "CS101");
    EXPECT_TRUE(res.ok);
}

TEST_F(FullAttendanceTestSuite, TC46_AddCourseRegistrationDuplicateRejected) {
    ASSERT_TRUE(svc.addAdminCourseRegistration("012345678901", "CS101").ok);
    auto res = svc.addAdminCourseRegistration("012345678901", "CS101");
    EXPECT_FALSE(res.ok);
}

TEST_F(FullAttendanceTestSuite, TC47_AddCourseRegistrationClosedCourseRejected) {
    auto res = svc.addAdminCourseRegistration("012345678901", "CS999");
    EXPECT_FALSE(res.ok);
}

TEST_F(FullAttendanceTestSuite, TC48_AssignLecturerSuccess) {
    auto res = svc.assignLecturer("CS101", "012345678902");
    EXPECT_TRUE(res.ok);
}

TEST_F(FullAttendanceTestSuite, TC49_AssignLecturerNonExistentRejected) {
    auto res = svc.assignLecturer("CS101", "999999999999");
    EXPECT_FALSE(res.ok);
}

TEST_F(FullAttendanceTestSuite, TC50_AssignLecturerDuplicateRejected) {
    ASSERT_TRUE(svc.assignLecturer("CS101", "012345678902").ok);
    auto res = svc.assignLecturer("CS101", "012345678902");
    EXPECT_FALSE(res.ok);
}

TEST_F(FullAttendanceTestSuite, TC51_CreateDepartmentSuccess) {
    auto res = svc.createDepartment("IT", "Information Technology");
    EXPECT_TRUE(res.ok);
}

TEST_F(FullAttendanceTestSuite, TC52_CreateDepartmentDuplicateCodeRejected) {
    ASSERT_TRUE(svc.createDepartment("IT", "Information Technology").ok);
    auto res = svc.createDepartment("IT", "Information Systems");
    EXPECT_FALSE(res.ok);
}

TEST_F(FullAttendanceTestSuite, TC53_CreateDepartmentSpacesOnlyRejected) {
    auto res = svc.createDepartment("HR", "   ");
    EXPECT_FALSE(res.ok);
}

TEST_F(FullAttendanceTestSuite, TC54_CreateAdminClassSessionSuccess) {
    auto res = svc.createAdminClassSession("CS101", "2026-12-01", "10:00", "12:00", "A101");
    EXPECT_TRUE(res.ok);
}

TEST_F(FullAttendanceTestSuite, TC55_CreateAdminClassSessionOverlapRejected) {
    auto res = svc.createAdminClassSession("CS101", "2026-12-01", "09:00", "11:00", "A101");
    EXPECT_FALSE(res.ok);
}

TEST_F(FullAttendanceTestSuite, TC56_CreateAdminClassSessionEndBeforeStartRejected) {
    auto res = svc.createAdminClassSession("CS101", "2026-12-01", "10:00", "08:00", "A101");
    EXPECT_FALSE(res.ok);
}

TEST_F(FullAttendanceTestSuite, TC57_ViewAttendanceReportsWithRecords) {
    vector<AttendanceReportRow> rows;
    auto res = svc.viewAttendanceReports("CS101", "2026-12-01", "2026-12-31", rows);
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(rows.size(), 2u);
}

TEST_F(FullAttendanceTestSuite, TC58_ViewAttendanceReportsEmptyRange) {
    vector<AttendanceReportRow> rows;
    auto res = svc.viewAttendanceReports("CS101", "2027-01-01", "2027-01-02", rows);
    EXPECT_TRUE(res.ok);
    EXPECT_TRUE(rows.empty());
}

TEST_F(FullAttendanceTestSuite, TC59_ViewAttendanceReportsInvalidRangeRejected) {
    vector<AttendanceReportRow> rows;
    auto res = svc.viewAttendanceReports("CS101", "2026-12-31", "2026-12-01", rows);
    EXPECT_FALSE(res.ok);
}

TEST_F(FullAttendanceTestSuite, TC60_ViewChangeHistoryMatchingId) {
    vector<ChangeHistoryItem> by_student;
    auto res_student = svc.searchChangeHistoryById("012345678901", by_student);
    EXPECT_TRUE(res_student.ok);
    ASSERT_EQ(by_student.size(), 1u);
    EXPECT_EQ(by_student[0].session_id, "202612010001");
    EXPECT_EQ(by_student[0].previous_status, "Absent");
    EXPECT_EQ(by_student[0].new_status, "Present");
    EXPECT_FALSE(by_student[0].timestamp.empty());

    vector<ChangeHistoryItem> by_session;
    auto res_session = svc.searchChangeHistoryById("202612010001", by_session);
    EXPECT_TRUE(res_session.ok);
    ASSERT_EQ(by_session.size(), 1u);
}

TEST_F(FullAttendanceTestSuite, TC61_ViewChangeHistoryNoMatchingId) {
    vector<ChangeHistoryItem> items;
    auto res = svc.searchChangeHistoryById("999999999999", items);
    EXPECT_FALSE(res.ok);
    EXPECT_TRUE(items.empty());
}

TEST_F(FullAttendanceTestSuite, TC62_ViewChangeHistoryInvalidDateRangeRejected) {
    vector<ChangeHistoryItem> items;
    auto res = svc.searchChangeHistoryByDateRange("2026-12-31", "2026-12-01", items);
    EXPECT_FALSE(res.ok);
}

TEST_F(FullAttendanceTestSuite, TC63_ViewStudentProfileExisting) {
    UserAccount profile;
    auto res = svc.viewStudentProfile("ST001", true, &profile);
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(profile.username, "ST001");
}

TEST_F(FullAttendanceTestSuite, TC64_ViewStudentProfileUnknownRejected) {
    UserAccount profile;
    auto res = svc.viewStudentProfile("UNKNOWN999", true, &profile);
    EXPECT_FALSE(res.ok);
}

TEST_F(FullAttendanceTestSuite, TC65_ViewStudentProfileBackendUnavailable) {
    UserAccount profile;
    auto res = svc.viewStudentProfile("ST001", false, &profile);
    EXPECT_FALSE(res.ok);
    EXPECT_FALSE(res.message.empty());
}

TEST_F(FullAttendanceTestSuite, TC66_UpdateStudentProfileSuccess) {
    auto res = svc.updateStudentProfile("ST001", "Tran Minh ST001", "student@example.com", "0330000000", "District 5, HCMC");
    EXPECT_TRUE(res.ok);
    UserAccount reloaded;
    ASSERT_TRUE(svc.viewStudentProfile("ST001", true, &reloaded).ok);
    EXPECT_EQ(reloaded.email, "student@example.com");
    EXPECT_EQ(reloaded.phone, "0330000000");
}

TEST_F(FullAttendanceTestSuite, TC67_UpdateStudentProfileInvalidEmailRejected) {
    auto res = svc.updateStudentProfile("ST001", "Tran Minh ST001", "invalid-email", "0330000000", "District 5, HCMC");
    EXPECT_FALSE(res.ok);
}

TEST_F(FullAttendanceTestSuite, TC68_UpdateStudentProfileBlankNameRejected) {
    UserAccount before;
    ASSERT_TRUE(svc.viewStudentProfile("ST001", true, &before).ok);
    auto res = svc.updateStudentProfile("ST001", "   ", "student@example.com", "0330000000", "District 5, HCMC");
    EXPECT_FALSE(res.ok);
    UserAccount after;
    ASSERT_TRUE(svc.viewStudentProfile("ST001", true, &after).ok);
    EXPECT_EQ(after.full_name, before.full_name);
}

TEST_F(FullAttendanceTestSuite, TC69_UpdateStudentProfileScriptTreatedAsData) {
    string payload = "<script>alert(1)</script>";
    auto res = svc.updateStudentProfile("ST001", "Tran Minh ST001", "st001@attendance.edu.vn", "0901111111", payload);
    EXPECT_TRUE(res.ok);
    UserAccount reloaded;
    ASSERT_TRUE(svc.viewStudentProfile("ST001", true, &reloaded).ok);
    EXPECT_EQ(reloaded.address, payload);
}

TEST_F(FullAttendanceTestSuite, TC70_CourseRegistrationSuccess) {
    auto res = svc.registerStudentCourse("ST001", "CS201");
    EXPECT_TRUE(res.ok);
    auto list = svc.getRegisteredCourses("ST001");
    EXPECT_NE(find(list.begin(), list.end(), "CS201"), list.end());
}

TEST_F(FullAttendanceTestSuite, TC71_CourseRegistrationDuplicateRejected) {
    ASSERT_TRUE(svc.registerStudentCourse("ST001", "CS201").ok);
    auto res = svc.registerStudentCourse("ST001", "CS201");
    EXPECT_FALSE(res.ok);
    auto list = svc.getRegisteredCourses("ST001");
    EXPECT_EQ(count(list.begin(), list.end(), "CS201"), 1);
}

TEST_F(FullAttendanceTestSuite, TC72_CourseRegistrationOutsideWindowRejected) {
    auto res = svc.registerStudentCourse("ST001", "CS201", "2026-11-22", "2026-11-20");
    EXPECT_FALSE(res.ok);
}

TEST_F(FullAttendanceTestSuite, TC73_CourseRegistrationInvalidCourseRejected) {
    auto res = svc.registerStudentCourse("ST001", "INVALID");
    EXPECT_FALSE(res.ok);
}

TEST_F(FullAttendanceTestSuite, TC74_CourseRegistrationStoredCourseVisible) {
    auto list = svc.getRegisteredCourses("ST001");
    EXPECT_NE(find(list.begin(), list.end(), "PHI205"), list.end());
}

TEST_F(FullAttendanceTestSuite, TC75_CourseRegistrationEmptyStateForST002) {
    string empty_msg;
    auto list = svc.getRegisteredCourses("ST002", &empty_msg);
    EXPECT_TRUE(list.empty());
    EXPECT_FALSE(empty_msg.empty());
}

TEST_F(FullAttendanceTestSuite, TC76_CourseRegistrationPersistsCS202) {
    auto res = svc.registerStudentCourse("ST001", "CS202");
    EXPECT_TRUE(res.ok);
    auto list = svc.getRegisteredCourses("ST001");
    EXPECT_NE(find(list.begin(), list.end(), "CS202"), list.end());
}

TEST_F(FullAttendanceTestSuite, TC77_UnregisterCourseSuccess) {
    ASSERT_TRUE(svc.registerStudentCourse("ST001", "CS201").ok);
    auto res = svc.unregisterStudentCourse("ST001", "CS201", true);
    EXPECT_TRUE(res.ok);
    auto list = svc.getRegisteredCourses("ST001");
    EXPECT_EQ(find(list.begin(), list.end(), "CS201"), list.end());
}

TEST_F(FullAttendanceTestSuite, TC78_UnregisterCourseNotRegisteredRejected) {
    auto res = svc.unregisterStudentCourse("ST001", "CS305", true);
    EXPECT_FALSE(res.ok);
}

TEST_F(FullAttendanceTestSuite, TC79_UnregisterCourseCancelledPreserves) {
    ASSERT_TRUE(svc.registerStudentCourse("ST001", "CS201").ok);
    auto res = svc.unregisterStudentCourse("ST001", "CS201", false);
    EXPECT_FALSE(res.ok);
    auto list = svc.getRegisteredCourses("ST001");
    EXPECT_NE(find(list.begin(), list.end(), "CS201"), list.end());
}

TEST_F(FullAttendanceTestSuite, TC80_AttendanceCheckInValidSuccess) {
    ASSERT_TRUE(svc.registerStudentCourse("ST001", "CS201").ok);
    auto res = svc.studentCheckIn("ST001", "CS201", "S_CS201_13", true, true, true, "OPEN");
    EXPECT_TRUE(res.ok);
}

TEST_F(FullAttendanceTestSuite, TC81_AttendanceCheckInUnregisteredCourseRejected) {
    auto res = svc.studentCheckIn("ST001", "CS202", "S_CS202_1", true, true, true, "OPEN");
    EXPECT_FALSE(res.ok);
}

TEST_F(FullAttendanceTestSuite, TC82_AttendanceCheckInOutsideGpsRejected) {
    ASSERT_TRUE(svc.registerStudentCourse("ST001", "CS201").ok);
    auto res = svc.studentCheckIn("ST001", "CS201", "S_CS201_13", true, false, true, "OPEN");
    EXPECT_FALSE(res.ok);
}

TEST_F(FullAttendanceTestSuite, TC83_AttendanceCheckInClosedSessionRejected) {
    ASSERT_TRUE(svc.registerStudentCourse("ST001", "CS201").ok);
    auto res = svc.studentCheckIn("ST001", "CS201", "S_CS201_12", true, true, true, "CLOSED");
    EXPECT_FALSE(res.ok);
}

TEST_F(FullAttendanceTestSuite, TC84_AttendanceCheckInDuplicatePrevented) {
    ASSERT_TRUE(svc.registerStudentCourse("ST001", "CS201").ok);
    ASSERT_TRUE(svc.studentCheckIn("ST001", "CS201", "S_CS201_13", true, true, true, "OPEN").ok);
    auto res = svc.studentCheckIn("ST001", "CS201", "S_CS201_13", true, true, true, "OPEN");
    EXPECT_FALSE(res.ok);
}

TEST_F(FullAttendanceTestSuite, TC85_AttendanceCheckInGpsPermissionDenied) {
    ASSERT_TRUE(svc.registerStudentCourse("ST001", "CS201").ok);
    auto res = svc.studentCheckIn("ST001", "CS201", "S_CS201_13", false, true, true, "OPEN");
    EXPECT_FALSE(res.ok);
}

TEST_F(FullAttendanceTestSuite, TC86_AttendanceHistoryExistingRecords) {
    vector<AttendanceEntry> recs;
    auto res = svc.getStudentAttendanceHistory("ST001", "ST001", recs);
    EXPECT_TRUE(res.ok);
    ASSERT_FALSE(recs.empty());
    EXPECT_EQ(recs[0].course_id, "PHI205");
}

TEST_F(FullAttendanceTestSuite, TC87_AttendanceHistoryNoRecordsEmptyState) {
    vector<AttendanceEntry> recs;
    auto res = svc.getStudentAttendanceHistory("ST002", "ST002", recs);
    EXPECT_TRUE(res.ok);
    EXPECT_TRUE(recs.empty());
}

TEST_F(FullAttendanceTestSuite, TC88_AttendanceHistoryUnauthorizedDenied) {
    vector<AttendanceEntry> recs;
    auto res = svc.getStudentAttendanceHistory("ST001", "ST002", recs);
    EXPECT_FALSE(res.ok);
    EXPECT_TRUE(recs.empty());
}

TEST_F(FullAttendanceTestSuite, TC89_AttendanceRate80Percent) {
    auto summary = AttendanceSystemEngine::calculateAttendanceRate(8, 10);
    EXPECT_TRUE(summary.division_by_zero_safe);
    EXPECT_FALSE(summary.empty_state);
    EXPECT_NEAR(summary.rate, 80.0, 1e-6);
}

TEST_F(FullAttendanceTestSuite, TC90_AttendanceRateZeroSessionsSafe) {
    auto summary = AttendanceSystemEngine::calculateAttendanceRate(0, 0);
    EXPECT_TRUE(summary.division_by_zero_safe);
    EXPECT_TRUE(summary.empty_state);
    EXPECT_NEAR(summary.rate, 0.0, 1e-6);
}

TEST_F(FullAttendanceTestSuite, TC91_AttendanceRateMixedStatusesPolicy) {
    auto summary = AttendanceSystemEngine::calculatePolicyAttendanceRate(6, 3, 1);
    EXPECT_TRUE(summary.division_by_zero_safe);
    EXPECT_FALSE(summary.empty_state);
    EXPECT_NEAR(summary.rate, 70.0, 1e-6);
}

} // namespace attendance_full_test