#include "Database_SQLite.h"
#include "Table.h"
#include "SQLite_Database.h"


#define COLUMN_NAME(prop)   prop.Name
#define COLUMN_VALUETYPE(prop)  switch(prop.ValueType) \
                                { \
                                }

BEGIN_NAMESPACE_DB


Database_SQLite::Database_SQLite(
    const char* name, 
    int openMode)
    : m_OpenMode(openMode)
{
    m_DBImpl = new SQLite::Database(name, openMode);
}


Database_SQLite::Database_SQLite(
    const std::string& name, 
    int openMode)
    : Database_SQLite(name.c_str(), openMode)
{
}

Database_SQLite::~Database_SQLite()
{
    if (GetImpl())
    {
        delete GetImpl();
        m_DBImpl = nullptr;
    }
}


// private
std::string Database_SQLite::CreateTableQuery(std::shared_ptr<TableBase> table)
{

    const std::string& tablename = table->GetName();
    const std::vector<ColumnProperty>& columns = table->GetColumnProperties();

    auto getValueTypeString = [](ColumnProperty::eValueType type)
    {
        switch (type)
        {
        case ColumnProperty::INTEGER:
            return "INTEGER";
        case ColumnProperty::DOUBLE:
            return "REAL";
        case ColumnProperty::TEXT:
            return "TEXT";
        case ColumnProperty::DATE: // dates are stored as text in SQLITE
            return "TEXT";
        }

        assert(false);
        return "";
    };

    auto formatColumn = [&](const ColumnProperty& prop)
    {
        return std::format("{} {} {} {} {} {}",
            prop.Name,
            getValueTypeString(prop.ValueType),
            prop.IsPrimaryKey ? "PRIMARY KEY" : "", 
            prop.IsNotNull ? "NOT NULL" : "", 
            prop.IsUnique ? "UNIQUE" : "",
            prop.AutoIncrement ? "AUTOINCREMENT" : "");
    };

    std::ostringstream oss;
    oss << "CREATE TABLE IF NOT EXISTS " << tablename << "(";

    for(size_t i=0; i<columns.size(); ++i)
    {
        const auto& prop = columns[i];
        oss << formatColumn(prop);
        if (i != columns.size() - 1)
            oss << ", ";
    }

    oss << ");";

    return oss.str();
}

// 
bool Database_SQLite::ExecQuery(const std::string& query)
{
    try 
    {
        SQLITE_LOG_QUERY(query.c_str());
        int numChanges = GetImpl()->exec(query);
        return numChanges > 0;
    }
    catch (SQLite::Exception& e)
    {
        SQLITE_EXCEPTION(e);
    }

    return false;
}


std::shared_ptr<SQLite::Statement> Database_SQLite::Select(const std::string& query)
{
    try 
    {
        SQLITE_LOG_QUERY(query.c_str());
        return std::make_shared<SQLite::Statement>(*GetImpl(), query);
    }
    catch (SQLite::Exception& e) 
    {
        SQLITE_EXCEPTION(e);
    }

    return nullptr;
}

SQLite::Column Database_SQLite::ExecAndGet(const std::string& query)
{
    return GetImpl()->execAndGet(query);
}

END_NAMESPACE_DB

