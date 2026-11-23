#include "StudentController.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <mutex>
#include <cmath>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <cctype>

using namespace std;

// ==========================================
// 1. CONFIGURATION
// ==========================================

const string REGISTRATION_FILE =
    "StudentModule/course_registrations.txt";

const string ATTENDANCE_FILE =
    "StudentModule/attendance_demo.txt";

// Demonstration course and session
const string DEMO_COURSE_ID = "PHI205";
const string DEMO_SESSION_ID = "DEMO001";

// Demonstration school location
const double DEMO_CLASS_LAT = 10.8000;
const double DEMO_CLASS_LON = 106.7000;
const double DEMO_RADIUS_METERS = 100.0;

// Demonstration attendance session status
const string DEMO_SESSION_STATUS = "Open";

mutex fileMutex;

StudentController studentController;

// ==========================================
// 2. STUDENT INFORMATION
// ==========================================

struct StudentInformation
{
    string studentId;
    string fullName;
    string emailAddress;
    string phoneNumber;
};

StudentInformation currentStudent;

// ==========================================
// 3. INPUT HANDLING
// ==========================================

void clearInput()
{
    cin.clear();
    cin.ignore(
        numeric_limits<streamsize>::max(),
        '\n'
    );
}

int getChoice()
{
    int choice;

    while (true)
    {
        cout << "Enter your choice: ";

        if (cin >> choice)
        {
            clearInput();
            return choice;
        }

        cout << "Invalid input. Please enter a number.\n";
        clearInput();
    }
}

string getNonEmptyInput(const string& message)
{
    string value;

    while (true)
    {
        cout << message;
        getline(cin, value);

        if (!value.empty())
        {
            return value;
        }

        cout << "This field cannot be empty.\n";
    }
}

// ==========================================
// 4. VALIDATION FUNCTIONS
// ==========================================

bool isValidId(const string& value)
{
    if (value.empty() || value.size() > 30)
    {
        return false;
    }

    for (unsigned char character : value)
    {
        if (!isalnum(character) &&
            character != '_' &&
            character != '-')
        {
            return false;
        }
    }

    return true;
}

bool parseCoordinate(
    const string& value,
    double& coordinate
)
{
    try
    {
        size_t position = 0;

        coordinate = stod(value, &position);

        return position == value.size() &&
               isfinite(coordinate);
    }
    catch (...)
    {
        return false;
    }
}

bool isValidLatitude(double latitude)
{
    return latitude >= -90.0 &&
           latitude <= 90.0;
}

bool isValidLongitude(double longitude)
{
    return longitude >= -180.0 &&
           longitude <= 180.0;
}

// ==========================================
// 5. TIME FUNCTION
// ==========================================

string getCurrentCheckinTime()
{
    auto now = chrono::system_clock::now();

    time_t currentTime =
        chrono::system_clock::to_time_t(now);

    tm localTime = {};

#ifdef _WIN32
    localtime_s(&localTime, &currentTime);
#else
    localtime_r(&currentTime, &localTime);
#endif

    ostringstream output;

    output << put_time(
        &localTime,
        "%Y-%m-%dT%H:%M:%S"
    );

    return output.str();
}

// ==========================================
// 6. FILE HANDLING
// ==========================================

bool registrationExists(
    const string& studentId,
    const string& courseId
)
{
    ifstream file(REGISTRATION_FILE);

    if (!file.is_open())
    {
        return false;
    }

    string savedStudentId;
    string savedCourseId;

    while (file >> savedStudentId >> savedCourseId)
    {
        if (savedStudentId == studentId &&
            savedCourseId == courseId)
        {
            return true;
        }
    }

    return false;
}

bool alreadyCheckedIn(
    const string& studentId,
    const string& courseId,
    const string& sessionId
)
{
    ifstream file(ATTENDANCE_FILE);

    if (!file.is_open())
    {
        return false;
    }

    string recordLine;

    while (getline(file, recordLine))
    {
        istringstream recordStream(recordLine);

        string savedStudentId;
        string savedCourseId;
        string savedSessionId;
        string savedStatus;
        string savedCheckinTime;

        if (!(recordStream >> savedStudentId
                          >> savedCourseId
                          >> savedSessionId
                          >> savedStatus
                          >> savedCheckinTime))
        {
            continue;
        }

        if (savedStudentId == studentId &&
            savedCourseId == courseId &&
            savedSessionId == sessionId &&
            savedStatus == "DEMO_PRESENT")
        {
            return true;
        }
    }

    return false;
}

bool saveCourseRegistration(
    const string& studentId,
    const string& courseId
)
{
    ofstream file(
        REGISTRATION_FILE,
        ios::app
    );

    if (!file.is_open())
    {
        return false;
    }

    file << studentId << '\t'
         << courseId << '\n';

    file.flush();

    return file.good();
}

bool saveAttendanceRecord(
    const string& studentId,
    const string& courseId,
    const string& sessionId,
    const string& status
)
{
    ofstream file(
        ATTENDANCE_FILE,
        ios::app
    );

    if (!file.is_open())
    {
        return false;
    }

    file << studentId << '\t'
         << courseId << '\t'
         << sessionId << '\t'
         << status << '\t'
         << getCurrentCheckinTime()
         << '\n';

    file.flush();

    return file.good();
}

// ==========================================
// 7. GPS VALIDATION
// ==========================================

bool isWithinDemoGps(
    double latitude,
    double longitude
)
{
    return studentController.validateGpsLocation(
        latitude,
        longitude,
        DEMO_CLASS_LAT,
        DEMO_CLASS_LON,
        DEMO_RADIUS_METERS
    );
}

bool getStudentCoordinates(
    double& latitude,
    double& longitude
)
{
    string latitudeInput;
    string longitudeInput;

    cout << "\n========== GPS LOCATION ==========\n";

    cout << "Enter your latitude: ";
    getline(cin, latitudeInput);

    cout << "Enter your longitude: ";
    getline(cin, longitudeInput);

    if (!parseCoordinate(latitudeInput, latitude) ||
        !parseCoordinate(longitudeInput, longitude))
    {
        cout << "\nInvalid GPS coordinates.\n";
        return false;
    }

    if (!isValidLatitude(latitude) ||
        !isValidLongitude(longitude))
    {
        cout << "\nGPS coordinates are out of range.\n";
        return false;
    }

    return true;
}

// ==========================================
// 8. VIEW PERSONAL INFORMATION
// ==========================================

void viewPersonalInformation()
{
    cout << "\n========== PERSONAL INFORMATION ==========\n";

    cout << "Student ID: "
         << currentStudent.studentId << '\n';

    cout << "Full Name: "
         << currentStudent.fullName << '\n';

    cout << "Email Address: "
         << currentStudent.emailAddress << '\n';

    cout << "Phone Number: "
         << currentStudent.phoneNumber << '\n';

    cout << "==========================================\n";
}

// ==========================================
// 9. COURSE REGISTRATION
// ==========================================

void registerCourse()
{
    cout << "\n========== COURSE REGISTRATION ==========\n";

    cout << "Student ID: "
         << currentStudent.studentId << '\n';

    cout << "Available demonstration course:\n";
    cout << "Course ID: " << DEMO_COURSE_ID << '\n';

    string courseId = getNonEmptyInput(
        "Enter Course ID: "
    );

    if (!isValidId(courseId))
    {
        cout << "\nInvalid Course ID.\n";
        return;
    }

    lock_guard<mutex> lock(fileMutex);

    if (registrationExists(
            currentStudent.studentId,
            courseId
        ))
    {
        cout << "\nYou have already registered for this course.\n";
        return;
    }

    if (!saveCourseRegistration(
            currentStudent.studentId,
            courseId
        ))
    {
        cout << "\nCannot save course registration.\n";
        return;
    }

    cout << "\nCourse registration successful.\n";
}

// ==========================================
// 10. VIEW REGISTERED COURSES
// ==========================================

void viewRegisteredCourses()
{
    cout << "\n========== REGISTERED COURSES ==========\n";

    lock_guard<mutex> lock(fileMutex);

    ifstream file(REGISTRATION_FILE);

    if (!file.is_open())
    {
        cout << "No registration data available.\n";
        return;
    }

    string savedStudentId;
    string savedCourseId;

    bool found = false;

    cout << left
         << setw(20) << "Student ID"
         << setw(20) << "Course ID"
         << '\n';

    cout << string(40, '-') << '\n';

    while (file >> savedStudentId >> savedCourseId)
    {
        if (savedStudentId == currentStudent.studentId)
        {
            cout << left
                 << setw(20) << savedStudentId
                 << setw(20) << savedCourseId
                 << '\n';

            found = true;
        }
    }

    if (!found)
    {
        cout << "You have not registered for any courses.\n";
    }
}

// ==========================================
// 11. ATTENDANCE CHECK-IN
// ==========================================

void attendanceCheckIn()
{
    cout << "\n========== ATTENDANCE CHECK-IN ==========\n";

    cout << "Student ID: "
         << currentStudent.studentId << '\n';

    cout << "Demonstration Course: "
         << DEMO_COURSE_ID << '\n';

    cout << "Demonstration Session: "
         << DEMO_SESSION_ID << '\n';

    string courseId = getNonEmptyInput(
        "Enter Course ID: "
    );

    string sessionId = getNonEmptyInput(
        "Enter Session ID: "
    );

    if (!isValidId(courseId) ||
        !isValidId(sessionId))
    {
        cout << "\nInvalid course or session ID.\n";
        return;
    }

    // Only the configured demonstration session
    // is available in this standalone version.

    if (courseId != DEMO_COURSE_ID ||
        sessionId != DEMO_SESSION_ID)
    {
        cout << "\nAttendance session is not available.\n";
        return;
    }

    double latitude = 0;
    double longitude = 0;

    if (!getStudentCoordinates(latitude, longitude))
    {
        return;
    }

    lock_guard<mutex> lock(fileMutex);

    // Condition 1: Student must register for the course.
    bool isRegistered = registrationExists(
        currentStudent.studentId,
        courseId
    );

    // Condition 2: Attendance session must be open.
    string sessionStatus = DEMO_SESSION_STATUS;

    // Condition 3: Student must check in during
    // the configured attendance period.
    // The demonstration version does not yet connect
    // to the Lecturer Module's session schedule.
    bool isWithinTime = true;

    // Condition 4: Student must be inside the school radius.
    bool isWithinGpsRadius = isWithinDemoGps(
        latitude,
        longitude
    );

    // Condition 5: Student must not have checked in before.
    bool hasCheckedIn = alreadyCheckedIn(
        currentStudent.studentId,
        courseId,
        sessionId
    );

    bool canCheckIn = studentController.validateCheckIn(
        isRegistered,
        sessionStatus,
        isWithinTime,
        isWithinGpsRadius,
        hasCheckedIn
    );

    if (!canCheckIn)
    {
        if (!isRegistered)
        {
            cout << "\nYou must register for this course first.\n";
        }
        else if (!studentController.canCheckIn(sessionStatus))
        {
            cout << "\nAttendance session is closed.\n";
        }
        else if (!isWithinTime)
        {
            cout << "\nYou are outside the attendance time window.\n";
        }
        else if (!isWithinGpsRadius)
        {
            cout << "\nYou are outside the allowed school area.\n";
            cout << "Allowed radius: "
                 << DEMO_RADIUS_METERS << " meters.\n";
        }
        else if (hasCheckedIn)
        {
            cout << "\nYou have already checked in.\n";
        }
        else
        {
            cout << "\nAttendance check-in denied.\n";
        }

        return;
    }

    if (!saveAttendanceRecord(
            currentStudent.studentId,
            courseId,
            sessionId,
            "DEMO_PRESENT"
        ))
    {
        cout << "\nCannot save attendance record.\n";
        return;
    }

    cout << "\nAttendance check-in successful!\n";

    cout << "Course ID: " << courseId << '\n';
    cout << "Session ID: " << sessionId << '\n';
    cout << "Status: Present\n";
    cout << "Check-in Time: "
         << getCurrentCheckinTime() << '\n';
}

// ==========================================
// 12. ATTENDANCE HISTORY
// ==========================================

void viewAttendanceHistory()
{
    cout << "\n========== ATTENDANCE HISTORY ==========\n";

    lock_guard<mutex> lock(fileMutex);

    ifstream file(ATTENDANCE_FILE);

    if (!file.is_open())
    {
        cout << "No attendance history available.\n";
        return;
    }

    string recordLine;

    bool found = false;

    cout << left
         << setw(18) << "Course ID"
         << setw(18) << "Session ID"
         << setw(18) << "Status"
         << setw(25) << "Check-in Time"
         << '\n';

    cout << string(79, '-') << '\n';

    while (getline(file, recordLine))
    {
        istringstream recordStream(recordLine);

        string savedStudentId;
        string savedCourseId;
        string savedSessionId;
        string savedStatus;
        string savedCheckinTime;

        if (!(recordStream >> savedStudentId
                          >> savedCourseId
                          >> savedSessionId
                          >> savedStatus
                          >> savedCheckinTime))
        {
            continue;
        }

        if (savedStudentId != currentStudent.studentId)
        {
            continue;
        }

        cout << left
             << setw(18) << savedCourseId
             << setw(18) << savedSessionId
             << setw(18) << savedStatus
             << setw(25) << savedCheckinTime
             << '\n';

        found = true;
    }

    if (!found)
    {
        cout << "No attendance records found.\n";
    }
}

// ==========================================
// 13. ATTENDANCE RATE
// ==========================================

void viewAttendanceRate()
{
    cout << "\n========== ATTENDANCE RATE ==========\n";

    string courseId = getNonEmptyInput(
        "Enter Course ID: "
    );

    if (!isValidId(courseId))
    {
        cout << "\nInvalid Course ID.\n";
        return;
    }

    int totalSessions;

    cout << "Enter total number of sessions: ";

    while (!(cin >> totalSessions) ||
           totalSessions <= 0)
    {
        cout << "Please enter a positive number: ";
        clearInput();
    }

    clearInput();

    int attendedSessions = 0;

    {
        lock_guard<mutex> lock(fileMutex);

        ifstream file(ATTENDANCE_FILE);

        if (!file.is_open())
        {
            cout << "\nNo attendance data available.\n";
            return;
        }

        string recordLine;

        while (getline(file, recordLine))
        {
            istringstream recordStream(recordLine);

            string savedStudentId;
            string savedCourseId;
            string savedSessionId;
            string savedStatus;
            string savedCheckinTime;

            if (!(recordStream >> savedStudentId
                              >> savedCourseId
                              >> savedSessionId
                              >> savedStatus
                              >> savedCheckinTime))
            {
                continue;
            }

            if (savedStudentId == currentStudent.studentId &&
                savedCourseId == courseId &&
                savedStatus == "DEMO_PRESENT")
            {
                attendedSessions++;
            }
        }
    }

    if (attendedSessions > totalSessions)
    {
        cout << "\nAttendance data is inconsistent.\n";
        cout << "Recorded attended sessions: "
             << attendedSessions << '\n';
        cout << "Total sessions entered: "
             << totalSessions << '\n';
        return;
    }

    double attendanceRate =
        studentController.calculateAttendanceRate(
            attendedSessions,
            totalSessions
        );

    cout << "\nStudent ID: "
         << currentStudent.studentId << '\n';

    cout << "Course ID: "
         << courseId << '\n';

    cout << "Attended Sessions: "
         << attendedSessions << '\n';

    cout << "Total Sessions: "
         << totalSessions << '\n';

    cout << fixed << setprecision(2);

    cout << "Attendance Rate: "
         << attendanceRate << "%\n";
}

// ==========================================
// 14. STUDENT MENU
// ==========================================

void displayStudentMenu()
{
    cout << "\n";
    cout << "============================================\n";
    cout << "       STUDENT ATTENDANCE MANAGEMENT        \n";
    cout << "============================================\n";

    cout << "Logged in as: "
         << currentStudent.fullName << '\n';

    cout << "Student ID: "
         << currentStudent.studentId << '\n';

    cout << "--------------------------------------------\n";

    cout << "1. View Personal Information\n";
    cout << "2. Course Registration\n";
    cout << "3. View Registered Courses\n";
    cout << "4. Attendance Check-in\n";
    cout << "5. Attendance History\n";
    cout << "6. Attendance Rate\n";
    cout << "0. Exit\n";

    cout << "============================================\n";
}

// ==========================================
// 15. STUDENT LOGIN
// ==========================================

bool studentLogin()
{
    cout << "\n============================================\n";
    cout << "             STUDENT LOGIN                  \n";
    cout << "============================================\n";

    currentStudent.studentId = getNonEmptyInput(
        "Enter Student ID: "
    );

    if (!isValidId(currentStudent.studentId))
    {
        cout << "\nInvalid Student ID.\n";
        return false;
    }

    // Demonstration information.
    // Replace with authenticated account data
    // when the User/Database integration is available.

    currentStudent.fullName = getNonEmptyInput(
        "Enter Full Name: "
    );

    currentStudent.emailAddress = getNonEmptyInput(
        "Enter Email Address: "
    );

    currentStudent.phoneNumber = getNonEmptyInput(
        "Enter Phone Number: "
    );

    cout << "\nLogin successful (demonstration mode).\n";

    return true;
}

// ==========================================
// 16. MAIN
// ==========================================

int main()
{
    cout << "============================================\n";
    cout << "     STUDENT ATTENDANCE MANAGEMENT SYSTEM   \n";
    cout << "============================================\n";

    if (!studentLogin())
    {
        cout << "\nCannot start Student Module.\n";
        return 1;
    }

    int choice;

    do
    {
        displayStudentMenu();

        choice = getChoice();

        switch (choice)
        {
            case 1:
                viewPersonalInformation();
                break;

            case 2:
                registerCourse();
                break;

            case 3:
                viewRegisteredCourses();
                break;

            case 4:
                attendanceCheckIn();
                break;

            case 5:
                viewAttendanceHistory();
                break;

            case 6:
                viewAttendanceRate();
                break;

            case 0:
                cout << "\nExiting Student Module...\n";
                break;

            default:
                cout << "\nInvalid choice. Please try again.\n";
                break;
        }

        if (choice != 0)
        {
            cout << "\nPress Enter to continue...";
            cin.get();
        }

    } while (choice != 0);

    cout << "\nStudent Module closed.\n";

    return 0;
}