#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include <string>
#include "User.h"

using namespace std;

class Administrator : public User
{
private:
    string adminId;
    string phoneNumber;
    string fullName;
    string email;

public:
    Administrator(string userId, string username, string passwordHash,
        bool active, string adminId, string phoneNumber,
        string fullName, string email);

    ~Administrator() override;

    string getAdminId() const;
    string getPhoneNumber() const;
    string getFullName() const;
    string getEmail() const;

    void setPhoneNumber(string phoneNumber);
    void setFullName(string fullName);
    void setEmail(string email);
};

#endif