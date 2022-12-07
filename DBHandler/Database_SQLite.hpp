#pragma once

BEGIN_NAMESPACE_DB


template<typename T>
std::shared_ptr<T> Database_SQLite::CreateTable()
{
    const std::string& tablename = T::GetName();
    std::shared_ptr<T> table = std::make_shared<T>(*this, tablename);
    table->InitializeColumns();

    try
    {
        if (GetImpl()->tableExists(tablename))
            return table;

        std::string creationQuery = CreateTableQuery(table);
        SQLITE_LOG_QUERY(creationQuery.c_str());
        GetImpl()->exec(creationQuery);
    }
    catch (SQLite::Exception& e)
    {
        SQLITE_EXCEPTION(e);
        return nullptr;
    }

    return table;
}


template<typename T>
bool Database_SQLite::DropTable()
{
    const std::string& tablename = T::GetName();

    if (!GetImpl()->tableExists(tablename))
    {
        assert(!std::format("'{}' table does not exist!", tablename.c_str()).c_str());
        return false;
    }

    GetImpl()->exec(std::format("DROP TABLE {};", tablename));
    return false;
}


END_NAMESPACE_DB
