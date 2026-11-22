
#ifndef STUDENT_H
#define STUDENT_H

#include <string>
using namespace std;

class Student {
private:
    string studentId;
    string fullName;
    string dateOfBirth;
    string gender;
    string emailAddress;
    string phoneNumber;
    string address;

public:
    Student();

    // Keep the old constructor for compatibility.
    Student(string id, string name,
            string email, string phone);

    // Constructor with complete student information.
    Student(string id, string name,
            string birth, string studentGender,
            string email, string phone,
            string studentAddress);

    string getStudentId() const;
    string getFullName() const;
    string getDateOfBirth() const;
    string getGender() const;
    string getEmailAddress() const;
    string getPhoneNumber() const;
    string getAddress() const;

    void setFullName(string name);
    void setDateOfBirth(string birth);
    void setGender(string studentGender);
    void setEmailAddress(string email);
    void setPhoneNumber(string phone);
    void setAddress(string studentAddress);

    void displayInformation() const;
};

#endif
