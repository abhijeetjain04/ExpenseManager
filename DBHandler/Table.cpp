#include "Table.h"
#include "Database_SQLite.h"

BEGIN_NAMESPACE_DB


TableBase::TableBase(Database_SQLite& database, const std::string& tablename)
    : m_Database(database)
    , m_Name(tablename)
{
}


bool TableBase::ExecQuery(const std::string& query)
{
    return GetDatabase().ExecQuery(query);
}


END_NAMESPACE_DB

