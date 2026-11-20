#include "User.h"
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

User::User(string userId, string username, string passwordHash,
           Role role, bool active)
{
    this->userId = userId;
    this->username = username;
    this->passwordHash = passwordHash;
    this->role = role;
    this->active = active;

    this->createdAt = chrono::system_clock::now();
    this->updatedAt = this->createdAt;
}

User::~User() = default;

bool User::changePassword(string oldPassword, string newPassword)
{
    if (!verifyPassword(oldPassword))
        return false;

    if (newPassword.empty())
        return false;

    passwordHash = hashPassword(newPassword);
    updatedAt = chrono::system_clock::now();

    return true;
}

bool User::verifyPassword(string rawPassword)
{
    return passwordHash == hashPassword(rawPassword);
}

void User::updateProfile()
{
    updatedAt = chrono::system_clock::now();
}

bool User::isActive() const
{
    return active;
}

string User::hashPassword(string rawPassword)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];

    SHA256(
        reinterpret_cast<const unsigned char*>(rawPassword.c_str()),
        rawPassword.length(),
        hash
    );

    stringstream ss;

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }

    return ss.str();
}

string User::getUserId() const
{
    return userId;
}

string User::getUsername() const
{
    return username;
}

Role User::getRole() const
{
    return role;
}

void User::setUsername(string username)
{
    if (!username.empty())
    {
        this->username = username;
        updateProfile();
    }
}

void User::setActive(bool active)
{
    this->active = active;
    updateProfile();
}