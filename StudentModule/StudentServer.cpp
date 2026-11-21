

#include "httplib.h"
#include "GpsValidator.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <mutex>
#include <vector>
#include <cctype>

using namespace std;

const string REGISTRATION_FILE =
    "StudentModule/course_registrations.txt";

mutex registrationMutex;

bool isValidId(const string& value, size_t maxLength) {
    if (value.empty() || value.size() > maxLength) {
        return false;
    }

    for (char character : value) {
        if (!isalnum(static_cast<unsigned char>(character)) &&
            character != '_' && character != '-') {
            return false;
        }
    }

    return true;
}

string escapeJson(const string& value) {
    string result;

    for (char character : value) {
        if (character == '"' || character == '\\') {
            result += '\\';
        }
        result += character;
    }

    return result;
}

bool registrationExists(
    const string& studentId,
    const string& courseId
) {
    ifstream file(REGISTRATION_FILE);
    string line;

    while (getline(file, line)) {
        stringstream stream(line);
        string savedStudentId;
        string savedCourseId;

        getline(stream, savedStudentId, '\t');
        getline(stream, savedCourseId, '\t');

        if (savedStudentId == studentId &&
            savedCourseId == courseId) {
            return true;
        }
    }

    return false;
}

int main() {
    httplib::Server server;

    server.Get("/", [](const httplib::Request&,
                       httplib::Response& res) {
        res.set_redirect("/index.html");
    });

    server.Get(
        R"(/(index|courses|history|profile|rate)\.html)",
        [](const httplib::Request& req,
           httplib::Response& res) {

            string filePath =
                "StudentModule/StudentFrontend" + req.path;

            ifstream file(filePath, ios::binary);

            if (!file) {
                res.status = 404;
                res.set_content(
                    "Page not found",
                    "text/plain"
                );
                return;
            }

            stringstream buffer;
            buffer << file.rdbuf();

            res.set_content(
                buffer.str(),
                "text/html; charset=UTF-8"
            );
        }
    );

    server.Get("/api/health",
        [](const httplib::Request&,
           httplib::Response& res) {

            res.set_content(
                R"({"status":"ok","module":"Student"})",
                "application/json"
            );
        }
    );

    server.Get("/api/check-gps",
        [](const httplib::Request& req,
           httplib::Response& res) {

            if (!req.has_param("studentLat") ||
                !req.has_param("studentLon") ||
                !req.has_param("classLat") ||
                !req.has_param("classLon") ||
                !req.has_param("radius")) {

                res.status = 400;
                res.set_content(
                    R"({"error":"Missing GPS parameters"})",
                    "application/json"
                );
                return;
            }

            try {
                double studentLat =
                    stod(req.get_param_value("studentLat"));

                double studentLon =
                    stod(req.get_param_value("studentLon"));

                double classLat =
                    stod(req.get_param_value("classLat"));

                double classLon =
                    stod(req.get_param_value("classLon"));

                double radius =
                    stod(req.get_param_value("radius"));

                if (!isfinite(studentLat) ||
                    !isfinite(studentLon) ||
                    !isfinite(classLat) ||
                    !isfinite(classLon) ||
                    !isfinite(radius) ||
                    studentLat < -90 || studentLat > 90 ||
                    classLat < -90 || classLat > 90 ||
                    studentLon < -180 || studentLon > 180 ||
                    classLon < -180 || classLon > 180 ||
                    radius < 0) {

                    res.status = 400;
                    res.set_content(
                        R"({"error":"Invalid GPS parameters"})",
                        "application/json"
                    );
                    return;
                }

                bool valid = GpsValidator::isWithinRadius(
                    studentLat,
                    studentLon,
                    classLat,
                    classLon,
                    radius
                );

                res.set_content(
                    valid
                        ? R"({"withinRadius":true})"
                        : R"({"withinRadius":false})",
                    "application/json"
                );

            } catch (...) {
                res.status = 400;
                res.set_content(
                    R"({"error":"Invalid GPS parameters"})",
                    "application/json"
                );
            }
        }
    );

    server.Post("/api/register-course",
        [](const httplib::Request& req,
           httplib::Response& res) {

            if (!req.has_param("studentId") ||
                !req.has_param("courseId")) {

                res.status = 400;
                res.set_content(
                    R"({"success":false,"message":"Missing information"})",
                    "application/json"
                );
                return;
            }

            string studentId =
                req.get_param_value("studentId");

            string courseId =
                req.get_param_value("courseId");

            if (!isValidId(studentId, 12) ||
                !isValidId(courseId, 20)) {

                res.status = 400;
                res.set_content(
                    R"({"success":false,"message":"Invalid student or course ID"})",
                    "application/json"
                );
                return;
            }

            lock_guard<mutex> lock(registrationMutex);

            if (registrationExists(studentId, courseId)) {
                res.status = 409;
                res.set_content(
                    R"({"success":false,"message":"Course already registered"})",
                    "application/json"
                );
                return;
            }

            ofstream file(REGISTRATION_FILE, ios::app);

            if (!file) {
                res.status = 500;
                res.set_content(
                    R"({"success":false,"message":"Cannot open registration file"})",
                    "application/json"
                );
                return;
            }

            file << studentId << '\t'
                 << courseId << '\n';

            file.flush();

            if (!file) {
                res.status = 500;
                res.set_content(
                    R"({"success":false,"message":"Cannot save registration"})",
                    "application/json"
                );
                return;
            }

            res.set_content(
                R"({"success":true,"message":"Course registered successfully"})",
                "application/json"
            );
        }
    );

    server.Get("/api/my-courses",
        [](const httplib::Request& req,
           httplib::Response& res) {

            if (!req.has_param("studentId")) {
                res.status = 400;
                res.set_content(
                    R"({"error":"Missing student ID"})",
                    "application/json"
                );
                return;
            }

            string studentId =
                req.get_param_value("studentId");

            if (!isValidId(studentId, 12)) {
                res.status = 400;
                res.set_content(
                    R"({"error":"Invalid student ID"})",
                    "application/json"
                );
                return;
            }

            lock_guard<mutex> lock(registrationMutex);

            ifstream file(REGISTRATION_FILE);
            string line;
            string json = "{\"courses\":[";
            bool first = true;

            while (getline(file, line)) {
                stringstream stream(line);
                string savedStudentId;
                string savedCourseId;

                getline(stream, savedStudentId, '\t');
                getline(stream, savedCourseId, '\t');

                if (savedStudentId == studentId) {
                    if (!first) {
                        json += ",";
                    }

                    json += "\"" +
                        escapeJson(savedCourseId) + "\"";

                    first = false;
                }
            }

            json += "]}";

            res.set_content(json, "application/json");
        }
    );

    // Demo attendance API - not real GPS verification
    server.Post("/api/check-in",
        [](const httplib::Request& req,
           httplib::Response& res) {

            if (!req.has_param("studentId") ||
                !req.has_param("courseId")) {
                res.status = 400;
                res.set_content(
                    R"({"success":false,"message":"Missing information"})",
                    "application/json"
                );
                return;
            }

            string studentId = req.get_param_value("studentId");
            string courseId = req.get_param_value("courseId");

            if (!isValidId(studentId, 12) ||
                !isValidId(courseId, 20)) {
                res.status = 400;
                res.set_content(
                    R"({"success":false,"message":"Invalid ID"})",
                    "application/json"
                );
                return;
            }

            // Fixed demonstration session
            if (courseId != "PHI205") {
                res.status = 403;
                res.set_content(
                    R"({"success":false,"message":"No demo session for this course"})",
                    "application/json"
                );
                return;
            }

            lock_guard<mutex> lock(registrationMutex);

            if (!registrationExists(studentId, courseId)) {
                res.status = 403;
                res.set_content(
                    R"({"success":false,"message":"Course not registered"})",
                    "application/json"
                );
                return;
            }

            const string attendanceFile =
                "StudentModule/attendance_demo.txt";

            const string sessionId = "DEMO001";

            ifstream input(attendanceFile);
            string line;

            while (getline(input, line)) {
                stringstream stream(line);
                string savedStudentId;
                string savedCourseId;
                string savedSessionId;

                getline(stream, savedStudentId, '\t');
                getline(stream, savedCourseId, '\t');
                getline(stream, savedSessionId, '\t');

                if (savedStudentId == studentId &&
                    savedCourseId == courseId &&
                    savedSessionId == sessionId) {
                    res.status = 409;
                    res.set_content(
                        R"({"success":false,"message":"Already checked in"})",
                        "application/json"
                    );
                    return;
                }
            }

            input.close();

            ofstream output(attendanceFile, ios::app);

            if (!output) {
                res.status = 500;
                res.set_content(
                    R"({"success":false,"message":"Cannot save attendance"})",
                    "application/json"
                );
                return;
            }

            output << studentId << '\t'
                   << courseId << '\t'
                   << sessionId << '\t'
                   << "DEMO_PRESENT" << '\n';

            output.flush();

            if (!output) {
                res.status = 500;
                res.set_content(
                    R"({"success":false,"message":"Save failed"})",
                    "application/json"
                );
                return;
            }

            res.set_content(
                R"({"success":true,"message":"Demo check-in saved"})",
                "application/json"
            );
        }
    );

    // Read demo attendance history
    server.Get("/api/attendance-history",
        [](const httplib::Request& req,
           httplib::Response& res) {

            if (!req.has_param("studentId")) {
                res.status = 400;
                res.set_content(
                    R"({"error":"Missing student ID"})",
                    "application/json"
                );
                return;
            }

            string studentId = req.get_param_value("studentId");

            if (!isValidId(studentId, 12)) {
                res.status = 400;
                res.set_content(
                    R"({"error":"Invalid student ID"})",
                    "application/json"
                );
                return;
            }

            lock_guard<mutex> lock(registrationMutex);

            ifstream file("StudentModule/attendance_demo.txt");

            string line;
            string json = "{\"records\":[";
            bool first = true;

            while (getline(file, line)) {
                stringstream stream(line);
                string savedStudentId;
                string courseId;
                string sessionId;
                string status;

                getline(stream, savedStudentId, '\t');
                getline(stream, courseId, '\t');
                getline(stream, sessionId, '\t');
                getline(stream, status, '\t');

                if (savedStudentId != studentId) {
                    continue;
                }

                if (!first) {
                    json += ",";
                }

                json += "{\"courseId\":\"" +
                    escapeJson(courseId) +
                    "\",\"sessionId\":\"" +
                    escapeJson(sessionId) +
                    "\",\"status\":\"" +
                    escapeJson(status) + "\"}";

                first = false;
            }

            json += "]}";

            res.set_content(json, "application/json");
        }
    );

    // Get student profile - demo data
    server.Get("/api/student-profile",
        [](const httplib::Request& req,
           httplib::Response& res) {

            if (!req.has_param("studentId") ||
                req.get_param_value("studentId") != "ST001") {
                res.status = 404;
                res.set_content(
                    R"({"error":"Student not found"})",
                    "application/json"
                );
                return;
            }

            res.set_content(
                R"({
                    "studentId":"ST001",
                    "fullName":"Nguyen Minh Anh",
                    "dateOfBirth":"15 Mar 2005",
                    "gender":"Female",
                    "emailAddress":"minhanh.nguyen@university.edu.vn",
                    "phoneNumber":"0900000000",
                    "address":"Ho Chi Minh City, Vietnam"
                })",
                "application/json"
            );
        }
    );

    cout << "Student Server: http://localhost:8080"
         << endl;

    if (!server.listen("127.0.0.1", 8080)) {
        cerr << "Cannot start server on port 8080"
             << endl;
        return 1;
    }

    return 0;
}
