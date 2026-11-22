
#include "httplib.h"
#include "StudentController.h"
#include "Student.h"

#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <mutex>
#include <cmath>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <cctype>
#include <algorithm>

using namespace std;

// File paths
const string FRONTEND_FOLDER =
    "StudentModule/StudentFrontend/";

const string REGISTRATION_FILE =
    "StudentModule/course_registrations.txt";

const string ATTENDANCE_FILE =
    "StudentModule/attendance_demo.txt";

const string PROFILE_FILE =
    "StudentModule/student_profiles.txt";

// Demo attendance
const string DEMO_COURSE_ID = "PHI205";
const string DEMO_SESSION_ID = "DEMO001";

const double DEMO_CLASS_LAT = 10.8000;
const double DEMO_CLASS_LON = 106.7000;
const double DEMO_RADIUS_METERS = 100.0;

// Global objects
mutex fileMutex;
StudentController studentController;

// Get current time
string getCurrentCheckinTime() {
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

// Escape JSON special characters
string escapeJson(const string& value) {
    string result;

    for (unsigned char character : value) {
        switch (character) {
            case '"':
                result += "\\\"";
                break;

            case '\\':
                result += "\\\\";
                break;

            case '\n':
                result += "\\n";
                break;

            case '\r':
                result += "\\r";
                break;

            case '\t':
                result += "\\t";
                break;

            default:
                if (character < 0x20) {
                    ostringstream escaped;
                    escaped << "\\u"
                            << hex
                            << setw(4)
                            << setfill('0')
                            << static_cast<int>(character);
                    result += escaped.str();
                } else {
                    result += static_cast<char>(character);
                }
                break;
        }
    }

    return result;
}

// Return a JSON message
void sendJson(
    httplib::Response& response,
    int statusCode,
    bool success,
    const string& message
) {
    response.status = statusCode;

    response.set_content(
        "{\"success\":" +
        string(success ? "true" : "false") +
        ",\"message\":\"" +
        escapeJson(message) +
        "\"}",
        "application/json; charset=UTF-8"
    );
}

// Validate student ID or course ID
bool isValidId(const string& value) {
    if (value.empty() || value.size() > 30) {
        return false;
    }

    for (unsigned char character : value) {
        if (!isalnum(character) &&
            character != '_' &&
            character != '-') {
            return false;
        }
    }

    return true;
}

// Read a form parameter
string getParameter(
    const httplib::Request& request,
    const string& name
) {
    if (!request.has_param(name)) {
        return "";
    }

    return request.get_param_value(name);
}

// Convert coordinate string to double
bool parseCoordinate(
    const string& value,
    double& coordinate
) {
    try {
        size_t position = 0;

        coordinate = stod(value, &position);

        return position == value.size() &&
               isfinite(coordinate);
    }
    catch (...) {
        return false;
    }
}

// Read GPS coordinates
bool readCoordinates(
    const httplib::Request& request,
    double& latitude,
    double& longitude
) {
    if (!parseCoordinate(
            getParameter(request, "studentLat"),
            latitude
        )) {
        return false;
    }

    if (!parseCoordinate(
            getParameter(request, "studentLon"),
            longitude
        )) {
        return false;
    }

    return latitude >= -90.0 &&
           latitude <= 90.0 &&
           longitude >= -180.0 &&
           longitude <= 180.0;
}

// Check whether a course is registered
bool registrationExists(
    const string& studentId,
    const string& courseId
) {
    ifstream file(REGISTRATION_FILE);

    string savedStudentId;
    string savedCourseId;

    while (file >> savedStudentId >> savedCourseId) {
        if (savedStudentId == studentId &&
            savedCourseId == courseId) {
            return true;
        }
    }

    return false;
}

// Check duplicate attendance
bool alreadyCheckedIn(
    const string& studentId,
    const string& sessionId
) {
    ifstream file(ATTENDANCE_FILE);

    string recordLine;

    while (getline(file, recordLine)) {
        istringstream recordStream(recordLine);

        string savedStudentId;
        string savedCourseId;
        string savedSessionId;
        string savedStatus;

        if (!(recordStream >> savedStudentId
                          >> savedCourseId
                          >> savedSessionId
                          >> savedStatus)) {
            continue;
        }

        if (savedStudentId == studentId &&
            savedSessionId == sessionId) {
            return true;
        }
    }

    return false;
}

// Validate demo GPS radius
bool isWithinDemoGps(
    double latitude,
    double longitude
) {
    return studentController.validateGpsLocation(
        latitude,
        longitude,
        DEMO_CLASS_LAT,
        DEMO_CLASS_LON,
        DEMO_RADIUS_METERS
    );
}

// Serve HTML pages
void serveHtml(
    const string& fileName,
    httplib::Response& response
) {
    ifstream file(
        FRONTEND_FOLDER + fileName,
        ios::binary
    );

    if (!file.is_open()) {
        response.status = 404;

        response.set_content(
            "HTML file not found.",
            "text/plain"
        );

        return;
    }

    stringstream buffer;
    buffer << file.rdbuf();

    response.set_content(
        buffer.str(),
        "text/html; charset=UTF-8"
    );
}

/*
    PART 2 - STUDENT PROFILE AND SERVER SETUP
*/

// Student profile data
struct StudentProfile {
    string studentId;
    string fullName;
    string dateOfBirth;
    string gender;
    string emailAddress;
    string phoneNumber;
    string address;
};

// Create default demo profile
StudentProfile createDefaultProfile(
    const string& studentId
) {
    StudentProfile student;

    student.studentId = studentId;
    student.fullName =
        studentId == "ST001" ? "Nguyễn Minh Anh" : "";

    student.dateOfBirth = "";
    student.gender = "";
    student.emailAddress = "";
    student.phoneNumber = "";
    student.address = "";

    return student;
}

// Read a single student profile
bool readStudentProfile(
    const string& studentId,
    StudentProfile& student
) {
    ifstream file(PROFILE_FILE);

    if (!file.is_open()) {
        return false;
    }

    StudentProfile savedStudent;

    while (
        file >> quoted(savedStudent.studentId)
             >> quoted(savedStudent.fullName)
             >> quoted(savedStudent.dateOfBirth)
             >> quoted(savedStudent.gender)
             >> quoted(savedStudent.emailAddress)
             >> quoted(savedStudent.phoneNumber)
             >> quoted(savedStudent.address)
    ) {
        if (savedStudent.studentId == studentId) {
            student = savedStudent;
            return true;
        }
    }

    return false;
}

// Convert profile to JSON
string studentProfileToJson(
    const StudentProfile& student
) {
    string json = "{";

    json += "\"studentId\":\"" +
        escapeJson(student.studentId) + "\",";

    json += "\"fullName\":\"" +
        escapeJson(student.fullName) + "\",";

    json += "\"dateOfBirth\":\"" +
        escapeJson(student.dateOfBirth) + "\",";

    json += "\"gender\":\"" +
        escapeJson(student.gender) + "\",";

    json += "\"emailAddress\":\"" +
        escapeJson(student.emailAddress) + "\",";

    json += "\"phoneNumber\":\"" +
        escapeJson(student.phoneNumber) + "\",";

    json += "\"address\":\"" +
        escapeJson(student.address) + "\"";

    json += "}";

    return json;
}

// Validate student profile
bool isValidStudentProfile(
    const StudentProfile& student
) {
    if (!isValidId(student.studentId)) {
        return false;
    }

    if (student.fullName.empty() ||
        student.fullName.size() > 100) {
        return false;
    }

    if (student.dateOfBirth.size() > 10 ||
        student.gender.size() > 20 ||
        student.emailAddress.size() > 150 ||
        student.phoneNumber.size() > 20 ||
        student.address.size() > 250) {
        return false;
    }

    if (!student.gender.empty() &&
        student.gender != "Male" &&
        student.gender != "Female" &&
        student.gender != "Other") {
        return false;
    }

    if (!student.dateOfBirth.empty()) {
        if (student.dateOfBirth.size() != 10 ||
            student.dateOfBirth[4] != '-' ||
            student.dateOfBirth[7] != '-') {
            return false;
        }

        for (size_t i = 0;
             i < student.dateOfBirth.size();
             i++) {
            if (i == 4 || i == 7) {
                continue;
            }

            if (!isdigit(
                    static_cast<unsigned char>(
                        student.dateOfBirth[i]
                    )
                )) {
                return false;
            }
        }
    }

    return true;
}

// Save or update student profile
bool saveStudentProfile(
    const StudentProfile& student
) {
    vector<StudentProfile> profiles;

    {
        ifstream input(PROFILE_FILE);

        StudentProfile savedStudent;

        while (
            input >> quoted(savedStudent.studentId)
                  >> quoted(savedStudent.fullName)
                  >> quoted(savedStudent.dateOfBirth)
                  >> quoted(savedStudent.gender)
                  >> quoted(savedStudent.emailAddress)
                  >> quoted(savedStudent.phoneNumber)
                  >> quoted(savedStudent.address)
        ) {
            if (savedStudent.studentId != student.studentId) {
                profiles.push_back(savedStudent);
            }
        }
    }

    profiles.push_back(student);

    const string tempFile = PROFILE_FILE + ".tmp";
    const string backupFile = PROFILE_FILE + ".bak";

    ofstream output(tempFile, ios::trunc);

    if (!output.is_open()) {
        return false;
    }

    for (const StudentProfile& current : profiles) {
        output
            << quoted(current.studentId) << ' '
            << quoted(current.fullName) << ' '
            << quoted(current.dateOfBirth) << ' '
            << quoted(current.gender) << ' '
            << quoted(current.emailAddress) << ' '
            << quoted(current.phoneNumber) << ' '
            << quoted(current.address) << '\n';
    }

    output.flush();

    bool writeSucceeded = output.good();
    output.close();

    if (!writeSucceeded) {
        std::remove(tempFile.c_str());
        return false;
    }

    bool oldFileExists = false;

    {
        ifstream existing(PROFILE_FILE);
        oldFileExists = existing.good();
    }

    if (oldFileExists) {
        std::remove(backupFile.c_str());

        if (std::rename(
                PROFILE_FILE.c_str(),
                backupFile.c_str()
            ) != 0) {
            std::remove(tempFile.c_str());
            return false;
        }
    }

    if (std::rename(
            tempFile.c_str(),
            PROFILE_FILE.c_str()
        ) != 0) {
        if (oldFileExists) {
            std::rename(
                backupFile.c_str(),
                PROFILE_FILE.c_str()
            );
        }

        std::remove(tempFile.c_str());
        return false;
    }

    if (oldFileExists) {
        std::remove(backupFile.c_str());
    }

    return true;
}

/*
    START STUDENT SERVER
*/

int main() {
    httplib::Server server;

    // Redirect to check-in page
    server.Get(
        "/",
        [](const httplib::Request&,
           httplib::Response& response) {
            response.set_redirect("/index.html");
        }
    );

    // Serve HTML pages
    server.Get(
        R"(/(index|courses|history|profile|rate)\.html)",
        [](const httplib::Request& request,
           httplib::Response& response) {

            string fileName =
                request.matches[1].str() + ".html";

            serveHtml(fileName, response);
        }
    );

    // Serve CSS and JavaScript
    server.Get(
        R"(/(css|js)/(index|courses|history|profile|rate|figma-shared)\.(css|js))",
        [](const httplib::Request& request,
           httplib::Response& response) {

            string folder = request.matches[1].str();
            string page = request.matches[2].str();
            string extension = request.matches[3].str();

            if (folder != extension) {
                response.status = 404;
                return;
            }

            string filePath =
                FRONTEND_FOLDER +
                folder + "/" +
                page + "." + extension;

            ifstream file(filePath, ios::binary);

            if (!file.is_open()) {
                response.status = 404;
                return;
            }

            ostringstream buffer;
            buffer << file.rdbuf();

            response.set_content(
                buffer.str(),
                extension == "css"
                    ? "text/css; charset=UTF-8"
                    : "application/javascript; charset=UTF-8"
            );
        }
    );

    // Health check API
    server.Get(
        "/api/health",
        [](const httplib::Request&,
           httplib::Response& response) {

            response.set_content(
                R"({"status":"ok","module":"Student"})",
                "application/json"
            );
        }
    );

    // Get student profile
    server.Get(
        "/api/student-profile",
        [](const httplib::Request& request,
           httplib::Response& response) {

            string studentId =
                getParameter(request, "studentId");

            if (!isValidId(studentId)) {
                sendJson(
                    response,
                    400,
                    false,
                    "Invalid student ID."
                );
                return;
            }

            lock_guard<mutex> lock(fileMutex);

            StudentProfile student =
                createDefaultProfile(studentId);

            StudentProfile savedStudent;

            if (readStudentProfile(
                    studentId,
                    savedStudent
                )) {
                student = savedStudent;
            }

            response.set_content(
                studentProfileToJson(student),
                "application/json; charset=UTF-8"
            );
        }
    );

    // Update student profile
    server.Post(
        "/api/update-profile",
        [](const httplib::Request& request,
           httplib::Response& response) {

            StudentProfile student;

            student.studentId =
                getParameter(request, "studentId");

            student.fullName =
                getParameter(request, "fullName");

            student.dateOfBirth =
                getParameter(request, "dateOfBirth");

            student.gender =
                getParameter(request, "gender");

            student.emailAddress =
                getParameter(request, "emailAddress");

            student.phoneNumber =
                getParameter(request, "phoneNumber");

            student.address =
                getParameter(request, "address");

            if (!isValidStudentProfile(student)) {
                sendJson(
                    response,
                    400,
                    false,
                    "Invalid student profile information."
                );
                return;
            }

            lock_guard<mutex> lock(fileMutex);

            if (!saveStudentProfile(student)) {
                sendJson(
                    response,
                    500,
                    false,
                    "Cannot save student profile."
                );
                return;
            }

            sendJson(
                response,
                200,
                true,
                "Profile updated successfully."
            );
        }
    );


    /*
        PART 3 - COURSE REGISTRATION
    */

    // Register a course
    server.Post(
        "/api/register-course",
        [](const httplib::Request& request,
           httplib::Response& response) {

            string studentId =
                getParameter(request, "studentId");

            string courseId =
                getParameter(request, "courseId");

            if (!isValidId(studentId) ||
                !isValidId(courseId)) {
                sendJson(
                    response,
                    400,
                    false,
                    "Invalid student or course ID."
                );
                return;
            }

            lock_guard<mutex> lock(fileMutex);

            // Check duplicate registration
            if (registrationExists(studentId, courseId)) {
                sendJson(
                    response,
                    409,
                    false,
                    "Course already registered."
                );
                return;
            }

            ofstream file(REGISTRATION_FILE, ios::app);

            if (!file.is_open()) {
                sendJson(
                    response,
                    500,
                    false,
                    "Cannot open registration file."
                );
                return;
            }

            file << studentId << '\t'
                 << courseId << '\n';

            file.flush();

            if (!file.good()) {
                sendJson(
                    response,
                    500,
                    false,
                    "Cannot save registration."
                );
                return;
            }

            sendJson(
                response,
                200,
                true,
                "Course registered successfully."
            );
        }
    );

    // Get registered courses
    server.Get(
        "/api/my-courses",
        [](const httplib::Request& request,
           httplib::Response& response) {

            string studentId =
                getParameter(request, "studentId");

            if (!isValidId(studentId)) {
                sendJson(
                    response,
                    400,
                    false,
                    "Invalid student ID."
                );
                return;
            }

            lock_guard<mutex> lock(fileMutex);

            ifstream file(REGISTRATION_FILE);

            if (!file.is_open()) {
                sendJson(
                    response,
                    500,
                    false,
                    "Cannot open registration file."
                );
                return;
            }

            string savedStudentId;
            string savedCourseId;

            string json = "{\"courses\":[";

            bool first = true;

            while (file >> savedStudentId >> savedCourseId) {

                if (savedStudentId != studentId) {
                    continue;
                }

                if (!first) {
                    json += ",";
                }

                json += "\"" +
                    escapeJson(savedCourseId) +
                    "\"";

                first = false;
            }

            json += "]}";

            response.set_content(
                json,
                "application/json; charset=UTF-8"
            );
        }
    );

    // Unregister a course
    server.Post(
        "/api/unregister-course",
        [](const httplib::Request& request,
           httplib::Response& response) {

            string studentId =
                getParameter(request, "studentId");

            string courseId =
                getParameter(request, "courseId");

            if (!isValidId(studentId) ||
                !isValidId(courseId)) {
                sendJson(
                    response,
                    400,
                    false,
                    "Invalid student or course ID."
                );
                return;
            }

            lock_guard<mutex> lock(fileMutex);

            ifstream inputFile(REGISTRATION_FILE);

            if (!inputFile.is_open()) {
                sendJson(
                    response,
                    500,
                    false,
                    "Cannot open registration file."
                );
                return;
            }

            const string tempFileName =
                REGISTRATION_FILE + ".tmp";

            const string backupFileName =
                REGISTRATION_FILE + ".bak";

            ofstream outputFile(
                tempFileName,
                ios::trunc
            );

            if (!outputFile.is_open()) {
                inputFile.close();

                sendJson(
                    response,
                    500,
                    false,
                    "Cannot create temporary registration file."
                );
                return;
            }

            string savedStudentId;
            string savedCourseId;

            bool removed = false;

            // Copy registrations except the selected one
            while (inputFile >> savedStudentId >> savedCourseId) {

                if (!removed &&
                    savedStudentId == studentId &&
                    savedCourseId == courseId) {

                    removed = true;
                    continue;
                }

                outputFile << savedStudentId << '\t'
                           << savedCourseId << '\n';
            }

            inputFile.close();

            outputFile.flush();

            bool writeSucceeded = outputFile.good();

            outputFile.close();

            if (!writeSucceeded) {
                std::remove(tempFileName.c_str());

                sendJson(
                    response,
                    500,
                    false,
                    "Cannot save registration changes."
                );
                return;
            }

            if (!removed) {
                std::remove(tempFileName.c_str());

                sendJson(
                    response,
                    404,
                    false,
                    "Course registration not found."
                );
                return;
            }

            // Back up the original registration file
            std::remove(backupFileName.c_str());

            if (std::rename(
                    REGISTRATION_FILE.c_str(),
                    backupFileName.c_str()
                ) != 0) {

                std::remove(tempFileName.c_str());

                sendJson(
                    response,
                    500,
                    false,
                    "Cannot create registration backup."
                );
                return;
            }

            // Replace original with updated file
            if (std::rename(
                    tempFileName.c_str(),
                    REGISTRATION_FILE.c_str()
                ) != 0) {

                std::rename(
                    backupFileName.c_str(),
                    REGISTRATION_FILE.c_str()
                );

                std::remove(tempFileName.c_str());

                sendJson(
                    response,
                    500,
                    false,
                    "Cannot replace registration file."
                );
                return;
            }

            // Remove backup after successful update
            std::remove(backupFileName.c_str());

            sendJson(
                response,
                200,
                true,
                "Course unregistered successfully."
            );
        }
    );


    /*
        PART 4 - STUDENT ATTENDANCE
    */

    // Get current attendance session
    server.Get(
        "/api/checkin-session",
        [](const httplib::Request&,
           httplib::Response& response) {

            bool canCheckIn =
                studentController.canCheckIn("Completed");

            string json =
                R"({"courseId":"CS201",)"
                R"("sessionId":"S13",)"
                R"("status":"Completed",)"
                "\"canCheckIn\":" +
                string(canCheckIn ? "true" : "false") +
                R"(,"message":"This attendance session is closed."})";

            response.set_content(
                json,
                "application/json; charset=UTF-8"
            );
        }
    );

    // Get demonstration attendance session
    server.Get(
        "/api/demo-session",
        [](const httplib::Request&,
           httplib::Response& response) {

            response.set_content(
                R"({"courseId":"PHI205","sessionId":"DEMO001","status":"Demo Open","canCheckIn":true,"message":"Student Module demonstration only."})",
                "application/json; charset=UTF-8"
            );
        }
    );

    // Check GPS coordinates
    server.Get(
        "/api/check-gps",
        [](const httplib::Request& request,
           httplib::Response& response) {

            double latitude = 0;
            double longitude = 0;

            if (!readCoordinates(
                    request,
                    latitude,
                    longitude
                )) {

                sendJson(
                    response,
                    400,
                    false,
                    "Invalid GPS coordinates."
                );
                return;
            }

            bool withinRadius =
                isWithinDemoGps(latitude, longitude);

            string json =
                "{\"withinRadius\":" +
                string(withinRadius ? "true" : "false") +
                ",\"message\":\"" +
                string(
                    withinRadius
                        ? "Inside demo classroom radius."
                        : "Outside demo classroom radius."
                ) +
                "\"}";

            response.set_content(
                json,
                "application/json; charset=UTF-8"
            );
        }
    );

    // Student attendance check-in
    server.Post(
        "/api/check-in",
        [](const httplib::Request& request,
           httplib::Response& response) {

            string studentId =
                getParameter(request, "studentId");

            string courseId =
                getParameter(request, "courseId");

            string sessionId =
                getParameter(request, "sessionId");

            // Validate attendance information
            if (!isValidId(studentId) ||
                !isValidId(courseId) ||
                !isValidId(sessionId)) {

                sendJson(
                    response,
                    400,
                    false,
                    "Invalid attendance information."
                );
                return;
            }

            // Only allow the isolated demo session
            if (courseId != DEMO_COURSE_ID ||
                sessionId != DEMO_SESSION_ID) {

                sendJson(
                    response,
                    403,
                    false,
                    "Attendance session is not available."
                );
                return;
            }

            double latitude = 0;
            double longitude = 0;

            // Read student GPS coordinates
            if (!readCoordinates(
                    request,
                    latitude,
                    longitude
                )) {

                sendJson(
                    response,
                    400,
                    false,
                    "Valid GPS coordinates are required."
                );
                return;
            }

            lock_guard<mutex> lock(fileMutex);

            // Check course registration
            bool isRegistered =
                registrationExists(studentId, courseId);

            // Check GPS radius
            bool isWithinGpsRadius =
                isWithinDemoGps(latitude, longitude);

            // Check duplicate attendance
            bool hasCheckedIn =
                alreadyCheckedIn(studentId, sessionId);

            string sessionStatus = "Open";
            bool isWithinTime = true;

            // Validate all attendance conditions
            bool canCheckIn =
                studentController.validateCheckIn(
                    isRegistered,
                    sessionStatus,
                    isWithinTime,
                    isWithinGpsRadius,
                    hasCheckedIn
                );

            if (!canCheckIn) {

                if (!isRegistered) {
                    sendJson(
                        response,
                        403,
                        false,
                        "Please register the course first."
                    );
                }
                else if (!studentController.canCheckIn(
                             sessionStatus
                         )) {

                    sendJson(
                        response,
                        403,
                        false,
                        "Attendance session is closed."
                    );
                }
                else if (!isWithinTime) {

                    sendJson(
                        response,
                        403,
                        false,
                        "Outside attendance time window."
                    );
                }
                else if (!isWithinGpsRadius) {

                    sendJson(
                        response,
                        403,
                        false,
                        "Outside demo classroom radius."
                    );
                }
                else if (hasCheckedIn) {

                    sendJson(
                        response,
                        409,
                        false,
                        "You have already checked in."
                    );
                }
                else {

                    sendJson(
                        response,
                        403,
                        false,
                        "Check-in denied."
                    );
                }

                return;
            }

            // Save demo attendance record
            ofstream file(ATTENDANCE_FILE, ios::app);

            if (!file.is_open()) {
                sendJson(
                    response,
                    500,
                    false,
                    "Cannot open attendance file."
                );
                return;
            }

            file << studentId << '\t'
                 << courseId << '\t'
                 << sessionId << '\t'
                 << "DEMO_PRESENT" << '\t'
                 << getCurrentCheckinTime() << '\n';

            file.flush();

            if (!file.good()) {
                sendJson(
                    response,
                    500,
                    false,
                    "Cannot save attendance."
                );
                return;
            }

            sendJson(
                response,
                200,
                true,
                "Demo check-in successful."
            );
        }
    );

    /*
        PART 5 - ATTENDANCE HISTORY
    */

    // Get student attendance history
    server.Get(
        "/api/attendance-history",
        [](const httplib::Request& request,
           httplib::Response& response) {

            string studentId =
                getParameter(request, "studentId");

            if (!isValidId(studentId)) {
                sendJson(
                    response,
                    400,
                    false,
                    "Invalid student ID."
                );
                return;
            }

            lock_guard<mutex> lock(fileMutex);

            ifstream file(ATTENDANCE_FILE);

            string savedStudentId;
            string savedCourseId;
            string savedSessionId;
            string savedStatus;
            string savedCheckinTime;
            string recordLine;

            string json = "{\"records\":[";

            bool first = true;

            while (getline(file, recordLine)) {

                istringstream recordStream(recordLine);

                if (!(recordStream >> savedStudentId
                                  >> savedCourseId
                                  >> savedSessionId
                                  >> savedStatus)) {
                    continue;
                }

                savedCheckinTime = "";
                recordStream >> savedCheckinTime;

                // Only return records for this student
                if (savedStudentId != studentId) {
                    continue;
                }

                if (!first) {
                    json += ",";
                }

                json +=
                    "{\"courseId\":\"" +
                    escapeJson(savedCourseId) +
                    "\",\"sessionId\":\"" +
                    escapeJson(savedSessionId) +
                    "\",\"status\":\"" +
                    escapeJson(savedStatus) +
                    "\",\"checkinTime\":\"" +
                    escapeJson(savedCheckinTime) +
                    "\"}";

                first = false;
            }

            json += "]}";

            response.set_content(
                json,
                "application/json; charset=UTF-8"
            );
        }
    );

    /*
        START SERVER
    */

    cout << "Student Server: http://localhost:8080"
         << endl;

    if (!server.listen("127.0.0.1", 8080)) {
        cerr << "Cannot start Student Server."
             << endl;

        return 1;
    }

    return 0;
}
