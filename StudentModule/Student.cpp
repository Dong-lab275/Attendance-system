
#include "Student.h"
#include <iostream>

using namespace std;

Student::Student() {
    studentId = "";
    fullName = "";
    dateOfBirth = "";
    gender = "";
    emailAddress = "";
    phoneNumber = "";
    address = "";
}

// Old constructor
Student::Student(string id, string name,
                 string email, string phone) {
    studentId = id;
    fullName = name;
    dateOfBirth = "";
    gender = "";
    emailAddress = email;
    phoneNumber = phone;
    address = "";
}

// Complete constructor
Student::Student(string id, string name,
                 string birth, string studentGender,
                 string email, string phone,
                 string studentAddress) {
    studentId = id;
    fullName = name;
    dateOfBirth = birth;
    gender = studentGender;
    emailAddress = email;
    phoneNumber = phone;
    address = studentAddress;
}

string Student::getStudentId() const {
    return studentId;
}

string Student::getFullName() const {
    return fullName;
}

string Student::getDateOfBirth() const {
    return dateOfBirth;
}

string Student::getGender() const {
    return gender;
}

string Student::getEmailAddress() const {
    return emailAddress;
}

string Student::getPhoneNumber() const {
    return phoneNumber;
}

string Student::getAddress() const {
    return address;
}

void Student::setFullName(string name) {
    fullName = name;
}

void Student::setDateOfBirth(string birth) {
    dateOfBirth = birth;
}

void Student::setGender(string studentGender) {
    gender = studentGender;
}

void Student::setEmailAddress(string email) {
    emailAddress = email;
}

void Student::setPhoneNumber(string phone) {
    phoneNumber = phone;
}

void Student::setAddress(string studentAddress) {
    address = studentAddress;
}

void Student::displayInformation() const {
    cout << "Student ID: " << studentId << endl;
    cout << "Full Name: " << fullName << endl;
    cout << "Date of Birth: " << dateOfBirth << endl;
    cout << "Gender: " << gender << endl;
    cout << "Email: " << emailAddress << endl;
    cout << "Phone: " << phoneNumber << endl;
    cout << "Address: " << address << endl;
}
