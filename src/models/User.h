
#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <chrono>

using namespace std;

enum class Role
{
    STUDENT,
    LECTURER,
    ADMINISTRATOR
};

using DateTime = chrono::system_clock::time_point;

class User{
protected:
    string userId;
    string username;
    string passwordHash;
    Role role;
    bool active;

    DateTime createdAt;
    DateTime updatedAt;

    string hashPassword(string rawPassword);

public:
    User(string userId,string username,string passwordHash,Role role,bool active);

    virtual ~User();

    bool changePassword(string oldPassword, string newPassword);
    bool verifyPassword(string rawPassword);
    void updateProfile();
    bool isActive() const;

    string getUserId() const;
    string getUsername() const;
    Role getRole() const;
    string getPasswordHash() const;

    void setUsername(string username);
    void setActive(bool active);
};

#endif
