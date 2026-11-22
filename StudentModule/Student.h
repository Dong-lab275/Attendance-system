
#ifndef STUDENT_H
#define STUDENT_H

#include <string>
using namespace std;

class Student {
private:
    string studentId;
    string fullName;
    string emailAddress;
    string phoneNumber;

public:
    Student();

    Student(string id, string name,
            string email, string phone);

    string getStudentId() const;
    string getFullName() const;
    string getEmailAddress() const;
    string getPhoneNumber() const;

    void setFullName(string name);
    void setEmailAddress(string email);
    void setPhoneNumber(string phone);

    void displayInformation() const;
};

#endif

