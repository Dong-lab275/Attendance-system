
#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H

#include <string>
#include "../models/User.h"
#include "../database/DatabaseConnection.h"

using namespace std;

class UserController
{
private:
    DatabaseConnection* database;
    string currentUserId;

public:
    UserController(DatabaseConnection* database);
    // Authentication
    User* login(string username, string password);
    bool changePassword(string oldPassword, string newPassword, string confirmPassword);
    bool forgotPassword(string email);
    bool resetPassword(string recoveryToken,string newPassword,string confirmPassword);
};

#endif
