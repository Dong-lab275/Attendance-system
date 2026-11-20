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
    bool addClassSession(string sessionId,string classDate,string startTime,
    string endTime,string classroom,string courseId);

    bool viewClassSessions();
    bool updateClassSession(string sessionId,string classDate,string startTime,
        string endTime,string classroom,string courseId);

    bool deleteClassSession(string sessionId);
};

#endif