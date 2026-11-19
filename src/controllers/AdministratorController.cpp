#include "AdministratorController.h"
#include <iostream>
#include <libpq-fe.h>

using namespace std;

AdministratorController::AdministratorController(DatabaseConnection* database)
{
    this->database = database;
}

string AdministratorController::generateRegistrationId()
{
    PGconn* connection = database->getConnection();
    const char* query =
        "SELECT COALESCE(MAX(registration_id), 'R00000000000') "
        "FROM course_registrations";

    PGresult* result = PQexec(connection, query);

    if (result == nullptr ||PQresultStatus(result) != PGRES_TUPLES_OK){
        if (result != nullptr)
            PQclear(result);
        return "";
    }

    string lastId = PQgetvalue(result, 0, 0);
    PQclear(result);
    int number = stoi(lastId.substr(1));
    number++;

    string numberString = to_string(number);
    string newId = "R" + string(11 - numberString.length(), '0') + numberString;

    return newId;
}

bool AdministratorController::addCourseEnrollment(string courseId, string studentId)
{
    if (courseId.empty() || studentId.empty()){
        cerr << "Course ID and Student ID cannot be empty!" << endl;
        return false;
    }

    if (database == nullptr){
        cerr << "Database connection is not available!" << endl;
        return false;
    }

    PGconn* connection = database->getConnection();

    if (connection == nullptr || PQstatus(connection) != CONNECTION_OK){
        cerr << "Database connection failed!" << endl;
        return false;
    }

    string registrationId = generateRegistrationId();

    if (registrationId.empty()){
        cerr << "Cannot generate registration ID!" << endl;
        return false;
    }

    const char* query =
        "INSERT INTO course_registrations "
        "(registration_id, registration_status, student_id, course_id) "
        "VALUES ($1, 'Pending', $2, $3)";

    const char* parameters[3] = {
        registrationId.c_str(),
        studentId.c_str(),
        courseId.c_str()
    };

    PGresult* result = PQexecParams(
        connection, query, 3, nullptr,
        parameters, nullptr, nullptr, 0
    );

    if (result == nullptr){
        cerr << "Add course enrollment failed!" << endl;
        return false;
    }

    if (PQresultStatus(result) != PGRES_COMMAND_OK){
        cerr << "Add course enrollment failed: "
             << PQerrorMessage(connection) << endl;

        PQclear(result);
        return false;
    }

    PQclear(result);

    cout << "Course enrollment added successfully! ID: "
         << registrationId << endl;

    return true;
}