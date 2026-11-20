
#ifndef STUDENTCONTROLLER_H
#define STUDENTCONTROLLER_H

#include "Student.h"
#include <string>

using namespace std;

class StudentController {
private:
    Student student;

public:
    StudentController();

    void setStudentInformation(
        string studentId,
        string fullName,
        string emailAddress,
        string phoneNumber
    );

    void updateStudentInformation(
        string fullName,
        string emailAddress,
        string phoneNumber
    );

    void displayStudentInformation() const;
    void registerCourse(string courseId);

bool canCheckIn(string sessionStatus) const;

};

#endif
