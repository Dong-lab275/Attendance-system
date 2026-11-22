#include "httplib.h"
#include "GpsValidator.h"
#include "StudentController.h"
#include "StudentView.h"
#include "Course.h"
#include "Student.h"

#include <cstdlib>
#include <iostream>
#include <mutex>
#include <string>

namespace {

std::string getEnvOrDefault(const char* key, const std::string& fallback) {
    const char* val = std::getenv(key);
    return (val != nullptr && *val != '\0') ? std::string(val) : fallback;
}

std::string escapeJson(const std::string& value) {
    std::string out;
    out.reserve(value.size());
    for (char ch : value) {
        if (ch == '"' || ch == '\\') {
            out.push_back('\\');
        }
        out.push_back(ch);
    }
    return out;
}

} // namespace

int main() {
    const std::string host = getEnvOrDefault("SERVER_HOST", "0.0.0.0");
    const int port = std::stoi(getEnvOrDefault("SERVER_PORT", "8080"));
    const std::string dbConnStr = getEnvOrDefault(
        "DATABASE_URL",
        "host=localhost port=5432 dbname=attendance_db user=postgres password=postgres"
    );

    httplib::Server server;
    std::mutex stateMutex;

    StudentController studentController;
    studentController.setStudentInformation(
        "ST001",
        "Nguyen Minh Anh",
        "minhanh.nguyen@university.edu.vn",
        "0900000000"
    );

    server.Get("/api/health", [&](const httplib::Request&, httplib::Response& res) {
        res.set_content(
            R"({"status":"ok","service":"attendance-system","roles":["Lecturer","Admin","Student"]})",
            "application/json"
        );
    });

    // --- Student endpoints ---
    server.Get("/api/check-gps", [](const httplib::Request& req, httplib::Response& res) {
        if (!req.has_param("studentLat") || !req.has_param("studentLon") ||
            !req.has_param("classLat") || !req.has_param("classLon") ||
            !req.has_param("radius")) {
            res.status = 400;
            res.set_content(R"({"error":"Missing GPS parameters"})", "application/json");
            return;
        }

        try {
            double studentLat = std::stod(req.get_param_value("studentLat"));
            double studentLon = std::stod(req.get_param_value("studentLon"));
            double classLat = std::stod(req.get_param_value("classLat"));
            double classLon = std::stod(req.get_param_value("classLon"));
            double radius = std::stod(req.get_param_value("radius"));

            bool withinRadius = GpsValidator::isWithinRadius(
                studentLat, studentLon, classLat, classLon, radius
            );
            res.set_content(
                withinRadius ? R"({"withinRadius":true})" : R"({"withinRadius":false})",
                "application/json"
            );
        } catch (...) {
            res.status = 400;
            res.set_content(R"({"error":"Invalid GPS parameters"})", "application/json");
        }
    });

    server.Get("/api/student/rate", [&](const httplib::Request& req, httplib::Response& res) {
        int attended = req.has_param("attended") ? std::stoi(req.get_param_value("attended")) : 8;
        int total = req.has_param("total") ? std::stoi(req.get_param_value("total")) : 10;

        std::lock_guard<std::mutex> lock(stateMutex);
        double rate = studentController.calculateAttendanceRate(attended, total);
        res.set_content(
            "{\"attended\":" + std::to_string(attended) +
            ",\"total\":" + std::to_string(total) +
            ",\"attendanceRate\":" + std::to_string(rate) + "}",
            "application/json"
        );
    });

    // --- Lecturer endpoints ---
    server.Get("/api/lecturer/session-status", [](const httplib::Request& req, httplib::Response& res) {
        std::string status = req.has_param("status") ? req.get_param_value("status") : "Open";
        bool openForCheckIn = (status == "Open");
        res.set_content(
            "{\"sessionStatus\":\"" + escapeJson(status) +
            "\",\"openForCheckIn\":" + std::string(openForCheckIn ? "true" : "false") + "}",
            "application/json"
        );
    });

    // --- Admin endpoints ---
    server.Get("/api/admin/status", [&](const httplib::Request&, httplib::Response& res) {
        res.set_content(
            "{\"adminModule\":\"ready\",\"lecturerModule\":\"ready\",\"studentModule\":\"ready\",\"dbConfigured\":" +
            std::string(dbConnStr.empty() ? "false" : "true") + "}",
            "application/json"
        );
    });

    std::cout << "Attendance System (Lecturer / Admin / Student) running on http://"
              << host << ":" << port << std::endl;

    if (!server.listen(host.c_str(), port)) {
        std::cerr << "Failed to bind server on " << host << ":" << port << std::endl;
        return 1;
    }

    return 0;
}