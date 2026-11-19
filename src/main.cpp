#include <iostream>

#include "database/DatabaseConnection.h"
#include "controllers/UserController.h"
#include "controllers/AdministratorController.h"

using namespace std;

int main()
{
    cout << "Program started!" << endl;
    // 1. Connect to PostgreSQL
    DatabaseConnection database;
   

    if (!database.connectDatabase())
    {
        cout << "Ket noi PostgreSQL that bai!" << endl;
        return 1;
    }

    // 2. Login as Administrator
    UserController userController(&database);

    User* user = userController.login("admin01", "123456");

    if (user == nullptr)
    {
        cout << "Administrator login failed!" << endl;
        return 1;
    }

    if (user->getRole() != Role::ADMINISTRATOR)
    {
        cout << "This account is not an Administrator!" << endl;
        delete user;
        return 1;
    }

    cout << "Administrator login successful!" << endl;

    // 3. Test Add Course Enrollment
    AdministratorController adminController(&database);

    bool result = adminController.addCourseEnrollment(
        "C0003",
        "079204005678"
    );

    if (result)
    {
        cout << "TEST PASSED: Add Course Enrollment" << endl;
    }
    else
    {
        cout << "TEST FAILED: Add Course Enrollment" << endl;
    }

    // 4. Release memory
    delete user;

    return 0;
}