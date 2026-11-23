
#ifndef APISERVER_H
#define APISERVER_H

#include "httplib.h"

#include "../database/DatabaseConnection.h"
#include "../controllers/UserController.h"
#include "../controllers/AdministratorController.h"

#include <string>
#include <unordered_map>
#include <mutex>
#include <chrono>

class ApiServer
{
private:
    // HTTP server
    httplib::Server server;

    // Database
    DatabaseConnection database;

    // Controllers
    UserController userController;
    AdministratorController administratorController;

    // Login session information
    struct SessionInfo
    {
        std::string userId;
        std::string username;
        std::string role;

        std::chrono::system_clock::time_point expiresAt;
    };

    // Store active sessions
    std::unordered_map<std::string, SessionInfo> sessions;

    // Protect session data from concurrent requests
    std::mutex sessionMutex;

    // Generate a secure random token
    std::string generateToken();

    // Validate token and retrieve session
    bool getAuthenticatedSession(
        const httplib::Request& req,
        SessionInfo& session
    );

    // Check Administrator permission
    bool requireAdministrator(
        const httplib::Request& req,
        httplib::Response& res,
        SessionInfo& session
    );

public:
    ApiServer();

    void start();
};

#endif
