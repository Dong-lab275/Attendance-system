
#include <iostream>
#include <string>
#include "../AdminUserModule/database/DatabaseConnection.h"
#include "../AdminUserModule/controllers/UserController.h"
#include "../AdminUserModule/controllers/AdministratorController.h"

using namespace std;

void testUser(UserController& userController)
{
    int choice;

    do {
        cout << "\n========== USER MANAGEMENT ==========\n";
        cout << "1. Login\n";
        cout << "2. Change Password\n";
        cout << "3. Forgot Password\n";
        cout << "0. Back\n";
        cout << "Choose: ";

        cin >> choice;
        cin.ignore(10000, '\n');

        string username, password, oldPassword;
        string newPassword, confirmPassword, email;

        switch (choice) {
        case 1: {
            cout << "Username: ";
            getline(cin, username);

            cout << "Password: ";
            getline(cin, password);

            User* user = userController.login(username, password);

            if (user != nullptr) {
                cout << "LOGIN: PASS\n";
                cout << "User ID: " << user->getUserId() << endl;
                delete user;
            }
            else {
                cout << "LOGIN: FAIL\n";
            }

            break;
        }

        case 2:
            cout << "Old password: ";
            getline(cin, oldPassword);

            cout << "New password: ";
            getline(cin, newPassword);

            cout << "Confirm new password: ";
            getline(cin, confirmPassword);

            cout << (userController.changePassword(
                oldPassword,
                newPassword,
                confirmPassword
            ) ? "PASS\n" : "FAIL\n");

            break;

        case 3:
            cout << "Enter email: ";
            getline(cin, email);

            cout << (userController.forgotPassword(email)
                ? "PASS\n"
                : "FAIL\n");

            break;

        case 0:
            break;

        default:
            cout << "Invalid choice!\n";
        }

    } while (choice != 0);
}

void testAdministrator(AdministratorController& admin)
{
    int choice;

    do {
        cout << "\n========== ADMINISTRATOR TEST ==========\n";
        cout << "1. Create User Account\n";
        cout << "2. View User Accounts\n";
        cout << "3. Update Username\n";
        cout << "4. Delete User Account\n";
        cout << "5. Add Department\n";
        cout << "6. View Departments\n";
        cout << "7. Update Department\n";
        cout << "8. Delete Department\n";
        cout << "9. Add Course\n";
        cout << "10. View Courses\n";
        cout << "11. Update Course\n";
        cout << "12. Delete Course\n";
        cout << "13. Add Course Enrollment\n";
        cout << "14. View Course Enrollments\n";
        cout << "15. Delete Course Enrollment\n";
        cout << "16. Assign Lecturer To Course\n";
        cout << "17. Unassign Lecturer From Course\n";
        cout << "18. Add Class Session\n";
        cout << "19. View Class Sessions\n";
        cout << "20. Update Class Session\n";
        cout << "21. Delete Class Session\n";
        cout << "22. View Class Session Calendar\n";
        cout << "23. View Attendance History\n";
        cout << "24. View Attendance Reports\n";
        cout << "0. Back\n";
        cout << "Choose: ";

        cin >> choice;
        cin.ignore(10000, '\n');

        string userId, profileId, username, password, role;
        string fullName, email, phone, address, dob, gender, departmentId;
        string courseId, courseName, description, academicYear, lecturerId;
        string registrationId, sessionId, classDate, startTime, endTime;
        string classroom, departmentName;
        int semester;

        switch (choice) {

        case 1:
            cout << "User ID: ";
            getline(cin, userId);
            cout << "Profile ID: ";
            getline(cin, profileId);
            cout << "Username: ";
            getline(cin, username);
            cout << "Password: ";
            getline(cin, password);
            cout << "Role (Student/Lecturer/Administrator): ";
            getline(cin, role);
            cout << "Full name: ";
            getline(cin, fullName);
            cout << "Email: ";
            getline(cin, email);
            cout << "Phone: ";
            getline(cin, phone);

            if (role == "Student") {
                cout << "Address: ";
                getline(cin, address);
                cout << "Date of birth (YYYY-MM-DD): ";
                getline(cin, dob);
                cout << "Gender: ";
                getline(cin, gender);
            }
            else if (role == "Lecturer") {
                cout << "Date of birth (YYYY-MM-DD): ";
                getline(cin, dob);
                cout << "Gender: ";
                getline(cin, gender);
                cout << "Department ID: ";
                getline(cin, departmentId);
            }

            cout << (admin.createUserAccount(
            userId, username, password, role,
            fullName, email, phone, address, dob, gender,
            departmentId
        ) ? "PASS\n" : "FAIL\n");
            break;

        case 2:
            admin.viewUserAccounts();
            break;

        case 3:
            cout << "User ID: ";
            getline(cin, userId);
            cout << "New username: ";
            getline(cin, username);
            cout << (admin.updateUserAccount(userId, username) ? "PASS\n" : "FAIL\n");
            break;

        case 4:
            cout << "User ID: ";
            getline(cin, userId);
            cout << "Are you sure? (Y/N): ";
            {
                char confirm;
                cin >> confirm;
                cin.ignore(10000, '\n');

                if (confirm == 'Y' || confirm == 'y')
                    cout << (admin.deleteUserAccount(userId) ? "PASS\n" : "FAIL\n");
                else
                    cout << "Cancelled.\n";
            }
            break;

        case 5:
            cout << "Department ID: ";
            getline(cin, departmentId);
            cout << "Department name: ";
            getline(cin, departmentName);
            cout << (admin.addDepartment(departmentId, departmentName) ? "PASS\n" : "FAIL\n");
            break;

        case 6:
            admin.viewDepartments();
            break;

        case 7:
            cout << "Department ID: ";
            getline(cin, departmentId);
            cout << "New department name: ";
            getline(cin, departmentName);
            cout << (admin.updateDepartment(departmentId, departmentName) ? "PASS\n" : "FAIL\n");
            break;

        case 8:
            cout << "Department ID: ";
            getline(cin, departmentId);
            cout << (admin.deleteDepartment(departmentId) ? "PASS\n" : "FAIL\n");
            break;

        case 9:
            cout << "Course ID: ";
            getline(cin, courseId);
            cout << "Course name: ";
            getline(cin, courseName);
            cout << "Description: ";
            getline(cin, description);
            cout << "Academic year: ";
            getline(cin, academicYear);
            cout << "Semester (0 = NULL): ";
            cin >> semester;
            cin.ignore(10000, '\n');
            cout << "Lecturer ID (empty if none): ";
            getline(cin, lecturerId);

            cout << (admin.addCourse(courseId, courseName, description,
                academicYear, semester, lecturerId) ? "PASS\n" : "FAIL\n");
            break;

        case 10:
            admin.viewCourses();
            break;

        case 11:
            cout << "Course ID: ";
            getline(cin, courseId);
            cout << "New course name: ";
            getline(cin, courseName);
            cout << "Description: ";
            getline(cin, description);
            cout << "Academic year: ";
            getline(cin, academicYear);
            cout << "Semester (0 = NULL): ";
            cin >> semester;
            cin.ignore(10000, '\n');
            cout << "Lecturer ID (empty if none): ";
            getline(cin, lecturerId);

            cout << (admin.updateCourse(courseId, courseName, description,
                academicYear, semester, lecturerId) ? "PASS\n" : "FAIL\n");
            break;

        case 12:
            cout << "Course ID: ";
            getline(cin, courseId);
            cout << (admin.deleteCourse(courseId) ? "PASS\n" : "FAIL\n");
            break;

        case 13:
            cout << "Course ID: ";
            getline(cin, courseId);
            cout << "Student ID: ";
            getline(cin, userId);
            cout << (admin.addCourseEnrollment(courseId, userId) ? "PASS\n" : "FAIL\n");
            break;

        case 14:
            admin.viewCourseEnrollments();
            break;

        case 15:
            cout << "Registration ID: ";
            getline(cin, registrationId);
            cout << (admin.deleteCourseEnrollment(registrationId) ? "PASS\n" : "FAIL\n");
            break;

        case 16:
            cout << "Course ID: ";
            getline(cin, courseId);
            cout << "Lecturer ID: ";
            getline(cin, lecturerId);
            cout << (admin.assignLecturerToCourse(courseId, lecturerId) ? "PASS\n" : "FAIL\n");
            break;

        case 17:
            cout << "Course ID: ";
            getline(cin, courseId);
            cout << (admin.unassignLecturerFromCourse(courseId) ? "PASS\n" : "FAIL\n");
            break;

        case 18:
            cout << "Session ID: ";
            getline(cin, sessionId);
            cout << "Class date (YYYY-MM-DD): ";
            getline(cin, classDate);
            cout << "Start time (HH:MM:SS): ";
            getline(cin, startTime);
            cout << "End time (HH:MM:SS): ";
            getline(cin, endTime);
            cout << "Classroom: ";
            getline(cin, classroom);
            cout << "Course ID: ";
            getline(cin, courseId);

            cout << (admin.addClassSession(sessionId, classDate, startTime,
                endTime, classroom, courseId) ? "PASS\n" : "FAIL\n");
            break;

        case 19:
            admin.viewClassSessions();
            break;

        case 20:
            cout << "Session ID: ";
            getline(cin, sessionId);
            cout << "New class date (YYYY-MM-DD): ";
            getline(cin, classDate);
            cout << "New start time: ";
            getline(cin, startTime);
            cout << "New end time: ";
            getline(cin, endTime);
            cout << "New classroom: ";
            getline(cin, classroom);
            cout << "Course ID: ";
            getline(cin, courseId);

            cout << (admin.updateClassSession(sessionId, classDate,
                startTime, endTime, classroom, courseId) ? "PASS\n" : "FAIL\n");
            break;

        case 21:
            cout << "Session ID: ";
            getline(cin, sessionId);
            cout << (admin.deleteClassSession(sessionId) ? "PASS\n" : "FAIL\n");
            break;

        case 22:
            admin.viewClassSessionCalendar();
            break;

        case 23:
            admin.viewAttendanceHistory();
            break;

        case 24:
            admin.viewAttendanceReports();
            break;

        case 0:
            break;

        default:
            cout << "Invalid choice!\n";
        }

    } while (choice != 0);
}

int main()
{
    DatabaseConnection database;

    if (!database.connectDatabase()) {
        cout << "Cannot connect to database!\n";
        return 1;
    }

    UserController userController(&database);
    AdministratorController admin(&database);

    int choice;

    do {
        cout << "\n========== STUDENT ATTENDANCE SYSTEM ==========\n";
        cout << "1. Test User Functions\n";
        cout << "2. Test Administrator Functions\n";
        cout << "0. Exit\n";
        cout << "Choose: ";

        cin >> choice;
        cin.ignore(10000, '\n');

        switch (choice) {
        case 1:
            testUser(userController);
            break;

        case 2:
            testAdministrator(admin);
            break;

        case 0:
            cout << "Exiting program...\n";
            break;

        default:
            cout << "Invalid choice!\n";
        }

    } while (choice != 0);

    database.closeDatabase();
    return 0;
}
