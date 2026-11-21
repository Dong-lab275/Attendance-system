#include "UserController.h"
#include <iostream>
#include <libpq-fe.h>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

string hashPassword(string password)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];

    SHA256(
        reinterpret_cast<const unsigned char*>(password.c_str()),
        password.length(),
        hash
    );

    stringstream ss;

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << setw(2) << setfill('0')
           << static_cast<int>(hash[i]);
    }

    return ss.str();
}


UserController::UserController(DatabaseConnection *database)
{
    this->database = database;
}

User *UserController::login(string username, string password)
{
    currentUserId.clear();
    if (username.empty() || password.empty()){
        return nullptr;
    }
    else if (database == nullptr){
        return nullptr;
    }

    PGconn* connection = database->getConnection();

    if (connection == nullptr || PQstatus(connection) != CONNECTION_OK){
        return nullptr;
    }

    const char* query =
        "SELECT user_id, username, password, role, is_active "
        "FROM users "
        "WHERE username = $1";

    const char* parameters[1] = { username.c_str() };

    PGresult* result = PQexecParams(connection,query,1,
        nullptr,parameters,nullptr,nullptr,0);

    if (result == nullptr){
    cerr << "Login query failed!" << endl;
    return nullptr;
    }
        
    if (PQresultStatus(result) != PGRES_TUPLES_OK){
        cerr << "Login query failed: "<< PQerrorMessage(connection) << endl;
        PQclear(result);
        return nullptr;
    }

    if (PQntuples(result) != 1){
        PQclear(result);
        return nullptr;
    }

    string userId = PQgetvalue(result, 0, 0);
    while (!userId.empty() && userId.back() == ' '){
    userId.pop_back();
    }
    string storedUsername = PQgetvalue(result, 0, 1);
    string passwordHash = PQgetvalue(result, 0, 2);
    string roleString = PQgetvalue(result, 0, 3);
    bool active = (string(PQgetvalue(result, 0, 4)) == "t");

    PQclear(result);

    if (!active){
        return nullptr;
    }

    Role role;

    if (roleString == "Student"){
        role = Role::STUDENT;
    }
    else if (roleString == "Lecturer"){
        role = Role::LECTURER;
    }
    else if (roleString == "Administrator"){
        role = Role::ADMINISTRATOR;
    }
    else{
        return nullptr;
    }

    User* user = new User(userId,storedUsername,
        passwordHash,role,active);

    if (!user->verifyPassword(password)){
        delete user;
        return nullptr;
    }
    currentUserId = userId;
    return user;
}


bool UserController::changePassword(string oldPassword, string newPassword, string confirmPassword)
{
     if (currentUserId.empty())
    {
        cout << "Please log in first!" << endl;
        return false;
    }

    if (oldPassword.empty() ||
        newPassword.empty() ||
        confirmPassword.empty())
    {
        cout << "Please fill in all information!" << endl;
        return false;
    }

    if (newPassword != confirmPassword)
    {
        cout << "New password and confirmation do not match!" << endl;
        return false;
    }

    if (newPassword.length() < 6)
    {
        cout << "New password must contain at least 6 characters!" << endl;
        return false;
    }

    if (oldPassword == newPassword)
    {
        cout << "New password must be different from old password!" << endl;
        return false;
    }

    if (database == nullptr)
    {
        return false;
    }

    PGconn* connection = database->getConnection();

    if (connection == nullptr ||
        PQstatus(connection) != CONNECTION_OK)
    {
        return false;
    }

    string oldHash = hashPassword(oldPassword);
    string newHash = hashPassword(newPassword);

    const char* parameters[] = {
        newHash.c_str(),
        currentUserId.c_str(),
        oldHash.c_str()
    };

    PGresult* result = PQexecParams(
        connection,
        "UPDATE users "
        "SET password = $1, updated_at = CURRENT_TIMESTAMP "
        "WHERE user_id = $2 "
        "AND password = $3 "
        "AND is_active = TRUE",
        3,
        nullptr,
        parameters,
        nullptr,
        nullptr,
        0
    );

    if (result == nullptr)
    {
        return false;
    }

    if (PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        cout << "Failed to change password!" << endl;
        PQclear(result);
        return false;
    }

    bool success = (string(PQcmdTuples(result)) == "1");

    if (success)
    {
        cout << "Password changed successfully!" << endl;
    }
    else
    {
        cout << "Incorrect old password or account not found!" << endl;
    }

    PQclear(result);

    return success;

}

bool UserController::forgotPassword(string email)
{
      if (email.empty())
    {
        cout << "Please enter your email address!" << endl;
        return false;
    }


    cout << "Email recovery service has not been implemented yet!"
         << endl;

    return false;
}

// Reset password - implementation requires recovery token verification
bool UserController::resetPassword(
    string recoveryToken,
    string newPassword,
    string confirmPassword
)
{
    if (recoveryToken.empty() ||
        newPassword.empty() ||
        confirmPassword.empty())
    {
        cout << "Please fill in all information!" << endl;
        return false;
    }

    if (newPassword != confirmPassword)
    {
        cout << "New password and confirmation do not match!" << endl;
        return false;
    }

    if (newPassword.length() < 6)
    {
        cout << "New password must contain at least 6 characters!" << endl;
        return false;
    }


    cout << "Password recovery token verification has not been implemented yet!"
         << endl;

    return false;
}
