
#include <iostream>
#include <string>
#include <limits>

#include "../AdminUserModule/database/DatabaseConnection.h"
#include "../AdminUserModule/controllers/UserController.h"
#include "../AdminUserModule/controllers/AdministratorController.h"

using namespace std;


// INPUT FUNCTIONS
void clearInput()
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getChoice()
{
    int choice;

    while (!(cin >> choice))
    {
        cout << "Invalid input! Please enter a number: ";
        cin.clear();
        clearInput();
    }

    clearInput();
    return choice;
}

void printResult(bool result)
{
    if (result)
        cout << "\nOperation completed successfully!\n";
    else
        cout << "\nOperation failed!\n";
}


// USER FUNCTIONS
void testUser(UserController& userController)
{
    int choice;

    do
    {
        cout << "\n========================================\n";
        cout << "             USER MANAGEMENT            \n";
        cout << "========================================\n";
        cout << "1. Login\n";
        cout << "2. Change Password\n";
        cout << "3. Forgot Password\n";
        cout << "0. Back\n";
        cout << "----------------------------------------\n";
        cout << "Choose: ";

        choice = getChoice();

        string username;
        string password;
        string oldPassword;
        string newPassword;
        string confirmPassword;
        string email;

        switch (choice)
        {
        case 1:
        {
            cout << "\n========== LOGIN ==========\n";

            cout << "Username: ";
            getline(cin, username);

            cout << "Password: ";
            getline(cin, password);

            User* user = userController.login(username, password);

            if (user != nullptr)
            {
                cout << "\nLOGIN: PASS\n";
                cout << "User ID: " << user->getUserId() << '\n';

                delete user;
            }
            else
            {
                cout << "\nLOGIN: FAIL\n";
                cout << "Invalid username or password.\n";
            }

            break;
        }

        case 2:
            cout << "\n========== CHANGE PASSWORD ==========\n";

            cout << "Old password: ";
            getline(cin, oldPassword);

            cout << "New password: ";
            getline(cin, newPassword);

            cout << "Confirm new password: ";
            getline(cin, confirmPassword);

            printResult(userController.changePassword(
                oldPassword,
                newPassword,
                confirmPassword
            ));

            break;

        case 3:
            cout << "\n========== FORGOT PASSWORD ==========\n";

            cout << "Enter email: ";
            getline(cin, email);

            printResult(userController.forgotPassword(email));

            break;

        case 0:
            cout << "Returning to main menu...\n";
            break;

        default:
            cout << "Invalid choice!\n";
        }

    } while (choice != 0);
}


// ADMINISTRATOR FUNCTIONS
void testAdministrator(AdministratorController& admin)
{
    int choice;

    do
    {
        cout << "\n================================================\n";
        cout << "             ADMINISTRATOR MANAGEMENT           \n";
        cout << "================================================\n";

        cout << "\n--- USER ACCOUNT MANAGEMENT ---\n";
        cout << "1.  Create User Account\n";
        cout << "2.  View User Accounts\n";
        cout << "3.  Update Username\n";
        cout << "4.  Delete User Account\n";
        cout << "5.  Lock User Account\n";
        cout << "6.  Unlock User Account\n";

        cout << "\n--- DEPARTMENT MANAGEMENT ---\n";
        cout << "7.  Add Department\n";
        cout << "8.  View Departments\n";
        cout << "9.  Update Department\n";
        cout << "10. Delete Department\n";

        cout << "\n--- COURSE MANAGEMENT ---\n";
        cout << "11. Add Course\n";
        cout << "12. View Courses\n";
        cout << "13. Update Course\n";
        cout << "14. Delete Course\n";

        cout << "\n--- COURSE REGISTRATION MANAGEMENT ---\n";
        cout << "15. Add Course Enrollment\n";
        cout << "16. View Course Enrollments\n";
        cout << "17. Delete Course Enrollment\n";

        cout << "\n--- LECTURER ASSIGNMENT MANAGEMENT ---\n";
        cout << "18. Assign Lecturer To Course\n";
        cout << "19. Unassign Lecturer From Course\n";

        cout << "\n--- CLASS SESSION MANAGEMENT ---\n";
        cout << "20. Add Class Session\n";
        cout << "21. View Class Sessions\n";
        cout << "22. Update Class Session\n";
        cout << "23. Delete Class Session\n";
        cout << "24. View Class Session Calendar\n";

        cout << "\n--- ATTENDANCE MANAGEMENT ---\n";
        cout << "25. View Attendance History\n";
        cout << "26. View Attendance Reports\n";

        cout << "\n0.  Back\n";

        cout << "================================================\n";
        cout << "Choose: ";

        choice = getChoice();

        string userId;
        string username;
        string password;
        string role;

        string fullName;
        string email;
        string phone;
        string address;
        string dob;
        string gender;
        string departmentId;
        string departmentName;

        string courseId;
        string courseName;
        string description;
        string academicYear;
        string lecturerId;

        string registrationId;

        string sessionId;
        string classDate;
        string startTime;
        string endTime;
        string classroom;

        int semester;

        switch (choice)
        {


        case 1:
            cout << "\n========== CREATE USER ACCOUNT ==========\n";

            cout << "User ID: ";
            getline(cin, userId);

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

            if (role == "Student")
            {
                cout << "Address: ";
                getline(cin, address);

                cout << "Date of birth (YYYY-MM-DD): ";
                getline(cin, dob);

                cout << "Gender: ";
                getline(cin, gender);
            }
            else if (role == "Lecturer")
            {
                cout << "Date of birth (YYYY-MM-DD): ";
                getline(cin, dob);

                cout << "Gender: ";
                getline(cin, gender);

                cout << "Department ID: ";
                getline(cin, departmentId);
            }

            printResult(admin.createUserAccount(
                userId,
                username,
                password,
                role,
                fullName,
                email,
                phone,
                address,
                dob,
                gender,
                departmentId
            ));

            break;



        case 2:
            cout << "\n========== USER ACCOUNTS ==========\n";
            admin.viewUserAccounts();
            break;


        case 3:
            cout << "\n========== UPDATE USERNAME ==========\n";

            cout << "User ID: ";
            getline(cin, userId);

            cout << "New username: ";
            getline(cin, username);

            printResult(admin.updateUserAccount(userId, username));

            break;


        case 4:
        {
            cout << "\n========== DELETE USER ACCOUNT ==========\n";

            cout << "User ID: ";
            getline(cin, userId);

            char confirm;

            cout << "Are you sure? (Y/N): ";
            cin >> confirm;
            clearInput();

            if (confirm == 'Y' || confirm == 'y')
            {
                printResult(admin.deleteUserAccount(userId));
            }
            else
            {
                cout << "Operation cancelled.\n";
            }

            break;
        }


        case 5:
            cout << "\n========== LOCK USER ACCOUNT ==========\n";

            cout << "User ID: ";
            getline(cin, userId);

            printResult(admin.lockUserAccount(userId));

            break;

        case 6:
            cout << "\n========== UNLOCK USER ACCOUNT ==========\n";

            cout << "User ID: ";
            getline(cin, userId);

            printResult(admin.unlockUserAccount(userId));

            break;

        case 7:
            cout << "\n========== ADD DEPARTMENT ==========\n";

            cout << "Department ID: ";
            getline(cin, departmentId);

            cout << "Department name: ";
            getline(cin, departmentName);

            printResult(admin.addDepartment(
                departmentId,
                departmentName
            ));

            break;

        case 8:
            cout << "\n========== DEPARTMENT LIST ==========\n";
            admin.viewDepartments();
            break;

        case 9:
            cout << "\n========== UPDATE DEPARTMENT ==========\n";

            cout << "Department ID: ";
            getline(cin, departmentId);

            cout << "New department name: ";
            getline(cin, departmentName);

            printResult(admin.updateDepartment(
                departmentId,
                departmentName
            ));

            break;


        case 10:
            cout << "\n========== DELETE DEPARTMENT ==========\n";

            cout << "Department ID: ";
            getline(cin, departmentId);

            printResult(admin.deleteDepartment(departmentId));

            break;

        case 11:
            cout << "\n========== ADD COURSE ==========\n";

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
            clearInput();

            cout << "Lecturer ID (empty if none): ";
            getline(cin, lecturerId);

            printResult(admin.addCourse(
                courseId,
                courseName,
                description,
                academicYear,
                semester,
                lecturerId
            ));

            break;

        case 12:
            cout << "\n========== COURSE LIST ==========\n";
            admin.viewCourses();
            break;


        case 13:
            cout << "\n========== UPDATE COURSE ==========\n";

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
            clearInput();

            cout << "Lecturer ID (empty if none): ";
            getline(cin, lecturerId);

            printResult(admin.updateCourse(
                courseId,
                courseName,
                description,
                academicYear,
                semester,
                lecturerId
            ));

            break;

        case 14:
            cout << "\n========== DELETE COURSE ==========\n";

            cout << "Course ID: ";
            getline(cin, courseId);

            printResult(admin.deleteCourse(courseId));

            break;


        case 15:
            cout << "\n========== ADD COURSE ENROLLMENT ==========\n";

            cout << "Course ID: ";
            getline(cin, courseId);

            cout << "Student ID: ";
            getline(cin, userId);

            printResult(admin.addCourseEnrollment(
                courseId,
                userId
            ));

            break;

        case 16:
            cout << "\n========== COURSE ENROLLMENTS ==========\n";
            admin.viewCourseEnrollments();
            break;


        case 17:
            cout << "\n========== DELETE COURSE ENROLLMENT ==========\n";

            cout << "Registration ID: ";
            getline(cin, registrationId);

            printResult(admin.deleteCourseEnrollment(registrationId));

            break;

        case 18:
            cout << "\n========== ASSIGN LECTURER TO COURSE ==========\n";

            cout << "Course ID: ";
            getline(cin, courseId);

            cout << "Lecturer ID: ";
            getline(cin, lecturerId);

            printResult(admin.assignLecturerToCourse(
                courseId,
                lecturerId
            ));

            break;


        case 19:
            cout << "\n========== UNASSIGN LECTURER FROM COURSE ==========\n";

            cout << "Course ID: ";
            getline(cin, courseId);

            printResult(admin.unassignLecturerFromCourse(courseId));

            break;


        case 20:
            cout << "\n========== ADD CLASS SESSION ==========\n";

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

            printResult(admin.addClassSession(
                sessionId,
                classDate,
                startTime,
                endTime,
                classroom,
                courseId
            ));

            break;


        case 21:
            cout << "\n========== CLASS SESSIONS ==========\n";
            admin.viewClassSessions();
            break;


        case 22:
            cout << "\n========== UPDATE CLASS SESSION ==========\n";

            cout << "Session ID: ";
            getline(cin, sessionId);

            cout << "New class date (YYYY-MM-DD): ";
            getline(cin, classDate);

            cout << "New start time (HH:MM:SS): ";
            getline(cin, startTime);

            cout << "New end time (HH:MM:SS): ";
            getline(cin, endTime);

            cout << "New classroom: ";
            getline(cin, classroom);

            cout << "Course ID: ";
            getline(cin, courseId);

            printResult(admin.updateClassSession(
                sessionId,
                classDate,
                startTime,
                endTime,
                classroom,
                courseId
            ));

            break;

        case 23:
            cout << "\n========== DELETE CLASS SESSION ==========\n";

            cout << "Session ID: ";
            getline(cin, sessionId);

            printResult(admin.deleteClassSession(sessionId));

            break;


        case 24:
            cout << "\n========== CLASS SESSION CALENDAR ==========\n";
            admin.viewClassSessionCalendar();
            break;


        case 25:
            cout << "\n========== ATTENDANCE HISTORY ==========\n";
            admin.viewAttendanceHistory();
            break;


        case 26:
            cout << "\n========== ATTENDANCE REPORTS ==========\n";
            admin.viewAttendanceReports();
            break;

        case 0:
            cout << "Returning to main menu...\n";
            break;

        default:
            cout << "Invalid choice!\n";
        }

    } while (choice != 0);
}

int main()
{
    // Initialize database connection
    DatabaseConnection database;

    cout << "\nConnecting to PostgreSQL database...\n";

    if (!database.connectDatabase())
    {
        cout << "\nCannot connect to database!\n";
        cout << "Please check PostgreSQL and database configuration.\n";
        return 1;
    }

    cout << "Database connected successfully!\n";

    // Initialize controllers
    UserController userController(&database);
    AdministratorController admin(&database);

    int choice;

    do
    {
        cout << "\n================================================\n";
        cout << "          STUDENT ATTENDANCE SYSTEM             \n";
        cout << "================================================\n";
        cout << "1. Test User Functions\n";
        cout << "2. Test Administrator Functions\n";
        cout << "0. Exit\n";
        cout << "================================================\n";
        cout << "Choose: ";

        choice = getChoice();

        switch (choice)
        {
        case 1:
            testUser(userController);
            break;

        case 2:
            testAdministrator(admin);
            break;

        case 0:
            cout << "\nExiting program...\n";
            break;

        default:
            cout << "Invalid choice!\n";
        }

    } while (choice != 0);

    database.closeDatabase();

    cout << "Database connection closed.\n";

    return 0;
}
