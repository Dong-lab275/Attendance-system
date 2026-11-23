
#include "ApiServer.h"

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <chrono>

#include <openssl/rand.h>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

// ============================================
// CONSTRUCTOR
// ============================================

ApiServer::ApiServer()
    : userController(&database),
      administratorController(&database)
{
}

// ============================================
// CORS
// ============================================

static void setCorsHeaders(httplib::Response& res)
{
    res.set_header("Access-Control-Allow-Origin", "*");

    res.set_header(
        "Access-Control-Allow-Methods",
        "GET, POST, PUT, DELETE, OPTIONS"
    );

    res.set_header(
        "Access-Control-Allow-Headers",
        "Content-Type, Authorization"
    );
}

// ============================================
// GENERATE RANDOM TOKEN
// ============================================

string ApiServer::generateToken()
{
    unsigned char randomBytes[32];

    if (RAND_bytes(randomBytes, sizeof(randomBytes)) != 1)
    {
        return "";
    }

    stringstream ss;

    for (int i = 0; i < 32; i++)
    {
        ss << hex
           << setw(2)
           << setfill('0')
           << static_cast<int>(randomBytes[i]);
    }

    return ss.str();
}

// ============================================
// AUTHENTICATION
// ============================================

bool ApiServer::getAuthenticatedSession(
    const httplib::Request& req,
    SessionInfo& session
)
{
    if (!req.has_header("Authorization"))
    {
        return false;
    }

    string authorization = req.get_header_value("Authorization");

    const string prefix = "Bearer ";

    if (authorization.rfind(prefix, 0) != 0)
    {
        return false;
    }

    string token = authorization.substr(prefix.length());

    if (token.empty())
    {
        return false;
    }

    lock_guard<mutex> lock(sessionMutex);

    auto it = sessions.find(token);

    if (it == sessions.end())
    {
        return false;
    }

    // Check session expiration
    if (chrono::system_clock::now() >= it->second.expiresAt)
    {
        sessions.erase(it);
        return false;
    }

    session = it->second;

    return true;
}

// ============================================
// ADMINISTRATOR AUTHORIZATION
// ============================================

bool ApiServer::requireAdministrator(
    const httplib::Request& req,
    httplib::Response& res,
    SessionInfo& session
)
{
    if (!getAuthenticatedSession(req, session))
    {
        res.status = 401;

        json response;

        response["status"] = "error";
        response["message"] = "Unauthorized. Please log in first.";

        res.set_content(
            response.dump(),
            "application/json"
        );

        return false;
    }

    if (session.role != "Administrator")
    {
        res.status = 403;

        json response;

        response["status"] = "error";
        response["message"] =
            "Access denied. Administrator permission required.";

        res.set_content(
            response.dump(),
            "application/json"
        );

        return false;
    }

    return true;
}

// ============================================
// START SERVER
// ============================================

void ApiServer::start()
{
    // ========================================
    // 1. CONNECT DATABASE
    // ========================================

    cout << "Connecting to PostgreSQL..." << endl;

    if (!database.connectDatabase())
    {
        cerr << "Cannot start API Server: Database connection failed!"
             << endl;

        return;
    }

    // ========================================
    // 2. CORS PREFLIGHT
    // ========================================

    server.Options(
        R"(/api/.*)",
        [](const httplib::Request& req,
           httplib::Response& res)
        {
            setCorsHeaders(res);
            res.status = 204;
        }
    );

    // ========================================
    // 3. HEALTH CHECK
    // ========================================

    server.Get(
        "/api/health",
        [](const httplib::Request& req,
           httplib::Response& res)
        {
            setCorsHeaders(res);

            json response;

            response["status"] = "success";
            response["message"] = "SAMS API is running";
            response["database"] = "connected";

            res.set_content(
                response.dump(),
                "application/json"
            );
        }
    );

    // ========================================
    // 4. LOGIN API
    // ========================================

    server.Post(
        "/api/auth/login",
        [this](const httplib::Request& req,
               httplib::Response& res)
        {
            setCorsHeaders(res);

            try
            {
                json request = json::parse(req.body);

                if (!request.contains("username") ||
                    !request.contains("password") ||
                    !request["username"].is_string() ||
                    !request["password"].is_string())
                {
                    res.status = 400;

                    json response;

                    response["status"] = "error";
                    response["message"] =
                        "Username and password are required";

                    res.set_content(
                        response.dump(),
                        "application/json"
                    );

                    return;
                }

                string username = request["username"];
                string password = request["password"];

                User* user = userController.login(
                    username,
                    password
                );

                if (user == nullptr)
                {
                    res.status = 401;

                    json response;

                    response["status"] = "error";
                    response["message"] =
                        "Invalid username or password";

                    res.set_content(
                        response.dump(),
                        "application/json"
                    );

                    return;
                }

                // Get user role

                string role;

                switch (user->getRole())
                {
                    case Role::STUDENT:
                        role = "Student";
                        break;

                    case Role::LECTURER:
                        role = "Lecturer";
                        break;

                    case Role::ADMINISTRATOR:
                        role = "Administrator";
                        break;
                }

                // Generate token

                string token = generateToken();

                if (token.empty())
                {
                    delete user;

                    res.status = 500;

                    json response;

                    response["status"] = "error";
                    response["message"] =
                        "Could not generate authentication token";

                    res.set_content(
                        response.dump(),
                        "application/json"
                    );

                    return;
                }

                // Create session

                SessionInfo session;

                session.userId = user->getUserId();
                session.username = user->getUsername();
                session.role = role;

                session.expiresAt =
                    chrono::system_clock::now()
                    + chrono::hours(8);

                {
                    lock_guard<mutex> lock(sessionMutex);

                    sessions[token] = session;
                }

                // Prepare response

                json response;

                response["status"] = "success";
                response["message"] = "Login successful";

                response["data"]["token"] = token;

                response["data"]["userId"] =
                    session.userId;

                response["data"]["username"] =
                    session.username;

                response["data"]["role"] =
                    session.role;

                response["data"]["expiresIn"] = 28800;

                delete user;

                res.status = 200;

                res.set_content(
                    response.dump(),
                    "application/json"
                );
            }
            catch (const json::exception& e)
            {
                res.status = 400;

                json response;

                response["status"] = "error";
                response["message"] = "Invalid JSON request";

                res.set_content(
                    response.dump(),
                    "application/json"
                );
            }
        }
    );

    // ========================================
    // 5. GET CURRENT USER
    // ========================================

    server.Get(
        "/api/auth/me",
        [this](const httplib::Request& req,
               httplib::Response& res)
        {
            setCorsHeaders(res);

            SessionInfo session;

            if (!getAuthenticatedSession(req, session))
            {
                res.status = 401;

                json response;

                response["status"] = "error";
                response["message"] = "Invalid or expired token";

                res.set_content(
                    response.dump(),
                    "application/json"
                );

                return;
            }

            json response;

            response["status"] = "success";
            response["message"] = "Authenticated user";

            response["data"]["userId"] = session.userId;
            response["data"]["username"] = session.username;
            response["data"]["role"] = session.role;

            res.status = 200;

            res.set_content(
                response.dump(),
                "application/json"
            );
        }
    );

    // ========================================
    // 6. LOGOUT
    // ========================================

    server.Post(
        "/api/auth/logout",
        [this](const httplib::Request& req,
               httplib::Response& res)
        {
            setCorsHeaders(res);

            if (!req.has_header("Authorization"))
            {
                res.status = 401;

                json response;

                response["status"] = "error";
                response["message"] =
                    "Authorization token is required";

                res.set_content(
                    response.dump(),
                    "application/json"
                );

                return;
            }

            string authorization =
                req.get_header_value("Authorization");

            const string prefix = "Bearer ";

            if (authorization.rfind(prefix, 0) != 0)
            {
                res.status = 401;

                json response;

                response["status"] = "error";
                response["message"] =
                    "Invalid authorization format";

                res.set_content(
                    response.dump(),
                    "application/json"
                );

                return;
            }

            string token = authorization.substr(prefix.length());

            bool removed = false;

            {
                lock_guard<mutex> lock(sessionMutex);

                removed = (sessions.erase(token) > 0);
            }

            if (!removed)
            {
                res.status = 401;

                json response;

                response["status"] = "error";
                response["message"] =
                    "Invalid or expired token";

                res.set_content(
                    response.dump(),
                    "application/json"
                );

                return;
            }

            json response;

            response["status"] = "success";
            response["message"] = "Logout successful";

            res.status = 200;

            res.set_content(
                response.dump(),
                "application/json"
            );
        }
    );

    // ========================================
    // 7. GET ALL USER ACCOUNTS
    // ========================================

    server.Get(
        "/api/admin/users",
        [this](const httplib::Request& req,
               httplib::Response& res)
        {
            setCorsHeaders(res);

            SessionInfo session;

            if (!requireAdministrator(req, res, session))
            {
                return;
            }

            vector<UserAccountInfo> accounts;

            bool success =
                administratorController.getUserAccounts(accounts);

            if (!success)
            {
                res.status = 500;

                json response;

                response["status"] = "error";
                response["message"] =
                    "Failed to retrieve user accounts";

                res.set_content(
                    response.dump(),
                    "application/json"
                );

                return;
            }

            json response;

            response["status"] = "success";
            response["message"] =
                "User accounts retrieved successfully";

            response["data"] = json::array();

            for (const auto& account : accounts)
            {
                json item;

                item["userId"] = account.userId;
                item["username"] = account.username;
                item["role"] = account.role;
                item["fullName"] = account.fullName;
                item["email"] = account.email;
                item["phone"] = account.phone;
                item["active"] = account.active;

                response["data"].push_back(item);
            }

            response["total"] = accounts.size();

            res.status = 200;

            res.set_content(
                response.dump(),
                "application/json"
            );
        }
    );

    // ========================================
    // 8. CREATE USER ACCOUNT
    // ========================================

    server.Post(
        "/api/admin/users",
        [this](const httplib::Request& req,
               httplib::Response& res)
        {
            setCorsHeaders(res);

            SessionInfo session;

            if (!requireAdministrator(req, res, session))
            {
                return;
            }

            try
            {
                json request = json::parse(req.body);

                // Check required fields

                vector<string> requiredFields = {
                    "userId",
                    "username",
                    "password",
                    "role",
                    "fullName"
                };

                for (const string& field : requiredFields)
                {
                    if (!request.contains(field) ||
                        !request[field].is_string() ||
                        request[field].get<string>().empty())
                    {
                        res.status = 400;

                        json response;

                        response["status"] = "error";
                        response["message"] =
                            field + " is required";

                        res.set_content(
                            response.dump(),
                            "application/json"
                        );

                        return;
                    }
                }

                // Read required information

                string userId = request["userId"];
                string username = request["username"];
                string password = request["password"];
                string role = request["role"];
                string fullName = request["fullName"];

                // Read optional information

                string email = request.value("email", "");
                string phone = request.value("phone", "");
                string address = request.value("address", "");
                string dateOfBirth =
                    request.value("dateOfBirth", "");
                string gender = request.value("gender", "");
                string departmentId =
                    request.value("departmentId", "");

                // Call existing controller function

                bool success =
                    administratorController.createUserAccount(
                        userId,
                        username,
                        password,
                        role,
                        fullName,
                        email,
                        phone,
                        address,
                        dateOfBirth,
                        gender,
                        departmentId
                    );

                if (!success)
                {
                    res.status = 400;

                    json response;

                    response["status"] = "error";
                    response["message"] =
                        "Failed to create user account";

                    res.set_content(
                        response.dump(),
                        "application/json"
                    );

                    return;
                }

                json response;

                response["status"] = "success";
                response["message"] =
                    "User account created successfully";

                response["data"]["userId"] = userId;
                response["data"]["username"] = username;
                response["data"]["role"] = role;

                res.status = 201;

                res.set_content(
                    response.dump(),
                    "application/json"
                );
            }
            catch (const json::exception& e)
            {
                res.status = 400;

                json response;

                response["status"] = "error";
                response["message"] = "Invalid JSON request";

                res.set_content(
                    response.dump(),
                    "application/json"
                );
            }
        }
    );

      server.Put(
        R"(/api/admin/users/([A-Za-z0-9]+))",
        [this](const httplib::Request& req,
               httplib::Response& res)
        {
            setCorsHeaders(res);

            SessionInfo session;

            if (!requireAdministrator(req, res, session))
            {
                return;
            }

            try
            {
                // Get userId from URL
                string userId = req.matches[1];

                json request = json::parse(req.body);

                // Validate newUsername
                if (!request.contains("newUsername") ||
                    !request["newUsername"].is_string() ||
                    request["newUsername"].get<string>().empty())
                {
                    res.status = 400;

                    json response;

                    response["status"] = "error";
                    response["message"] =
                        "newUsername is required";

                    res.set_content(
                        response.dump(),
                        "application/json"
                    );

                    return;
                }

                string newUsername = request["newUsername"];

                // Call existing controller function
                bool success =
                    administratorController.updateUserAccount(
                        userId,
                        newUsername
                    );

                if (!success)
                {
                    res.status = 400;

                    json response;

                    response["status"] = "error";
                    response["message"] =
                        "Failed to update user account";

                    res.set_content(
                        response.dump(),
                        "application/json"
                    );

                    return;
                }

                json response;

                response["status"] = "success";
                response["message"] =
                    "User account updated successfully";

                response["data"]["userId"] = userId;
                response["data"]["username"] = newUsername;

                res.status = 200;

                res.set_content(
                    response.dump(),
                    "application/json"
                );
            }
            catch (const json::exception& e)
            {
                res.status = 400;

                json response;

                response["status"] = "error";
                response["message"] = "Invalid JSON request";

                res.set_content(
                    response.dump(),
                    "application/json"
                );
            }
        }
    );

    if (!server.set_mount_point("/frontend", "./frontend"))
{
    cerr << "Cannot mount main frontend!" << endl;
}

// Lecturer Frontend
if (!server.set_mount_point(
    "/lecturer",
    "./ModuleLecturer/FrontEnd/html"))
{
    cerr << "Cannot mount lecturer frontend!" << endl;
}

// ========================================
// 11. START SERVER
// ========================================

cout << "Starting SAMS API Server..." << endl;

bool serverStarted = server.listen(
    "127.0.0.1",
    8080
);

if (!serverStarted)
{
    cerr << "ERROR: Cannot start API Server!" << endl;

    cerr << "Possible causes: port 8080 is unavailable "
         << "or socket initialization failed."
         << endl;
}
else
{
    cout << "SAMS API Server stopped." << endl;
}
}
