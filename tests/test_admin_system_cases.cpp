#include <gtest/gtest.h>

#include <algorithm>
#include <cctype>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace admin_system_test {

struct ActionResult {
    bool ok = false;
    std::string message;
};

struct AccountItem {
    std::string full_name;
    std::string username;
    std::string email;
    std::string password;
    std::string role;
    std::string status = "Active";
    bool has_academic_records = false;
};

struct CourseItem {
    std::string code;
    std::string name;
    int credits = 0;
    bool open_for_registration = true;
    bool has_related_records = false;
};

struct RegistrationItem {
    std::string student_id;
    std::string course_code;
};

struct AssignmentItem {
    std::string course_code;
    std::string lecturer_id;
};

struct DepartmentItem {
    std::string code;
    std::string name;
};

struct ClassSessionItem {
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

class AdminSystemService {
public:
    AdminSystemService() {
        accounts_["khang.le"] = {"Le Minh Khang", "khang.le", "khang.le@attendance.edu.vn", "Admin@123", "Administrator", "Active", false};
        accounts_["student01"] = {"Nguyen Van Student", "student01", "student01@attendance.edu.vn", "123456", "Student", "Active", true};
        accounts_["test.student01"] = {"Test Student One", "test.student01", "test.student01@attendance.edu.vn", "Pass@123", "Student", "Active", false};
        accounts_["test.student02"] = {"Test Student Two", "test.student02", "test.student02@attendance.edu.vn", "Pass@123", "Student", "Active", false};
        accounts_["012345678902"] = {"Lecturer Assigned", "012345678902", "lec02@attendance.edu.vn", "Lec@123", "Lecturer", "Active", true};

        courses_["CS101"] = {"CS101", "Introduction to Programming", 3, true, true};
        courses_["CS303"] = {"CS303", "Discrete Structures", 3, true, false};
        courses_["CS999"] = {"CS999", "Closed Elective", 3, false, false};

        sessions_.push_back({"202612010001", "CS101", "2026-12-01", "08:00", "10:00", "A101"});

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

    ActionResult forgotPassword(const std::string& email) const {
        if (!isValidEmail(email)) {
            return {false, "Invalid email format."};
        }
        for (const auto& kv : accounts_) {
            if (kv.second.email == email) {
                return {true, "Password-reset instructions have been sent to the registered email address."};
            }
        }
        return {false, "If the email is registered, reset instructions have been sent."};
    }

    ActionResult changePassword(const std::string& username,
                                const std::string& current_password,
                                const std::string& new_password,
                                const std::string& confirm_password) {
        auto it = accounts_.find(username);
        if (it == accounts_.end() || it->second.password != current_password) {
            return {false, "Current password is incorrect."};
        }
        if (new_password != confirm_password) {
            return {false, "Passwords do not match."};
        }
        it->second.password = new_password;
        return {true, "Password updated successfully."};
    }

    ActionResult createAccount(const std::string& full_name,
                               const std::string& username,
                               const std::string& email,
                               const std::string& role,
                               const std::string& password) {
        if (isBlankOrWhitespace(full_name) ||
            isBlankOrWhitespace(username) ||
            isBlankOrWhitespace(email) ||
            isBlankOrWhitespace(role) ||
            isBlankOrWhitespace(password)) {
            return {false, "Required field is empty."};
        }
        if (accounts_.count(username) > 0) {
            return {false, "Username already exists."};
        }
        accounts_[username] = {full_name, username, email, password, role, "Active", false};
        return {true, "Account created successfully."};
    }

    std::vector<AccountItem> searchAccounts(const std::string& keyword) const {
        std::vector<AccountItem> result;
        for (const auto& kv : accounts_) {
            if (keyword.empty() || kv.first.find(keyword) != std::string::npos) {
                result.push_back(kv.second);
            }
        }
        return result;
    }

    ActionResult updateAccount(const std::string& username,
                               const std::string& new_full_name,
                               const std::string& new_email) {
        if (isBlankOrWhitespace(username) || isBlankOrWhitespace(new_full_name)) {
            return {false, "Required field is empty."};
        }
        if (!isValidEmail(new_email)) {
            return {false, "Invalid email format."};
        }
        auto it = accounts_.find(username);
        if (it == accounts_.end()) {
            return {false, "Account not found."};
        }
        it->second.full_name = new_full_name;
        it->second.email = new_email;
        return {true, "Account updated successfully."};
    }

    ActionResult deleteAccount(const std::string& username, bool confirm) {
        if (!confirm) {
            return {false, "Deletion cancelled."};
        }
        auto it = accounts_.find(username);
        if (it == accounts_.end()) {
            return {false, "Account not found."};
        }
        if (it->second.has_academic_records) {
            it->second.status = "Inactive";
            return {false, "Cannot delete account with related academic records; deactivated instead."};
        }
        accounts_.erase(it);
        return {true, "Account deleted successfully."};
    }

    ActionResult createCourse(const std::string& code,
                              const std::string& name,
                              int credits) {
        if (courses_.count(code) > 0) {
            return {false, "Course code already exists."};
        }
        if (credits <= 0) {
            return {false, "Credits must be a positive number."};
        }
        if (isBlankOrWhitespace(code) || isBlankOrWhitespace(name)) {
            return {false, "Required field is empty."};
        }
        courses_[code] = {code, name, credits, true, false};
        return {true, "Course created successfully."};
    }

    ActionResult updateCourse(const std::string& code, const std::string& new_name) {
        auto it = courses_.find(code);
        if (it == courses_.end()) {
            return {false, "Course not found."};
        }
        if (isBlankOrWhitespace(new_name)) {
            return {false, "Course name is required."};
        }
        it->second.name = new_name;
        return {true, "Course updated successfully."};
    }

    ActionResult deleteCourse(const std::string& code, bool confirm) {
        if (!confirm) {
            return {false, "Deletion cancelled."};
        }
        auto it = courses_.find(code);
        if (it == courses_.end()) {
            return {false, "Course not found."};
        }
        if (it->second.has_related_records) {
            return {false, "Cannot delete course because it has related registrations and sessions."};
        }
        courses_.erase(it);
        return {true, "Course deleted successfully."};
    }

    ActionResult addCourseRegistration(const std::string& student_id,
                                       const std::string& course_code) {
        auto it = courses_.find(course_code);
        if (it == courses_.end() || !it->second.open_for_registration) {
            return {false, "Course is closed or unavailable for registration."};
        }
        for (const auto& reg : registrations_) {
            if (reg.student_id == student_id && reg.course_code == course_code) {
                return {false, "Duplicate course registration is not allowed."};
            }
        }
        registrations_.push_back({student_id, course_code});
        it->second.has_related_records = true;
        return {true, "Course registration created successfully."};
    }

    ActionResult assignLecturer(const std::string& course_code,
                                const std::string& lecturer_id) {
        if (courses_.find(course_code) == courses_.end()) {
            return {false, "Course not found."};
        }
        auto acc_it = accounts_.find(lecturer_id);
        if (acc_it == accounts_.end() || acc_it->second.role != "Lecturer" || acc_it->second.status != "Active") {
            return {false, "Lecturer does not exist or is inactive."};
        }
        for (const auto& item : assignments_) {
            if (item.course_code == course_code && item.lecturer_id == lecturer_id) {
                return {false, "Duplicate lecturer assignment is not allowed."};
            }
        }
        assignments_.push_back({course_code, lecturer_id});
        return {true, "Lecturer assigned to course successfully."};
    }

    ActionResult createDepartment(const std::string& code, const std::string& name) {
        if (departments_.count(code) > 0) {
            return {false, "Department code already exists."};
        }
        if (isBlankOrWhitespace(name)) {
            return {false, "Invalid department name."};
        }
        departments_[code] = {code, name};
        return {true, "Department created successfully."};
    }

    ActionResult createClassSession(const std::string& course_code,
                                    const std::string& date,
                                    const std::string& start_time,
                                    const std::string& end_time,
                                    const std::string& room) {
        if (courses_.find(course_code) == courses_.end()) {
            return {false, "Course not found."};
        }
        int start_min = timeToMinutes(start_time);
        int end_min = timeToMinutes(end_time);
        if (start_min < 0 || end_min < 0 || end_min <= start_min) {
            return {false, "End time must be later than start time."};
        }
        for (const auto& s : sessions_) {
            if (s.room == room && s.date == date) {
                int s_start = timeToMinutes(s.start_time);
                int s_end = timeToMinutes(s.end_time);
                if (start_min < s_end && end_min > s_start) {
                    return {false, "Schedule conflict in the same room."};
                }
            }
        }
        std::string new_id = date + "_" + room + "_" + std::to_string(sessions_.size() + 1);
        sessions_.push_back({new_id, course_code, date, start_time, end_time, room});
        return {true, "Class session created successfully."};
    }

    ActionResult viewAttendanceReports(const std::string& course_code,
                                     const std::string& from_date,
                                     const std::string& to_date,
                                     std::vector<AttendanceReportRow>& out_rows) const {
        out_rows.clear();
        if (from_date > to_date) {
            return {false, "From date must not be later than to date."};
        }
        for (const auto& row : reports_) {
            if (row.course_code == course_code && row.date >= from_date && row.date <= to_date) {
                out_rows.push_back(row);
            }
        }
        if (out_rows.empty()) {
            return {true, "No attendance records found."};
        }
        return {true, "Attendance report generated successfully."};
    }

    ActionResult searchChangeHistoryById(const std::string& id_filter,
                                         std::vector<ChangeHistoryItem>& out_items) const {
        out_items.clear();
        for (const auto& item : history_) {
            if (item.student_id == id_filter || item.session_id == id_filter) {
                out_items.push_back(item);
            }
        }
        if (out_items.empty()) {
            return {false, "No records found."};
        }
        return {true, "Change history records found."};
    }

    ActionResult searchChangeHistoryByDateRange(const std::string& from_date,
                                              const std::string& to_date,
                                              std::vector<ChangeHistoryItem>& out_items) const {
        out_items.clear();
        if (from_date > to_date) {
            return {false, "Invalid date range: start date is later than end date."};
        }
        for (const auto& item : history_) {
            std::string item_date = item.timestamp.substr(0, 10);
            if (item_date >= from_date && item_date <= to_date) {
                out_items.push_back(item);
            }
        }
        return {true, "Change history filtered by date range."};
    }

    bool hasCourse(const std::string& code) const {
        return courses_.count(code) > 0;
    }

    std::string getCourseName(const std::string& code) const {
        auto it = courses_.find(code);
        return (it != courses_.end()) ? it->second.name : "";
    }

    std::string getAccountFullName(const std::string& username) const {
        auto it = accounts_.find(username);
        return (it != accounts_.end()) ? it->second.full_name : "";
    }

    std::string getAccountStatus(const std::string& username) const {
        auto it = accounts_.find(username);
        return (it != accounts_.end()) ? it->second.status : "";
    }

    std::string getAccountPassword(const std::string& username) const {
        auto it = accounts_.find(username);
        return (it != accounts_.end()) ? it->second.password : "";
    }

private:
    std::map<std::string, AccountItem> accounts_;
    std::map<std::string, CourseItem> courses_;
    std::vector<RegistrationItem> registrations_;
    std::vector<AssignmentItem> assignments_;
    std::map<std::string, DepartmentItem> departments_;
    std::vector<ClassSessionItem> sessions_;
    std::vector<AttendanceReportRow> reports_;
    std::vector<ChangeHistoryItem> history_;
};

class AdminSystemTestSuite : public ::testing::Test {
protected:
    AdminSystemService svc;
};

// Row 1 - Forgot Password: Registered email
TEST_F(AdminSystemTestSuite, TC01_ForgotPasswordRegisteredEmail) {
    auto res = svc.forgotPassword("khang.le@attendance.edu.vn");
    EXPECT_TRUE(res.ok);
    EXPECT_FALSE(res.message.empty());
}

// Row 2 - Forgot Password: Unregistered email
TEST_F(AdminSystemTestSuite, TC02_ForgotPasswordUnregisteredEmail) {
    auto res = svc.forgotPassword("unknown@example.com");
    EXPECT_FALSE(res.ok);
    EXPECT_FALSE(res.message.empty());
}

// Row 3 - Forgot Password: Invalid email format
TEST_F(AdminSystemTestSuite, TC03_ForgotPasswordInvalidEmailFormat) {
    auto res = svc.forgotPassword("admin-email");
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(res.message, "Invalid email format.");
}

// Row 4 - Change Password: Valid current and matching new password
TEST_F(AdminSystemTestSuite, TC04_ChangePasswordSuccess) {
    auto res = svc.changePassword("khang.le", "Admin@123", "NewAdmin@123", "NewAdmin@123");
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(svc.getAccountPassword("khang.le"), "NewAdmin@123");
}

// Row 5 - Change Password: Incorrect current password
TEST_F(AdminSystemTestSuite, TC05_ChangePasswordIncorrectCurrent) {
    auto res = svc.changePassword("khang.le", "Wrong123", "NewAdmin@123", "NewAdmin@123");
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(svc.getAccountPassword("khang.le"), "Admin@123");
}

// Row 6 - Change Password: New password and confirm password mismatch
TEST_F(AdminSystemTestSuite, TC06_ChangePasswordMismatch) {
    auto res = svc.changePassword("khang.le", "Admin@123", "NewAdmin@123", "Different123");
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(res.message, "Passwords do not match.");
    EXPECT_EQ(svc.getAccountPassword("khang.le"), "Admin@123");
}

// Row 7 - Create Account: Valid Student account
TEST_F(AdminSystemTestSuite, TC07_CreateAccountStudentSuccess) {
    auto res = svc.createAccount("Nguyen Van An", "an.nguyen", "an.nguyen@gmail.com", "Student", "An@123456");
    EXPECT_TRUE(res.ok);
    auto found = svc.searchAccounts("an.nguyen");
    ASSERT_EQ(found.size(), 1u);
    EXPECT_EQ(found[0].role, "Student");
}

// Row 8 - Create Account: Duplicate username
TEST_F(AdminSystemTestSuite, TC08_CreateAccountDuplicateUsername) {
    ASSERT_TRUE(svc.createAccount("Nguyen Van An", "an.nguyen", "an.nguyen@gmail.com", "Student", "An@123456").ok);
    auto res = svc.createAccount("Tran Van B", "an.nguyen", "b.tran@gmail.com", "Lecturer", "B@123456");
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(res.message, "Username already exists.");
}

// Row 9 - Create Account: Empty required field (username)
TEST_F(AdminSystemTestSuite, TC09_CreateAccountEmptyUsernameRejected) {
    auto res = svc.createAccount("Nguyen Van C", "", "c.nguyen@gmail.com", "Student", "C@123456");
    EXPECT_FALSE(res.ok);
    EXPECT_TRUE(svc.searchAccounts("c.nguyen").empty());
}

// Row 10 - View/Search Account: Existing username keyword
TEST_F(AdminSystemTestSuite, TC10_SearchAccountExistingKeyword) {
    ASSERT_TRUE(svc.createAccount("Nguyen Van An", "an.nguyen", "an.nguyen@gmail.com", "Student", "An@123456").ok);
    auto list = svc.searchAccounts("an.nguyen");
    ASSERT_EQ(list.size(), 1u);
    EXPECT_EQ(list[0].full_name, "Nguyen Van An");
    EXPECT_EQ(list[0].email, "an.nguyen@gmail.com");
}

// Row 11 - View/Search Account: Non-existent keyword
TEST_F(AdminSystemTestSuite, TC11_SearchAccountNoMatchingFound) {
    auto list = svc.searchAccounts("unknown999");
    EXPECT_TRUE(list.empty());
}

// Row 12 - View/Search Account: Empty keyword returns all accounts
TEST_F(AdminSystemTestSuite, TC12_SearchAccountEmptyReturnsAll) {
    auto all_accounts = svc.searchAccounts("");
    EXPECT_GE(all_accounts.size(), 5u);
}

// Row 13 - Update Account: Valid full name update
TEST_F(AdminSystemTestSuite, TC13_UpdateAccountFullNameSuccess) {
    ASSERT_TRUE(svc.createAccount("Nguyen Van An", "an.nguyen", "an.nguyen@gmail.com", "Student", "An@123456").ok);
    auto res = svc.updateAccount("an.nguyen", "Nguyen Van An Updated", "an.nguyen@gmail.com");
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(svc.getAccountFullName("an.nguyen"), "Nguyen Van An Updated");
}

// Row 14 - Update Account: Invalid email format
TEST_F(AdminSystemTestSuite, TC14_UpdateAccountInvalidEmailRejected) {
    ASSERT_TRUE(svc.createAccount("Nguyen Van An", "an.nguyen", "an.nguyen@gmail.com", "Student", "An@123456").ok);
    auto res = svc.updateAccount("an.nguyen", "Nguyen Van An", "invalid-email");
    EXPECT_FALSE(res.ok);
    auto list = svc.searchAccounts("an.nguyen");
    ASSERT_EQ(list.size(), 1u);
    EXPECT_EQ(list[0].email, "an.nguyen@gmail.com");
}

// Row 15 - Update Account: Empty required field (username)
TEST_F(AdminSystemTestSuite, TC15_UpdateAccountEmptyUsernameRejected) {
    ASSERT_TRUE(svc.createAccount("Nguyen Van An", "an.nguyen", "an.nguyen@gmail.com", "Student", "An@123456").ok);
    auto res = svc.updateAccount("", "Nguyen Van An", "an.nguyen@gmail.com");
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(svc.getAccountFullName("an.nguyen"), "Nguyen Van An");
}

// Row 16 - Delete Account: Confirm deletion of account without related records
TEST_F(AdminSystemTestSuite, TC16_DeleteAccountConfirmedSuccess) {
    auto res = svc.deleteAccount("test.student01", true);
    EXPECT_TRUE(res.ok);
    EXPECT_TRUE(svc.searchAccounts("test.student01").empty());
}

// Row 17 - Delete Account: Cancel confirmation dialog
TEST_F(AdminSystemTestSuite, TC17_DeleteAccountCancelledKeepsAccount) {
    auto res = svc.deleteAccount("test.student02", false);
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(svc.searchAccounts("test.student02").size(), 1u);
}

// Row 18 - Delete Account: Account with existing academic records
TEST_F(AdminSystemTestSuite, TC18_DeleteAccountWithAcademicRecordsProtected) {
    auto res = svc.deleteAccount("student01", true);
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(svc.getAccountStatus("student01"), "Inactive");
}

// Row 19 - Create Course: Valid course
TEST_F(AdminSystemTestSuite, TC19_CreateCourseSuccess) {
    auto res = svc.createCourse("CS102", "Introduction to Programming", 3);
    EXPECT_TRUE(res.ok);
    EXPECT_TRUE(svc.hasCourse("CS102"));
}

// Row 20 - Create Course: Duplicate course code
TEST_F(AdminSystemTestSuite, TC20_CreateCourseDuplicateCodeRejected) {
    auto res = svc.createCourse("CS101", "Advanced Programming", 3);
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(res.message, "Course code already exists.");
}

// Row 21 - Create Course: Negative credits rejected
TEST_F(AdminSystemTestSuite, TC21_CreateCourseNegativeCreditsRejected) {
    auto res = svc.createCourse("CS202", "Database Systems", -1);
    EXPECT_FALSE(res.ok);
    EXPECT_FALSE(svc.hasCourse("CS202"));
}

// Row 22 - Update Course: Valid course name update
TEST_F(AdminSystemTestSuite, TC22_UpdateCourseNameSuccess) {
    auto res = svc.updateCourse("CS101", "Programming Fundamentals");
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(svc.getCourseName("CS101"), "Programming Fundamentals");
}

// Row 23 - Update Course: Empty course name rejected
TEST_F(AdminSystemTestSuite, TC23_UpdateCourseEmptyNameRejected) {
    auto res = svc.updateCourse("CS101", "");
    EXPECT_FALSE(res.ok);
    EXPECT_EQ(svc.getCourseName("CS101"), "Introduction to Programming");
}

// Row 24 - Update Course: Maximum length 255-character course name
TEST_F(AdminSystemTestSuite, TC24_UpdateCourseMaxLength255Chars) {
    std::string max_name(255, 'A');
    auto res = svc.updateCourse("CS101", max_name);
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(svc.getCourseName("CS101").size(), 255u);
}

// Row 25 - Delete Course: Course without related records
TEST_F(AdminSystemTestSuite, TC25_DeleteCourseWithoutRelatedRecordsSuccess) {
    auto res = svc.deleteCourse("CS303", true);
    EXPECT_TRUE(res.ok);
    EXPECT_FALSE(svc.hasCourse("CS303"));
}

// Row 26 - Delete Course: Cancel deletion
TEST_F(AdminSystemTestSuite, TC26_DeleteCourseCancelledKeepsCourse) {
    auto res = svc.deleteCourse("CS101", false);
    EXPECT_FALSE(res.ok);
    EXPECT_TRUE(svc.hasCourse("CS101"));
}

// Row 27 - Delete Course: Course with existing registrations and sessions
TEST_F(AdminSystemTestSuite, TC27_DeleteCourseWithRelatedRecordsProtected) {
    auto res = svc.deleteCourse("CS101", true);
    EXPECT_FALSE(res.ok);
    EXPECT_TRUE(svc.hasCourse("CS101"));
}

// Row 28 - Add Course Registration: Valid student and open course
TEST_F(AdminSystemTestSuite, TC28_AddCourseRegistrationSuccess) {
    auto res = svc.addCourseRegistration("012345678901", "CS101");
    EXPECT_TRUE(res.ok);
}

// Row 29 - Add Course Registration: Duplicate registration rejected
TEST_F(AdminSystemTestSuite, TC29_AddCourseRegistrationDuplicateRejected) {
    ASSERT_TRUE(svc.addCourseRegistration("012345678901", "CS101").ok);
    auto res = svc.addCourseRegistration("012345678901", "CS101");
    EXPECT_FALSE(res.ok);
}

// Row 30 - Add Course Registration: Closed course rejected
TEST_F(AdminSystemTestSuite, TC30_AddCourseRegistrationClosedCourseRejected) {
    auto res = svc.addCourseRegistration("012345678901", "CS999");
    EXPECT_FALSE(res.ok);
}

// Row 31 - Assign Lecturer: Valid course and active lecturer
TEST_F(AdminSystemTestSuite, TC31_AssignLecturerSuccess) {
    auto res = svc.assignLecturer("CS101", "012345678902");
    EXPECT_TRUE(res.ok);
}

// Row 32 - Assign Lecturer: Non-existent lecturer ID rejected
TEST_F(AdminSystemTestSuite, TC32_AssignLecturerNonExistentRejected) {
    auto res = svc.assignLecturer("CS101", "999999999999");
    EXPECT_FALSE(res.ok);
}

// Row 33 - Assign Lecturer: Duplicate lecturer assignment rejected
TEST_F(AdminSystemTestSuite, TC33_AssignLecturerDuplicateRejected) {
    ASSERT_TRUE(svc.assignLecturer("CS101", "012345678902").ok);
    auto res = svc.assignLecturer("CS101", "012345678902");
    EXPECT_FALSE(res.ok);
}

// Row 34 - Create Department: Valid unique code and name
TEST_F(AdminSystemTestSuite, TC34_CreateDepartmentSuccess) {
    auto res = svc.createDepartment("IT", "Information Technology");
    EXPECT_TRUE(res.ok);
}

// Row 35 - Create Department: Duplicate department code rejected
TEST_F(AdminSystemTestSuite, TC35_CreateDepartmentDuplicateCodeRejected) {
    ASSERT_TRUE(svc.createDepartment("IT", "Information Technology").ok);
    auto res = svc.createDepartment("IT", "Information Systems");
    EXPECT_FALSE(res.ok);
}

// Row 36 - Create Department: Whitespace-only department name rejected
TEST_F(AdminSystemTestSuite, TC36_CreateDepartmentSpacesOnlyRejected) {
    auto res = svc.createDepartment("HR", "   ");
    EXPECT_FALSE(res.ok);
}

// Row 37 - Create Class Session: Valid schedule
TEST_F(AdminSystemTestSuite, TC37_CreateClassSessionSuccess) {
    auto res = svc.createClassSession("CS101", "2026-12-01", "10:00", "12:00", "A101");
    EXPECT_TRUE(res.ok);
}

// Row 38 - Create Class Session: Overlapping session in same room rejected
TEST_F(AdminSystemTestSuite, TC38_CreateClassSessionOverlapRejected) {
    auto res = svc.createClassSession("CS101", "2026-12-01", "09:00", "11:00", "A101");
    EXPECT_FALSE(res.ok);
}

// Row 39 - Create Class Session: End time earlier than start time rejected
TEST_F(AdminSystemTestSuite, TC39_CreateClassSessionEndBeforeStartRejected) {
    auto res = svc.createClassSession("CS101", "2026-12-01", "10:00", "08:00", "A101");
    EXPECT_FALSE(res.ok);
}

// Row 40 - View Attendance Reports: Valid course and date range with records
TEST_F(AdminSystemTestSuite, TC40_ViewAttendanceReportsWithRecords) {
    std::vector<AttendanceReportRow> rows;
    auto res = svc.viewAttendanceReports("CS101", "2026-12-01", "2026-12-31", rows);
    EXPECT_TRUE(res.ok);
    EXPECT_EQ(rows.size(), 2u);
}

// Row 41 - View Attendance Reports: Date range with no records
TEST_F(AdminSystemTestSuite, TC41_ViewAttendanceReportsEmptyRange) {
    std::vector<AttendanceReportRow> rows;
    auto res = svc.viewAttendanceReports("CS101", "2027-01-01", "2027-01-02", rows);
    EXPECT_TRUE(res.ok);
    EXPECT_TRUE(rows.empty());
}

// Row 42 - View Attendance Reports: Start date later than end date rejected
TEST_F(AdminSystemTestSuite, TC42_ViewAttendanceReportsInvalidRangeRejected) {
    std::vector<AttendanceReportRow> rows;
    auto res = svc.viewAttendanceReports("CS101", "2026-12-31", "2026-12-01", rows);
    EXPECT_FALSE(res.ok);
}

// Row 43 - View Attendance Change History: Existing student ID / session ID
TEST_F(AdminSystemTestSuite, TC43_ViewChangeHistoryMatchingId) {
    std::vector<ChangeHistoryItem> by_student;
    auto res_student = svc.searchChangeHistoryById("012345678901", by_student);
    EXPECT_TRUE(res_student.ok);
    ASSERT_EQ(by_student.size(), 1u);
    EXPECT_EQ(by_student[0].session_id, "202612010001");

    std::vector<ChangeHistoryItem> by_session;
    auto res_session = svc.searchChangeHistoryById("202612010001", by_session);
    EXPECT_TRUE(res_session.ok);
    ASSERT_EQ(by_session.size(), 1u);
}

// Row 44 - View Attendance Change History: Non-existent ID returns no records
TEST_F(AdminSystemTestSuite, TC44_ViewChangeHistoryNoMatchingId) {
    std::vector<ChangeHistoryItem> items;
    auto res = svc.searchChangeHistoryById("999999999999", items);
    EXPECT_FALSE(res.ok);
    EXPECT_TRUE(items.empty());
}

// Row 45 - View Attendance Change History: Start date later than end date rejected
TEST_F(AdminSystemTestSuite, TC45_ViewChangeHistoryInvalidDateRangeRejected) {
    std::vector<ChangeHistoryItem> items;
    auto res = svc.searchChangeHistoryByDateRange("2026-12-31", "2026-12-01", items);
    EXPECT_FALSE(res.ok);
}

} // namespace admin_system_test