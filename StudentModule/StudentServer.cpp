
#include "httplib.h"
#include "StudentController.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <mutex>
#include <cmath>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <cctype>

using namespace std;

const string FRONTEND_FOLDER =
    "StudentModule/StudentFrontend/";

const string REGISTRATION_FILE =
    "StudentModule/course_registrations.txt";

const string ATTENDANCE_FILE =
    "StudentModule/attendance_demo.txt";

const string DEMO_COURSE_ID = "PHI205";
const string DEMO_SESSION_ID = "DEMO001";

const double DEMO_CLASS_LAT = 10.8000;
const double DEMO_CLASS_LON = 106.7000;
const double DEMO_RADIUS_METERS = 100.0;

mutex fileMutex;
StudentController studentController;

string getCurrentCheckinTime() {
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);

    tm localTime = {};

#ifdef _WIN32
    localtime_s(&localTime, &currentTime);
#else
    localtime_r(&currentTime, &localTime);
#endif

    ostringstream output;
    output << put_time(&localTime, "%Y-%m-%dT%H:%M:%S");
    return output.str();
}
string escapeJson(const string& value) {
    string result;

    for (char character : value) {
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
                result += character;
        }
    }

    return result;
}

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
        "application/json"
    );
}

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

string getParameter(
    const httplib::Request& request,
    const string& name
) {
    if (!request.has_param(name)) {
        return "";
    }

    return request.get_param_value(name);
}

bool parseCoordinate(
    const string& value,
    double& coordinate
) {
    try {
        size_t position = 0;
        coordinate = stod(value, &position);

        return position == value.size() &&
               isfinite(coordinate);
    } catch (...) {
        return false;
    }
}

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

bool alreadyCheckedIn(
    const string& studentId,
    const string& sessionId
) {
    ifstream file(ATTENDANCE_FILE);

    string savedStudentId;
    string savedCourseId;
    string savedSessionId;
    string savedStatus;

    string savedCheckinTime;
            string recordLine;

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
        if (savedStudentId == studentId &&
            savedSessionId == sessionId) {
            return true;
        }
    }

    return false;
}

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

int main() {
    httplib::Server server;

    server.Get(
        "/",
        [](const httplib::Request&,
           httplib::Response& response) {
            response.set_redirect("/index.html");
        }
    );

    server.Get(
        R"(/(index|courses|history|profile|rate)\.html)",
        [](const httplib::Request& request,
           httplib::Response& response) {
            string fileName =
                request.matches[1].str() + ".html";

            serveHtml(fileName, response);
        }
    );

    server.Get(
        R"(/(css|js)/(index|courses|history|profile|rate)\.(css|js))",
        [](const httplib::Request& request,
           httplib::Response& response) {
            const string folder = request.matches[1].str();
            const string page = request.matches[2].str();
            const string extension = request.matches[3].str();

            if (folder != extension) {
                response.status = 404;
                return;
            }

            const string filePath =
                FRONTEND_FOLDER + folder + "/" +
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

    // CS201 is closed.
    // Real session data must come from the group's
    // existing backend when integration is available.
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
                "application/json"
            );
        }
    );

    // Isolated demonstration session.
    server.Get(
        "/api/demo-session",
        [](const httplib::Request&,
           httplib::Response& response) {
            response.set_content(
                R"({"courseId":"PHI205","sessionId":"DEMO001","status":"Demo Open","canCheckIn":true,"message":"Student Module demonstration only."})",
                "application/json"
            );
        }
    );

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
                    escapeJson(savedCourseId) + "\"";

                first = false;
            }

            json += "]}";

            response.set_content(
                json,
                "application/json"
            );
        }
    );

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

            bool withinRadius = isWithinDemoGps(
                latitude,
                longitude
            );

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
                "application/json"
            );
        }
    );

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

            // Do not allow official attendance through
            // the isolated demonstration server.
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

            bool isRegistered =
                registrationExists(studentId, courseId);

            bool isWithinGpsRadius =
                isWithinDemoGps(latitude, longitude);

            bool hasCheckedIn =
                alreadyCheckedIn(studentId, sessionId);

            // The demo is explicitly open for testing.
            // Its time window is not connected to
            // the Lecturer Module yet.
            string sessionStatus = "Open";
            bool isWithinTime = true;

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
                } else if (!studentController.canCheckIn(
                               sessionStatus
                           )) {
                    sendJson(
                        response,
                        403,
                        false,
                        "Attendance session is closed."
                    );
                } else if (!isWithinTime) {
                    sendJson(
                        response,
                        403,
                        false,
                        "Outside attendance time window."
                    );
                } else if (!isWithinGpsRadius) {
                    sendJson(
                        response,
                        403,
                        false,
                        "Outside demo classroom radius."
                    );
                } else if (hasCheckedIn) {
                    sendJson(
                        response,
                        409,
                        false,
                        "You have already checked in."
                    );
                } else {
                    sendJson(
                        response,
                        403,
                        false,
                        "Check-in denied."
                    );
                }

                return;
            }

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
                 << "DEMO_PRESENT" << '\t' << getCurrentCheckinTime() << '\n';

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

            string json = "{\"records\":[";
            bool first = true;

            string savedCheckinTime;
            string recordLine;

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
                "application/json"
            );
        }
    );

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

            response.set_content(
                "{\"studentId\":\"" +
                escapeJson(studentId) +
                "\",\"fullName\":\"Demo Student\",\"dateOfBirth\":\"\",\"gender\":\"\",\"emailAddress\":\"\",\"phoneNumber\":\"\",\"address\":\"\"}",
                "application/json"
            );
        }
    );

    cout << "Student Server: http://localhost:8080"
         << endl;

    if (!server.listen("127.0.0.1", 8080)) {
        cerr << "Cannot start Student Server."
             << endl;
        return 1;
    }

    return 0;
}
