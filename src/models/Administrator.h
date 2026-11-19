
#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include <iostream>
#include <string>

#include "User.h"

using namespace std;

class Student;
class Lecturer;
class Course;
class ClassSession;
class AttendanceReport;

class Administrator : public User
{
private:
    string adminId;
    string phoneNumber;
    string fullName;
    string email;

public:

    Administrator(string userId, string username, string passwordHash, 
        bool active, string  adminId, string phoneNumber, string fullName, string email);

    ~Administrator() override;

    string getAdminId() const;
    string getPhoneNumber() const;
    string getFullName() const;
    string getEmail() const;


    void setPhoneNumber(string phoneNumber);
    void setFullName(string fullName);
    void setEmail(string email);

    bool manageStudentAccounts(string action, Student student);
    bool manageLecturerAccounts(string action, Lecturer lecturer);
    bool manageCourses(string action, Course course);
    bool manageCourseEnrollments(string action,string  courseId,string  studentId);
    bool assignLecturer(string  courseId,string  lecturerId);
    bool manageClassSessions(string action,ClassSession session);

    AttendanceReport viewOverallAttendanceReport();
};

#endif
