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
    bool addCourse(string courseId,string courseName,string description,
        string academicYear,int semester,string lecturerId);

    bool updateCourse(string courseId,string courseName,string description,
        string academicYear,int semester,string lecturerId);

    bool deleteCourse(string courseId);
    bool viewCourses();
    bool viewCourseEnrollments();
    bool deleteCourseEnrollment(string registrationId);

    bool addClassSession(string sessionId,string classDate,string startTime,
    string endTime,string classroom,string courseId);

    bool viewClassSessions();
    bool updateClassSession(string sessionId,string classDate,string startTime,
        string endTime,string classroom,string courseId);

    bool deleteClassSession(string sessionId);

    bool createUserAccount(
    string userId,string username,string password,string role,string fullName,
    string email,string phone,string address = "",string dateOfBirth = "",string gender = "",string departmentId = "");

    bool viewUserAccounts();
    bool updateUserAccount(string userId, string newUsername);


    bool addDepartment(string departmentId, string departmentName);
    bool viewDepartments();
    bool updateDepartment(string departmentId, string departmentName);
    bool deleteDepartment(string departmentId);

    bool assignLecturerToCourse(string courseId, string lecturerId);
    bool unassignLecturerFromCourse(string courseId);

    bool viewAttendanceHistory();
    bool viewAttendanceReports();

    bool viewClassSessionCalendar();
    bool deleteUserAccount(string userId);
};

#endif