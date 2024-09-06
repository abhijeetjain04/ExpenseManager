#include "Database_SQLite.h"
#include "Table.h"
#include "SQLite_Database.h"
#include "JsonHelper/json.h"
#include "Util.h"

#include <fstream>

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

std::shared_ptr<db::Table> Database_SQLite::CreateTableFromJson(const std::filesystem::path& filepath)
{
    try
    {
        Json::Value root;

        std::ifstream inputStream(filepath);
        inputStream >> root;

        const std::string& tableName = root["name"].asString();

        const Json::Value& columnPropsJson = root["columns"];
        std::vector<ColumnProperty> columnProps;

        for (const Json::Value& colJson : columnPropsJson)
        {
            const std::string& name = colJson["name"].asString();
            const std::string& valueType = colJson["valueType"].asString();
            bool isPrimaryKey = colJson.isMember("isPrimaryKey") ? colJson["isPrimaryKey"].asBool() : false;
            bool isNotNull = colJson.isMember("isNotNull") ? colJson["isNotNull"].asBool() : false;
            bool isUnique = colJson.isMember("isUnique") ? colJson["isUnique"].asBool() : false;
            bool autoIncrement = colJson.isMember("autoIncrement") ? colJson["autoIncrement"].asBool() : false;

            columnProps.push_back(ColumnProperty(name, valueType, isPrimaryKey, isNotNull, isUnique, autoIncrement));
        }

        std::shared_ptr<db::Table> table = std::make_shared<db::Table>(*this, tableName, columnProps);
        m_Tables.insert(std::make_pair(tableName, table));
        return table;
    }
    catch (std::exception& ex)
    {
        printf("\nEXCEPTION: CreateTableFromJson: %s", ex.what());
        return nullptr;
    }

    return nullptr;
}

std::shared_ptr<db::Table> Database_SQLite::GetTable(const std::string& tableName)
{
    try
    {
        if (GetImpl()->tableExists(tableName) && m_Tables.find(tableName) != m_Tables.end())
            return m_Tables[tableName];
        else
        {
            printf("\nERROR: Table does not exist : %s", tableName.c_str());
            return nullptr;
        }
    }
    catch (std::exception& ex) 
    {
        printf("\nEXCEPTION: GetTable() - %s", ex.what());
        return nullptr;
    }
}

END_NAMESPACE_DB

