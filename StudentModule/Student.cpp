
#include "Student.h"
#include <iostream>

using namespace std;

Student::Student() {
    studentId = "";
    fullName = "";
    emailAddress = "";
    phoneNumber = "";
}

Student::Student(string studentId, string fullName,
                 string emailAddress, string phoneNumber) {
    this->studentId = studentId;
    this->fullName = fullName;
    this->emailAddress = emailAddress;
    this->phoneNumber = phoneNumber;
}

string Student::getStudentId() const {
    return studentId;
}

string Student::getFullName() const {
    return fullName;
}

string Student::getEmailAddress() const {
    return emailAddress;
}

string Student::getPhoneNumber() const {
    return phoneNumber;
}

void Student::setFullName(string fullName) {
    this->fullName = fullName;
}

void Student::setEmailAddress(string emailAddress) {
    this->emailAddress = emailAddress;
}

void Student::setPhoneNumber(string phoneNumber) {
    this->phoneNumber = phoneNumber;
}

void Student::displayInformation() const {
    cout << "Student ID: " << studentId << endl;
    cout << "Full Name: " << fullName << endl;
    cout << "Email: " << emailAddress << endl;
    cout << "Phone: " << phoneNumber << endl;
}
