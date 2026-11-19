#include "UserController.h"
#include <iostream>
#include <libpq-fe.h>

UserController::UserController(DatabaseConnection *database)
{
    this->database = database;
}

User *UserController::login(string username, string password)
{
    if (username.empty() || password.empty()){
        return nullptr;
    }
    else if (database == nullptr){
        return nullptr;
    }

    PGconn* connection = database->getConnection();

    if (connection == nullptr || PQstatus(connection) != CONNECTION_OK){
        return nullptr;
    }

    const char* query =
        "SELECT user_id, username, password, role, is_active "
        "FROM users "
        "WHERE username = $1";

    const char* parameters[1] = { username.c_str() };

    PGresult* result = PQexecParams(connection,query,1,
        nullptr,parameters,nullptr,nullptr,0);

    if (result == nullptr){
    cerr << "Login query failed!" << endl;
    return nullptr;
    }
        
    if (PQresultStatus(result) != PGRES_TUPLES_OK){
        cerr << "Login query failed: "<< PQerrorMessage(connection) << endl;
        PQclear(result);
        return nullptr;
    }

    if (PQntuples(result) != 1){
        PQclear(result);
        return nullptr;
    }

    string userId = PQgetvalue(result, 0, 0);
    while (!userId.empty() && userId.back() == ' '){
    userId.pop_back();
    }
    string storedUsername = PQgetvalue(result, 0, 1);
    string passwordHash = PQgetvalue(result, 0, 2);
    string roleString = PQgetvalue(result, 0, 3);
    bool active = (string(PQgetvalue(result, 0, 4)) == "t");

    PQclear(result);

    if (!active){
        return nullptr;
    }

    Role role;

    if (roleString == "Student"){
        role = Role::STUDENT;
    }
    else if (roleString == "Lecturer"){
        role = Role::LECTURER;
    }
    else if (roleString == "Administrator"){
        role = Role::ADMINISTRATOR;
    }
    else{
        return nullptr;
    }

    User* user = new User(userId,storedUsername,
        passwordHash,role,active);

    if (!user->verifyPassword(password)){
        delete user;
        return nullptr;
    }

    return user;
}
