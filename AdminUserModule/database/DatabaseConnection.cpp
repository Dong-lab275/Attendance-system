
#include "DatabaseConnection.h"
#include <iostream>
#include <cstdlib>

using namespace std;

DatabaseConnection::DatabaseConnection()
{
    connection = nullptr;
}

DatabaseConnection::~DatabaseConnection()
{
    closeDatabase();
}

bool DatabaseConnection::connectDatabase()
{
    if (connection != nullptr) {
        return PQstatus(connection) == CONNECTION_OK;
    }

    const char* password = getenv("DB_PASSWORD");

    if (password == nullptr || password[0] == '\0') {
        cerr << "Database password is not configured!" << endl;
        return false;
    }

    const char* keywords[] = {
        "host", "port", "dbname", "user", "password", nullptr
    };

    const char* values[] = {
        "127.0.0.1",
        "5433",
        "attendance_system",
        "attendance_admin",
        password,
        nullptr
    };

    connection = PQconnectdbParams(keywords, values, 0);

    if (connection == nullptr || PQstatus(connection) != CONNECTION_OK) {
        cerr << "Database connection failed: "
             << (connection != nullptr ? PQerrorMessage(connection) : "Unable to allocate connection")
             << endl;

        if (connection != nullptr) {
            PQfinish(connection);
            connection = nullptr;
        }

        return false;
    }

    cout << "Database connected successfully!" << endl;
    return true;
}

void DatabaseConnection::closeDatabase()
{
    if (connection != nullptr){
        PQfinish(connection);
        connection = nullptr;
    }
}

PGconn* DatabaseConnection::getConnection() const
{
    return connection;
}
