
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

public:
    UserController(DatabaseConnection* database);

    User* login(string username, string password);
};

#endif
