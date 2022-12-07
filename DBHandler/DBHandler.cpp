#include "pch.h"
#include "DBHandler.h"

#include <iostream>
#include "SQLite_SQLiteCpp.h"
#include "sqlite3.h"


#define DB_FILE_NAME "testDB.db"

class Database : public SQLite::Database
{
public:
    Database(const char* dbName)
        : SQLite::Database(dbName, SQLite::OPEN_READONLY)
    {
    }
};


class DBTable
{
public:

    DBTable(Database& database, const char* tablename)
        : m_Database(database)
        , m_Name(tablename)
    {
    }

    bool Exists() const
    {
        return m_Database.tableExists(m_Name);
    }

private:
    Database& m_Database;
    std::string m_Name;
};

class CreateTable
{
public:

    CreateTable(const char* tableName)
        : m_TableName(tableName)
    {
    }

    std::string ToString() const
    {
        return std::format("CREATE TABLE IF NOT EXIST {}", m_TableName);
    }

private:
    std::string m_TableName;
};

#if 0
void DBHandler::Print()
{
    Database db(DB_FILE_NAME);

    DBTable table(db, "test");

    try
    {
        SQLite::Database db(DB_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        db.exec("DROP TABLE IF EXISTS test");
        db.exec("CREATE TABLE test (id INTEGER PRIMARY KEY, value TEXT)");

        // first row
        int nb = db.exec("INSERT INTO test VALUES (NULL, \"test\")");
        std::cout << "INSERT INTO test VALUES (NULL, \"test\")\", returned " << nb << std::endl;

        // second row
        nb = db.exec("INSERT INTO test VALUES (NULL, \"second\")");
        std::cout << "INSERT INTO test VALUES (NULL, \"second\")\", returned " << nb << std::endl;

        // update the second row
        nb = db.exec("UPDATE test SET value=\"second-updated\" WHERE id='2'");
        std::cout << "UPDATE test SET value=\"second-updated\" WHERE id='2', returned " << nb << std::endl;

        nb = db.getTotalChanges();
        std::cout << "Nb of total changes since connection: " << nb << std::endl;

        // Check the results : expect two row of result
        SQLite::Statement   query(db, "SELECT * FROM test");
        std::cout << "SELECT * FROM test :\n";
        while (query.executeStep())
        {
            std::cout << "row (" << query.getColumn(0) << ", \"" << query.getColumn(1) << "\")\n";
        }

        db.exec("DROP TABLE test");

    }
    catch(std::exception& e)
    {
        printf("\n%s", e.what());
    }
}
#endif