
#include <iostream>

#include "../database/DatabaseConnection.h"
#include "../controllers/UserController.h"

using namespace std;

int main()
{
    DatabaseConnection database;

    if (!database.connectDatabase())
    {
        cerr << "Cannot connect to database!" << endl;
        return 1;
    }

    UserController userController(&database);

    User* user = userController.login("student01", "123456");

    if (user == nullptr)
    {
        cout << "Login failed!" << endl;
        return 1;
    }

    cout << "Login successful!" << endl;
    cout << "Username: " << user->getUsername() << endl;

    if (user->getRole() == Role::STUDENT)
    {
        cout << "Role: Student" << endl;
    }
    else
    {
        cout << "Unexpected role!" << endl;
    }

    delete user;

    return 0;
}
