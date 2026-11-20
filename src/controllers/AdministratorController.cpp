#include "AdministratorController.h"
#include <iostream>
#include <libpq-fe.h>
#include <cstdlib>

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

    if (courseId.length() != 5 || studentId.length() != 12) {
    cerr << "Invalid Course ID or Student ID format!" << endl;
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

    const char* checkQuery =
        "SELECT "
        "EXISTS (SELECT 1 FROM students WHERE student_id = $1), "
        "EXISTS (SELECT 1 FROM courses WHERE course_id = $2), "
        "EXISTS (SELECT 1 FROM course_registrations "
        "WHERE student_id = $1 AND course_id = $2)";

    const char* checkParams[2] = {
        studentId.c_str(),
        courseId.c_str()
    };

    PGresult* checkResult = PQexecParams(
        connection,
        checkQuery,
        2,
        nullptr,
        checkParams,
        nullptr,
        nullptr,
        0
    );

    if (checkResult == nullptr ||PQresultStatus(checkResult) != PGRES_TUPLES_OK) {

        cerr << "Data validation failed: " << PQerrorMessage(connection) << endl;

        if (checkResult != nullptr){
            PQclear(checkResult);
        }

        return false;
    }

    cout << "Student ID: " << studentId << endl;
    cout << "Course ID: " << courseId << endl;

    cout << "Student exists: "
     << PQgetvalue(checkResult, 0, 0) << endl;

    cout << "Course exists: "
     << PQgetvalue(checkResult, 0, 1) << endl;

    cout << "Enrollment exists: "
     << PQgetvalue(checkResult, 0, 2) << endl;

    bool studentExists =
    string(PQgetvalue(checkResult, 0, 0)) == "t";

    bool courseExists =
    string(PQgetvalue(checkResult, 0, 1)) == "t";

    bool enrollmentExists =
    string(PQgetvalue(checkResult, 0, 2)) == "t";

    PQclear(checkResult);

    if (!studentExists) {
        cerr << "Student does not exist!" << endl;
        return false;
    }

    if (!courseExists) {
        cerr << "Course does not exist!" << endl;
        return false;
    }

    if (enrollmentExists) {
        cerr << "Student is already registered for this course!" << endl;
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

bool AdministratorController::addCourse(string courseId, string courseName, string description, string academicYear, int semester, string lecturerId)
{
     if (courseId.empty() || courseName.empty()) {
        cout << "Course ID and Course Name cannot be empty!" << endl;
        return false;
    }

    if (courseId.length() != 5 || courseName.length() > 100) {
        cout << "Invalid course information!" << endl;
        return false;
    }

    if (database == nullptr) {
    cout << "Database connection is not available!" << endl;
    return false;
    }

    PGconn* connection = database->getConnection();

    if (connection == nullptr || PQstatus(connection) != CONNECTION_OK) {
        cout << "Database connection failed!" << endl;
        return false;
    }

    const char* query =
        "INSERT INTO courses (course_id, course_name) "
        "VALUES ($1, $2)";

    const char* params[2] = {
        courseId.c_str(),
        courseName.c_str()
    };

    PGresult* result = PQexecParams(
        connection, query, 2, nullptr,
        params, nullptr, nullptr, 0
    );

    if (result == nullptr ||
        PQresultStatus(result) != PGRES_COMMAND_OK) {

        cout << "Add course failed: "
             << PQerrorMessage(connection) << endl;

        if (result != nullptr)
            PQclear(result);

        return false;
    }

    PQclear(result);

    cout << "Course added successfully!" << endl;
    return true;
}

bool AdministratorController::updateCourse(string courseId, string courseName, string description, string academicYear, int semester, string lecturerId)
{
    if (courseId.empty() || courseName.empty()) {
        cout << "Course ID and Course Name cannot be empty!" << endl;
        return false;
    }

    if (courseId.length() != 5 || courseName.length() > 100) {
        cout << "Invalid course information!" << endl;
        return false;
    }

    if (database == nullptr) {
    cout << "Database connection is not available!" << endl;
    return false;
    }

    PGconn* connection = database->getConnection();

    if (connection == nullptr || PQstatus(connection) != CONNECTION_OK) {
        cout << "Database connection failed!" << endl;
        return false;
    }

    const char* query =
        "UPDATE courses SET course_name = $1 "
        "WHERE course_id = $2";

    const char* params[2] = {
        courseName.c_str(),
        courseId.c_str()
    };

    PGresult* result = PQexecParams(
        connection, query, 2, nullptr,
        params, nullptr, nullptr, 0
    );

    if (result == nullptr ||
        PQresultStatus(result) != PGRES_COMMAND_OK) {

        cout << "Update course failed: "
             << PQerrorMessage(connection) << endl;

        if (result != nullptr)
            PQclear(result);

        return false;
    }

    if (atoi(PQcmdTuples(result)) == 0) {
        cout << "Course does not exist!" << endl;
        PQclear(result);
        return false;
    }

    PQclear(result);

    cout << "Course updated successfully!" << endl;
    return true;
}

bool AdministratorController::deleteCourse(string courseId)
{
     if (courseId.empty() || courseId.length() != 5) {
        cout << "Invalid Course ID!" << endl;
        return false;
    }

    if (database == nullptr) {
    cout << "Database connection is not available!" << endl;
    return false;
    }

    PGconn* connection = database->getConnection();

    if (connection == nullptr || PQstatus(connection) != CONNECTION_OK) {
        cout << "Database connection failed!" << endl;
        return false;
    }

    const char* query =
        "DELETE FROM courses WHERE course_id = $1";

    const char* params[1] = {
        courseId.c_str()
    };

    PGresult* result = PQexecParams(
        connection, query, 1, nullptr,
        params, nullptr, nullptr, 0
    );

    if (result == nullptr ||
        PQresultStatus(result) != PGRES_COMMAND_OK) {

        cout << "Delete course failed: "
             << PQerrorMessage(connection) << endl;

        if (result != nullptr)
            PQclear(result);

        return false;
    }

    if (atoi(PQcmdTuples(result)) == 0) {
        cout << "Course does not exist!" << endl;
        PQclear(result);
        return false;
    }

    PQclear(result);

    cout << "Course deleted successfully!" << endl;
    return true;
}

bool AdministratorController::viewCourses()
{
    if (database == nullptr) {
    cout << "Database connection is not available!" << endl;
    return false;
    }

    PGconn* connection = database->getConnection();

    if (connection == nullptr || PQstatus(connection) != CONNECTION_OK) {
        cout << "Database connection failed!" << endl;
        return false;
    }

    const char* query =
        "SELECT course_id, course_name "
        "FROM courses ORDER BY course_id";

    PGresult* result = PQexec(connection, query);

    if (result == nullptr ||
        PQresultStatus(result) != PGRES_TUPLES_OK) {

        cout << "View courses failed: "
             << PQerrorMessage(connection) << endl;

        if (result != nullptr)
            PQclear(result);

        return false;
    }

    cout << "\n========== COURSE LIST ==========" << endl;

    for (int i = 0; i < PQntuples(result); i++) {
        cout << "Course ID: "
             << PQgetvalue(result, i, 0) << endl;

        cout << "Course Name: "
             << PQgetvalue(result, i, 1) << endl;

        cout << "--------------------------------" << endl;
    }

    cout << "Total courses: " << PQntuples(result) << endl;

    PQclear(result);
    return true;
}

bool AdministratorController::addClassSession(string sessionId, string classDate, string startTime, string endTime, string classroom, string courseId)
{
     if (sessionId.empty() || classDate.empty() ||
        startTime.empty() || endTime.empty() || courseId.empty())
    {
        cout << "Please enter all required information!" << endl;
        return false;
    }

    if (sessionId.length() != 5 || courseId.length() != 5)
    {
        cout << "Invalid Session ID or Course ID!" << endl;
        return false;
    }

    // Kiem tra ket noi database
    if (database == nullptr)
    {
        cout << "Database connection is not available!" << endl;
        return false;
    }

    PGconn* connection = database->getConnection();

    if (connection == nullptr || PQstatus(connection) != CONNECTION_OK)
    {
        cout << "Database connection failed!" << endl;
        return false;
    }

    // Kiem tra Course ID co ton tai khong
    const char* checkQuery =
        "SELECT EXISTS (SELECT 1 FROM courses WHERE course_id = $1)";

    const char* checkParams[1] = {courseId.c_str()};

    PGresult* checkResult = PQexecParams(
        connection, checkQuery, 1, nullptr,
        checkParams, nullptr, nullptr, 0
    );

    if (checkResult == nullptr ||
        PQresultStatus(checkResult) != PGRES_TUPLES_OK)
    {
        cout << "Cannot check course information!" << endl;

        if (checkResult != nullptr)
        {
            PQclear(checkResult);
        }

        return false;
    }

    bool courseExists =
        string(PQgetvalue(checkResult, 0, 0)) == "t";

    PQclear(checkResult);

    if (!courseExists)
    {
        cout << "Course does not exist!" << endl;
        return false;
    }

    // Them buoi hoc vao database
    const char* query =
        "INSERT INTO class_sessions "
        "(session_id, class_date, start_time, end_time, classroom, course_id) "
        "VALUES ($1, $2, $3, $4, $5, $6)";

    const char* params[6] = {
        sessionId.c_str(),
        classDate.c_str(),
        startTime.c_str(),
        endTime.c_str(),
        classroom.c_str(),
        courseId.c_str()
    };

    PGresult* result = PQexecParams(
        connection, query, 6, nullptr,
        params, nullptr, nullptr, 0
    );

    if (result == nullptr ||
        PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        cout << "Add Class Session failed: "
             << PQerrorMessage(connection) << endl;

        if (result != nullptr)
        {
            PQclear(result);
        }

        return false;
    }

    PQclear(result);

    cout << "Class Session added successfully!" << endl;
    return true;

}

bool AdministratorController::viewClassSessions()
{
    // Kiem tra database
    if (database == nullptr)
    {
        cout << "Database connection is not available!" << endl;
        return false;
    }

    PGconn* connection = database->getConnection();

    if (connection == nullptr || PQstatus(connection) != CONNECTION_OK)
    {
        cout << "Database connection failed!" << endl;
        return false;
    }

    // Lay danh sach buoi hoc
    const char* query =
        "SELECT session_id, class_date, start_time, end_time, "
        "classroom, course_id "
        "FROM class_sessions "
        "ORDER BY class_date, start_time";

    PGresult* result = PQexec(connection, query);

    if (result == nullptr ||
        PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        cout << "View Class Sessions failed!" << endl;

        if (result != nullptr)
        {
            PQclear(result);
        }

        return false;
    }

    // Hien thi danh sach buoi hoc
    cout << "\n========== CLASS SESSION LIST ==========" << endl;

    for (int i = 0; i < PQntuples(result); i++)
    {
        cout << "Session ID: " << PQgetvalue(result, i, 0) << endl;
        cout << "Class Date: " << PQgetvalue(result, i, 1) << endl;
        cout << "Start Time: " << PQgetvalue(result, i, 2) << endl;
        cout << "End Time: " << PQgetvalue(result, i, 3) << endl;
        cout << "Classroom: " << PQgetvalue(result, i, 4) << endl;
        cout << "Course ID: " << PQgetvalue(result, i, 5) << endl;

        cout << "----------------------------------------" << endl;
    }

    cout << "Total class sessions: " << PQntuples(result) << endl;

    PQclear(result);
    return true;
}

bool AdministratorController::updateClassSession(string sessionId, string classDate, string startTime, string endTime, string classroom, string courseId)
{
    // Kiem tra thong tin
    if (sessionId.empty() || classDate.empty() ||
        startTime.empty() || endTime.empty() || courseId.empty())
    {
        cout << "Please enter all required information!" << endl;
        return false;
    }

    if (sessionId.length() != 5 || courseId.length() != 5)
    {
        cout << "Invalid Session ID or Course ID!" << endl;
        return false;
    }

    // Kiem tra ket noi database
    if (database == nullptr)
    {
        cout << "Database connection is not available!" << endl;
        return false;
    }

    PGconn* connection = database->getConnection();

    if (connection == nullptr || PQstatus(connection) != CONNECTION_OK)
    {
        cout << "Database connection failed!" << endl;
        return false;
    }

    // Cap nhat thong tin buoi hoc
    const char* query =
        "UPDATE class_sessions "
        "SET class_date = $1, start_time = $2, end_time = $3, "
        "classroom = $4, course_id = $5 "
        "WHERE session_id = $6";

    const char* params[6] = {
        classDate.c_str(),
        startTime.c_str(),
        endTime.c_str(),
        classroom.c_str(),
        courseId.c_str(),
        sessionId.c_str()
    };

    PGresult* result = PQexecParams(
        connection, query, 6, nullptr,
        params, nullptr, nullptr, 0
    );

    if (result == nullptr ||
        PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        cout << "Update Class Session failed: "
             << PQerrorMessage(connection) << endl;

        if (result != nullptr)
        {
            PQclear(result);
        }

        return false;
    }

    // Kiem tra co buoi hoc nao duoc cap nhat khong
    if (atoi(PQcmdTuples(result)) == 0)
    {
        cout << "Class Session does not exist!" << endl;
        PQclear(result);
        return false;
    }

    PQclear(result);

    cout << "Class Session updated successfully!" << endl;
    return true;
}

bool AdministratorController::deleteClassSession(string sessionId)
{
    // Kiem tra Session ID
    if (sessionId.empty() || sessionId.length() != 5)
    {
        cout << "Invalid Session ID!" << endl;
        return false;
    }

    // Kiem tra database
    if (database == nullptr)
    {
        cout << "Database connection is not available!" << endl;
        return false;
    }

    PGconn* connection = database->getConnection();

    if (connection == nullptr || PQstatus(connection) != CONNECTION_OK)
    {
        cout << "Database connection failed!" << endl;
        return false;
    }

    // Xoa buoi hoc
    const char* query =
        "DELETE FROM class_sessions WHERE session_id = $1";

    const char* params[1] = {
        sessionId.c_str()
    };

    PGresult* result = PQexecParams(
        connection, query, 1, nullptr,
        params, nullptr, nullptr, 0
    );

    if (result == nullptr ||
        PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        cout << "Delete Class Session failed: "
             << PQerrorMessage(connection) << endl;

        if (result != nullptr)
        {
            PQclear(result);
        }

        return false;
    }

    // Kiem tra buoi hoc co ton tai khong
    if (atoi(PQcmdTuples(result)) == 0)
    {
        cout << "Class Session does not exist!" << endl;
        PQclear(result);
        return false;
    }

    PQclear(result);

    cout << "Class Session deleted successfully!" << endl;
    return true;
}
