#include "Administrator.h"

Administrator::Administrator(string userId, string username,
    string passwordHash, bool active, string adminId,
    string phoneNumber, string fullName, string email)
    : User(userId, username, passwordHash, Role::ADMINISTRATOR, active)
{
    this->adminId = adminId;
    this->phoneNumber = phoneNumber;
    this->fullName = fullName;
    this->email = email;
}

Administrator::~Administrator() = default;

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
}

void Administrator::setFullName(string fullName)
{
    if (!fullName.empty())
    {
        this->fullName = fullName;
    }
}

void Administrator::setEmail(string email)
{
    this->email = email;
}