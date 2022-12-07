#pragma once

#include "Common.h"
#include "SQLite_Database.h"

BEGIN_NAMESPACE_DB

const int OK = SQLite::OK;
const int OPEN_READONLY = SQLite::OPEN_READONLY;
const int OPEN_READWRITE = SQLite::OPEN_READWRITE;
const int OPEN_CREATE = SQLite::OPEN_CREATE;

struct ColumnProperty;
class TableBase;
typedef std::shared_ptr<TableBase> TableSPtr;


class Database_SQLite
{
public:
    Database_SQLite(const char* name, int openMode = OPEN_CREATE | OPEN_READWRITE);
    Database_SQLite(const std::string& name, int openMode = OPEN_CREATE | OPEN_READWRITE);

    Database_SQLite(const Database_SQLite&) = delete;
    Database_SQLite& operator=(const Database_SQLite&) = delete;

    virtual ~Database_SQLite();

    void operator delete(void* ptr, size_t size)
    {
        ::delete ptr;
        ptr = nullptr;
    }

    template<typename T>
    std::shared_ptr<T> CreateTable();
    template<typename T>
    bool DropTable();

    // queries
    SQLite::Column ExecAndGet(const std::string& query);

    bool ExecQuery(const std::string& query);
    std::shared_ptr<SQLite::Statement> Select(const std::string& query);

    const std::string GetName() const { return m_DBImpl->getFilename(); }
    int GetOpenMode() const { return m_OpenMode; }

private:
    std::string CreateTableQuery(const std::shared_ptr<TableBase> table);

    SQLite::Database* GetImpl() { return m_DBImpl; }

private:
    int                                             m_OpenMode;
    SQLite::Database*                               m_DBImpl;
};


END_NAMESPACE_DB

#include "Database_SQLite.hpp"
