
#include "StudentController.h"
#include <iostream>

using namespace std;

StudentController::StudentController() {
}

void StudentController::setStudentInformation(
    string studentId,
    string fullName,
    string emailAddress,
    string phoneNumber
) {
    student = Student(
        studentId,
        fullName,
        emailAddress,
        phoneNumber
    );
}

void StudentController::updateStudentInformation(
    string fullName,
    string emailAddress,
    string phoneNumber
) {
    student.setFullName(fullName);
    student.setEmailAddress(emailAddress);
    student.setPhoneNumber(phoneNumber);
}

void StudentController::displayStudentInformation() const {
    student.displayInformation();
}

void StudentController::registerCourse(string courseId) {
    if (courseId == "C001" || courseId == "C002") {
        cout << "Course selected: " << courseId << endl;
        cout << "Registration demo successful!" << endl;
    }
    else {
        cout << "Invalid course ID!" << endl;
    }
}
