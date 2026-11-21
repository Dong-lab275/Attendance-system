#include "AdministratorController.h"
#include <iostream>
#include <libpq-fe.h>
#include <cstdlib>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
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
        if (result != nullptr){
            PQclear(result);
        }
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

    cout << "Student exists: " << PQgetvalue(checkResult, 0, 0) << endl;

    cout << "Course exists: "<< PQgetvalue(checkResult, 0, 1) << endl;

    cout << "Enrollment exists: "<< PQgetvalue(checkResult, 0, 2) << endl;

    bool studentExists = string(PQgetvalue(checkResult, 0, 0)) == "t";

    bool courseExists = string(PQgetvalue(checkResult, 0, 1)) == "t";

    bool enrollmentExists = string(PQgetvalue(checkResult, 0, 2)) == "t";

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

    if (semester < 0) {
        cout << "Invalid semester!" << endl;
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
    "INSERT INTO courses "
    "(course_id, course_name, description, academic_year, semester, lecturer_id) "
    "VALUES ($1, $2, $3, NULLIF($4, ''), NULLIF($5, '')::INTEGER, NULLIF($6, ''))";

    string semesterStr = (semester == 0) ? "" : to_string(semester);

    const char* params[6] = {
        courseId.c_str(),
        courseName.c_str(),
        description.c_str(),
        academicYear.c_str(),
        semesterStr.c_str(),
        lecturerId.c_str()
    };

    PGresult* result = PQexecParams(
        connection, query, 6, nullptr,
        params, nullptr, nullptr, 0
    );

    if (result == nullptr || PQresultStatus(result) != PGRES_COMMAND_OK) {

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

    if (semester < 0) {
        cout << "Invalid semester!" << endl;
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
    "UPDATE courses SET "
    "course_name = $1, "
    "description = $2, "
    "academic_year = NULLIF($3, ''), "
    "semester = NULLIF($4, '')::INTEGER, "
    "lecturer_id = NULLIF($5, '') "
    "WHERE course_id = $6";

    string semesterStr = (semester == 0) ? "" : to_string(semester);

    const char* params[6] = {
        courseName.c_str(),
        description.c_str(),
        academicYear.c_str(),
        semesterStr.c_str(),
        lecturerId.c_str(),
        courseId.c_str()
    };

    PGresult* result = PQexecParams(
        connection, query, 6, nullptr,
        params, nullptr, nullptr, 0
    );

    if (result == nullptr || PQresultStatus(result) != PGRES_COMMAND_OK) {

        cout << "Update course failed: " << PQerrorMessage(connection) << endl;

        if (result != nullptr){
            PQclear(result);
        }

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
        "SELECT course_id, course_name, description, "
        "academic_year, semester, lecturer_id "
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
        cout << "Course ID: " << PQgetvalue(result, i, 0) << endl;
        cout << "Course Name: " << PQgetvalue(result, i, 1) << endl;
        cout << "Description: " << PQgetvalue(result, i, 2) << endl;
        cout << "Academic Year: " << PQgetvalue(result, i, 3) << endl;

        cout << "Semester: ";
        if (PQgetisnull(result, i, 4))
            cout << "Not assigned" << endl;
        else
            cout << PQgetvalue(result, i, 4) << endl;

        cout << "Lecturer ID: ";
        if (PQgetisnull(result, i, 5))
            cout << "Not assigned" << endl;
        else
            cout << PQgetvalue(result, i, 5) << endl;

        cout << "--------------------------------" << endl;
    }

    cout << "Total courses: " << PQntuples(result) << endl;

    PQclear(result);
    return true;
}

bool AdministratorController::viewCourseEnrollments()
{
     PGresult* result = PQexec(
        database->getConnection(),
        "SELECT registration_id, student_id, course_id, "
        "registration_date, registration_status "
        "FROM course_registrations "
        "ORDER BY registration_date DESC"
    );

    if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        cout << "Failed to retrieve course enrollments!\n";
        PQclear(result);
        return false;
    }

    cout << "\n========== COURSE ENROLLMENTS ==========\n";

    for (int i = 0; i < PQntuples(result); i++)
    {
        cout << "Registration ID: " << PQgetvalue(result, i, 0) << endl;
        cout << "Student ID: " << PQgetvalue(result, i, 1) << endl;
        cout << "Course ID: " << PQgetvalue(result, i, 2) << endl;
        cout << "Registration Date: " << PQgetvalue(result, i, 3) << endl;
        cout << "Status: " << PQgetvalue(result, i, 4) << endl;
        cout << "----------------------------------------\n";
    }

    PQclear(result);
    return true;
}

bool AdministratorController::deleteCourseEnrollment(string registrationId)
{
    if (registrationId.empty())
    {
        cout << "Registration ID cannot be empty!\n";
        return false;
    }

    const char* params[] = {
        registrationId.c_str()
    };

    PGresult* result = PQexecParams(
        database->getConnection(),
        "DELETE FROM course_registrations WHERE registration_id = $1",
        1, nullptr, params, nullptr, nullptr, 0
    );

    bool success =
        PQresultStatus(result) == PGRES_COMMAND_OK &&
        string(PQcmdTuples(result)) == "1";

    if (success)
        cout << "Course enrollment deleted successfully!\n";
    else
        cout << "Registration not found or deletion failed!\n";

    PQclear(result);
    return success;
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

string hashUserPassword(string password)
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
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }

    return ss.str();
}

bool AdministratorController::createUserAccount(string userId,string username,string password,string role,string fullName,
    string email,string phone,string address,string dateOfBirth,string gender,string departmentId)
{
    // 1. Validate input
    if (userId.empty() ||username.empty() || password.empty() ||
        role.empty() || fullName.empty())
    {
        cerr << "Required information cannot be empty!" << endl;
        return false;
    }

    if (userId.length() != 12)
    {
        cerr << "Invalid User ID!" << endl;
        return false;
    }

    if (role != "Student" &&role != "Lecturer" && role != "Administrator")
    {
        cerr << "Invalid role!" << endl;
        return false;
    }

    if (role == "Lecturer" && departmentId.empty())
    {
        cerr << "Lecturer must have a Department ID!" << endl;
        return false;
    }

    if (departmentId.length() > 5)
    {
        cerr << "Invalid Department ID!" << endl;
        return false;
    }

    // 2. Check database connection
    if (database == nullptr)
    {
        cerr << "Database connection is not available!" << endl;
        return false;
    }

    PGconn* connection = database->getConnection();

    if (connection == nullptr ||
        PQstatus(connection) != CONNECTION_OK)
    {
        cerr << "Database connection failed!" << endl;
        return false;
    }

    // 3. Hash password using SHA-256
    string passwordHash = hashUserPassword(password);

    // 4. Begin transaction
    PGresult* result = PQexec(connection, "BEGIN");

    if (result == nullptr ||
        PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        if (result != nullptr)
            PQclear(result);

        cerr << "Cannot start transaction!" << endl;
        return false;
    }

    PQclear(result);

    // 5. Insert user account
    const char* userQuery =
        "INSERT INTO users "
        "(user_id, username, password, role) "
        "VALUES ($1, $2, $3, $4)";

    const char* userParams[4] = {
        userId.c_str(),
        username.c_str(),
        passwordHash.c_str(),
        role.c_str()
    };

    result = PQexecParams(
        connection,
        userQuery,
        4,
        nullptr,
        userParams,
        nullptr,
        nullptr,
        0
    );

    if (result == nullptr ||
        PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        cerr << "Create user failed: "
             << PQerrorMessage(connection) << endl;

        if (result != nullptr)
            PQclear(result);

        PQexec(connection, "ROLLBACK");
        return false;
    }

    PQclear(result);

    // 6. Insert corresponding profile
    const char* profileQuery = nullptr;
    const char* profileParams[8];

    if (role == "Student")
    {
        profileQuery =
            "INSERT INTO students "
            "(student_id, full_name, email, phone, address, "
            "date_of_birth, gender, user_id) "
            "VALUES ($1, $2, $3, $4, $5, "
            "NULLIF($6, '')::DATE, NULLIF($7, ''), $8)";

        profileParams[0] = userId.c_str();
        profileParams[1] = fullName.c_str();
        profileParams[2] = email.c_str();
        profileParams[3] = phone.c_str();
        profileParams[4] = address.c_str();
        profileParams[5] = dateOfBirth.c_str();
        profileParams[6] = gender.c_str();
        profileParams[7] = userId.c_str();
    }
    else if (role == "Lecturer")
    {
        profileQuery =
            "INSERT INTO lecturers "
            "(lecturer_id, full_name, email, phone, "
            "date_of_birth, gender, department_id, user_id) "
            "VALUES ($1, $2, $3, $4, "
            "NULLIF($5, '')::DATE, NULLIF($6, ''), $7, $8)";

        profileParams[0] = userId.c_str();
        profileParams[1] = fullName.c_str();
        profileParams[2] = email.c_str();
        profileParams[3] = phone.c_str();
        profileParams[4] = dateOfBirth.c_str();
        profileParams[5] = gender.c_str();
        profileParams[6] = departmentId.c_str();
        profileParams[7] = userId.c_str();
    }
    else
    {
        profileQuery =
            "INSERT INTO administrators "
            "(admin_id, full_name, email, phone, user_id) "
            "VALUES ($1, $2, $3, $4, $5)";

        profileParams[0] = userId.c_str();
        profileParams[1] = fullName.c_str();
        profileParams[2] = email.c_str();
        profileParams[3] = phone.c_str();
        profileParams[4] = userId.c_str();
    }

    result = PQexecParams(
        connection,
        profileQuery,
        role == "Lecturer" || role == "Student" ? 8 : 5,
        nullptr,
        profileParams,
        nullptr,
        nullptr,
        0
    );

    if (result == nullptr ||
        PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        cerr << "Create profile failed: "
             << PQerrorMessage(connection) << endl;

        if (result != nullptr)
            PQclear(result);

        PQexec(connection, "ROLLBACK");
        return false;
    }

    PQclear(result);

    // 7. Commit transaction
    result = PQexec(connection, "COMMIT");

    if (result == nullptr ||
        PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        if (result != nullptr)
            PQclear(result);

        PQexec(connection, "ROLLBACK");
        cerr << "Transaction commit failed!" << endl;
        return false;
    }

    PQclear(result);

    cout << "User account created successfully!" << endl;
    cout << "Username: " << username << endl;
    cout << "Role: " << role << endl;

    return true;
}

bool AdministratorController::viewUserAccounts()
{
     if (database == nullptr)
        return false;

    PGconn* connection = database->getConnection();

    if (connection == nullptr ||
        PQstatus(connection) != CONNECTION_OK)
        return false;

    const char* query =
        "SELECT user_id, username, role, is_active, created_at "
        "FROM users ORDER BY user_id";

    PGresult* result = PQexec(connection, query);

    if (result == nullptr ||
        PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        cerr << "View user accounts failed: "
             << PQerrorMessage(connection) << endl;

        if (result != nullptr)
            PQclear(result);

        return false;
    }

    cout << "\n========== USER ACCOUNT LIST ==========\n";

    for (int i = 0; i < PQntuples(result); i++)
    {
        cout << "User ID: " << PQgetvalue(result, i, 0) << endl;
        cout << "Username: " << PQgetvalue(result, i, 1) << endl;
        cout << "Role: " << PQgetvalue(result, i, 2) << endl;
        cout << "Status: "
             << (string(PQgetvalue(result, i, 3)) == "t"
                 ? "Active" : "Locked") << endl;
        cout << "Created at: " << PQgetvalue(result, i, 4) << endl;
        cout << "---------------------------------------\n";
    }

    cout << "Total accounts: " << PQntuples(result) << endl;

    PQclear(result);
    return true;
}

bool AdministratorController::updateUserAccount(string userId, string newUsername)
{
    if (userId.empty() || newUsername.empty())
        return false;

    if (userId.length() != 12 || newUsername.length() > 100)
        return false;

    if (database == nullptr)
        return false;

    PGconn* connection = database->getConnection();

    if (connection == nullptr ||
        PQstatus(connection) != CONNECTION_OK)
        return false;

    const char* query =
        "UPDATE users "
        "SET username = $1, updated_at = CURRENT_TIMESTAMP "
        "WHERE user_id = $2";

    const char* params[2] = {
        newUsername.c_str(),
        userId.c_str()
    };

    PGresult* result = PQexecParams(
        connection, query, 2, nullptr,
        params, nullptr, nullptr, 0
    );

    if (result == nullptr ||
        PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        cerr << "Update user account failed: "
             << PQerrorMessage(connection) << endl;

        if (result != nullptr)
            PQclear(result);

        return false;
    }

    bool updated = atoi(PQcmdTuples(result)) > 0;
    PQclear(result);

    if (!updated)
    {
        cerr << "User account does not exist!" << endl;
        return false;
    }

    cout << "User account updated successfully!" << endl;
    return true;
}


bool AdministratorController::addDepartment(string departmentId, string departmentName)
{
    if (departmentId.empty() || departmentName.empty())
    {
        cout << "Department information cannot be empty!\n";
        return false;
    }

    const char* params[] = {
        departmentId.c_str(),
        departmentName.c_str()
    };

    PGresult* result = PQexecParams(
        database->getConnection(),
        "INSERT INTO departments (department_id, department_name) VALUES ($1, $2)",
        2, nullptr, params, nullptr, nullptr, 0
    );

    bool success = PQresultStatus(result) == PGRES_COMMAND_OK;

    if (success)
        cout << "Department added successfully!\n";
    else
        cout << "Failed to add department!\n";

    PQclear(result);
    return success;
}

bool AdministratorController::viewDepartments()
{
     if (database == nullptr)
    {
        cerr << "Database connection is not available!" << endl;
        return false;
    }

    PGconn* connection = database->getConnection();

    if (connection == nullptr || PQstatus(connection) != CONNECTION_OK)
    {
        cerr << "Database connection failed!" << endl;
        return false;
    }

    const char* query =
        "SELECT department_id, department_name "
        "FROM departments "
        "ORDER BY department_id";

    PGresult* result = PQexec(connection, query);

    if (result == nullptr ||
        PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        cerr << "Failed to retrieve departments: "
             << PQerrorMessage(connection) << endl;

        if (result != nullptr)
            PQclear(result);

        return false;
    }

    cout << "\n========== DEPARTMENT LIST ==========\n";

    for (int i = 0; i < PQntuples(result); i++)
    {
        cout << "Department ID: " << PQgetvalue(result, i, 0) << endl;
        cout << "Department Name: " << PQgetvalue(result, i, 1) << endl;
        cout << "-------------------------------------\n";
    }

    cout << "Total departments: " << PQntuples(result) << endl;

    PQclear(result);
    return true;
}

bool AdministratorController::updateDepartment(string departmentId, string departmentName)
{
     if (departmentId.empty() || departmentName.empty())
    {
        cout << "Department information cannot be empty!\n";
        return false;
    }

    const char* params[] = {
        departmentName.c_str(),
        departmentId.c_str()
    };

    PGresult* result = PQexecParams(
        database->getConnection(),
        "UPDATE departments SET department_name = $1 WHERE department_id = $2",
        2, nullptr, params, nullptr, nullptr, 0
    );

    bool success =
        PQresultStatus(result) == PGRES_COMMAND_OK &&
        string(PQcmdTuples(result)) == "1";

    if (success)
        cout << "Department updated successfully!\n";
    else
        cout << "Department not found or update failed!\n";

    PQclear(result);
    return success;
}

bool AdministratorController::deleteDepartment(string departmentId)
{
    if (departmentId.empty())
    {
        cout << "Department ID cannot be empty!\n";
        return false;
    }

    const char* params[] = {
        departmentId.c_str()
    };

    PGresult* result = PQexecParams(
        database->getConnection(),
        "DELETE FROM departments WHERE department_id = $1",
        1, nullptr, params, nullptr, nullptr, 0
    );

    bool success =
        PQresultStatus(result) == PGRES_COMMAND_OK &&
        string(PQcmdTuples(result)) == "1";

    if (success)
        cout << "Department deleted successfully!\n";
    else
        cout << "Department not found or is being used by lecturers!\n";

    PQclear(result);
    return success;
}

bool AdministratorController::assignLecturerToCourse(string courseId, string lecturerId)
{
     if (courseId.empty() || lecturerId.empty())
    {
        cout << "Course ID and Lecturer ID cannot be empty!\n";
        return false;
    }

    const char* params[] = {
        lecturerId.c_str(),
        courseId.c_str()
    };

    PGresult* result = PQexecParams(
        database->getConnection(),
        "UPDATE courses SET lecturer_id = $1 WHERE course_id = $2",
        2, nullptr, params, nullptr, nullptr, 0
    );

    bool success =
        PQresultStatus(result) == PGRES_COMMAND_OK &&
        string(PQcmdTuples(result)) == "1";

    if (success)
        cout << "Lecturer assigned successfully!\n";
    else
        cout << "Assignment failed. Check Course ID and Lecturer ID!\n";

    PQclear(result);
    return success;
}

bool AdministratorController::unassignLecturerFromCourse(string courseId)
{
    if (courseId.empty())
    {
        cout << "Course ID cannot be empty!\n";
        return false;
    }

    const char* params[] = {
        courseId.c_str()
    };

    PGresult* result = PQexecParams(
        database->getConnection(),
        "UPDATE courses SET lecturer_id = NULL WHERE course_id = $1",
        1, nullptr, params, nullptr, nullptr, 0
    );

    bool success =
        PQresultStatus(result) == PGRES_COMMAND_OK &&
        string(PQcmdTuples(result)) == "1";

    if (success)
        cout << "Lecturer unassigned successfully!\n";
    else
        cout << "Course not found or unassignment failed!\n";

    PQclear(result);
    return success;
}

bool AdministratorController::viewAttendanceHistory()
{
     PGresult* result = PQexec(
        database->getConnection(),
        "SELECT ar.attendance_id, "
        "ar.student_id, "
        "s.full_name, "
        "ar.session_id, "
        "cs.course_id, "
        "ar.attendance_timestamp, "
        "ar.status "
        "FROM attendance_records ar "
        "JOIN students s ON ar.student_id = s.student_id "
        "JOIN class_sessions cs ON ar.session_id = cs.session_id "
        "ORDER BY ar.attendance_timestamp DESC"
    );

    if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        cout << "Failed to retrieve attendance history!\n";
        PQclear(result);
        return false;
    }

    cout << "\n========== ATTENDANCE HISTORY ==========\n";

    for (int i = 0; i < PQntuples(result); i++)
    {
        cout << "Attendance ID: " << PQgetvalue(result, i, 0) << endl;
        cout << "Student ID: " << PQgetvalue(result, i, 1) << endl;
        cout << "Student Name: " << PQgetvalue(result, i, 2) << endl;
        cout << "Session ID: " << PQgetvalue(result, i, 3) << endl;
        cout << "Course ID: " << PQgetvalue(result, i, 4) << endl;
        cout << "Attendance Time: " << PQgetvalue(result, i, 5) << endl;
        cout << "Status: " << PQgetvalue(result, i, 6) << endl;
        cout << "----------------------------------------\n";
    }

    PQclear(result);
    return true;
}

bool AdministratorController::viewAttendanceReports()
{
     PGresult* result = PQexec(
        database->getConnection(),
        "SELECT cs.session_id, "
        "cs.course_id, "
        "cs.class_date, "
        "COUNT(ar.attendance_id) AS total_students, "
        "COUNT(*) FILTER (WHERE ar.status = 'Present') AS present_count, "
        "COUNT(*) FILTER (WHERE ar.status = 'Absent') AS absent_count, "
        "COUNT(*) FILTER (WHERE ar.status = 'Late') AS late_count "
        "FROM class_sessions cs "
        "LEFT JOIN attendance_records ar "
        "ON cs.session_id = ar.session_id "
        "GROUP BY cs.session_id, cs.course_id, cs.class_date "
        "ORDER BY cs.class_date DESC"
    );

    if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        cout << "Failed to retrieve attendance reports!\n";
        PQclear(result);
        return false;
    }

    cout << "\n========== ATTENDANCE REPORTS ==========\n";

    for (int i = 0; i < PQntuples(result); i++)
    {
        cout << "Session ID: " << PQgetvalue(result, i, 0) << endl;
        cout << "Course ID: " << PQgetvalue(result, i, 1) << endl;
        cout << "Class Date: " << PQgetvalue(result, i, 2) << endl;
        cout << "Total Recorded: " << PQgetvalue(result, i, 3) << endl;
        cout << "Present: " << PQgetvalue(result, i, 4) << endl;
        cout << "Absent: " << PQgetvalue(result, i, 5) << endl;
        cout << "Late: " << PQgetvalue(result, i, 6) << endl;
        cout << "----------------------------------------\n";
    }

    PQclear(result);
    return true;
}

bool AdministratorController::viewClassSessionCalendar()
{
    PGresult* result = PQexec(
        database->getConnection(),
        "SELECT session_id, course_id, class_date, "
        "start_time, end_time, classroom, status "
        "FROM class_sessions "
        "ORDER BY class_date, start_time"
    );

    if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        cout << "Failed to retrieve class session calendar!\n";
        PQclear(result);
        return false;
    }

    cout << "\n========== CLASS SESSION CALENDAR ==========\n";

    for (int i = 0; i < PQntuples(result); i++)
    {
        cout << "Session ID: " << PQgetvalue(result, i, 0) << endl;
        cout << "Course ID: " << PQgetvalue(result, i, 1) << endl;
        cout << "Date: " << PQgetvalue(result, i, 2) << endl;
        cout << "Start Time: " << PQgetvalue(result, i, 3) << endl;
        cout << "End Time: " << PQgetvalue(result, i, 4) << endl;
        cout << "Classroom: " << PQgetvalue(result, i, 5) << endl;
        cout << "Status: " << PQgetvalue(result, i, 6) << endl;
        cout << "--------------------------------------------\n";
    }

    PQclear(result);
    return true;
}

bool AdministratorController::deleteUserAccount(string userId)
{
    if (userId.empty() || userId.length() != 12) {
        cout << "Invalid User ID!" << endl;
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
        "DELETE FROM users WHERE user_id = $1";

    const char* params[1] = {
        userId.c_str()
    };

    PGresult* result = PQexecParams(
        connection,
        query,
        1,
        nullptr,
        params,
        nullptr,
        nullptr,
        0
    );

    if (result == nullptr ||
        PQresultStatus(result) != PGRES_COMMAND_OK) {

        cout << "Delete user account failed: "
             << PQerrorMessage(connection) << endl;

        if (result != nullptr)
            PQclear(result);

        return false;
    }

    if (PQcmdTuples(result)[0] == '0') {
        cout << "User account not found!" << endl;
        PQclear(result);
        return false;
    }

    PQclear(result);

    cout << "User account deleted successfully!" << endl;
    return true;
}
