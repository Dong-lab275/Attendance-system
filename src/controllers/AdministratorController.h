#ifndef ADMINISTRATORCONTROLLER_H
#define ADMINISTRATORCONTROLLER_H

#include <string>
#include "../database/DatabaseConnection.h"

using namespace std;

class AdministratorController
{
private:
    DatabaseConnection* database;
    string generateRegistrationId();

public:
    AdministratorController(DatabaseConnection* database);
    bool addCourseEnrollment(string courseId, string studentId);
};

#endif