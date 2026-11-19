#include "Administrator.h"

Administrator::Administrator(string  userId, string username, string passwordHash,
    bool active,string  adminId, string phoneNumber, string fullName, string email)  : User(userId,username,passwordHash,
        Role::ADMINISTRATOR,
        active
    )
{
    this->adminId = adminId;
    this->phoneNumber = phoneNumber;
    this->fullName = fullName;
    this->email = email;
}

Administrator::~Administrator()
{
}

bool Administrator::manageStudentAccounts(string action, Student student)
{
    return false;
}

bool Administrator::manageLecturerAccounts(string action, Lecturer lecturer)
{
    return false;
}

bool Administrator::manageCourses(string action, Course course)
{
    return false;
}

bool Administrator::manageCourseEnrollments(string action, string courseId, string studentId)
{
    return false;
}

bool Administrator::assignLecturer(string courseId, string lecturerId)
{
    return false;
}

bool Administrator::manageClassSessions(string action, ClassSession session)
{
    return false;
}

AttendanceReport Administrator::viewOverallAttendanceReport()
{
    return AttendanceReport();
}

string Administrator::getAdminId() const
{
    return adminId;
}

string Administrator::getPhoneNumber() const
{
    return phoneNumber;
}

string Administrator::getFullName() const
{
    return fullName;
}

string Administrator::getEmail() const
{
    return email;
}

void Administrator::setPhoneNumber(string phoneNumber)
{
    this->phoneNumber = phoneNumber;
    updateProfile();
}

void Administrator::setFullName(string fullName)
{
     if (!fullName.empty()){
        this->fullName = fullName;
        updateProfile();
    }
}

void Administrator::setEmail(string email)
{
    this->email = email;
    updateProfile();
}
