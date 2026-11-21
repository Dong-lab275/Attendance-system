#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <libpq-fe.h>
#include <string>

using namespace std;

class DatabaseConnection
{
private:
    PGconn* connection;

public:
    DatabaseConnection();
    ~DatabaseConnection();

    bool connectDatabase();
    void closeDatabase();
    PGconn* getConnection() const;
};

#endif