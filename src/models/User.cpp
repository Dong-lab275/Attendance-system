#include "User.h"

User::User(char userId, string username, string passwordHash, Role role, bool active)
{
    this->userId = userId;
    this->username = username;
    this->passwordHash = passwordHash;
    this->role = role;
    this->active = active;

    this->createdAt = chrono::system_clock::now();
    this->updatedAt = this->createdAt;
}

User::~User()
{
}

bool User::changePassword(string oldPassword, string newPassword)
{
    if (!verifyPassword(oldPassword)){
        return false;
    }
    if (newPassword.empty()){
        return false;
    }
    passwordHash = newPassword;
    updatedAt = chrono::system_clock::now();
    return true;
}

bool User::verifyPassword(string rawPassword)
{
    return passwordHash == rawPassword;
}

void User::updateProfile()
{
    updatedAt = chrono::system_clock::now();
}

bool User::isActive() const
{
    return active;
}
