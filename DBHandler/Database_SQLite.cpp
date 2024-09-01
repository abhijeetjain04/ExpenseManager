#include "Database_SQLite.h"
#include "Table.h"
#include "SQLite_Database.h"
#include "JsonHelper/json.h"
#include "Util.h"
#include "Logger/Logger.h"

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


// private
std::string Database_SQLite::CreateTableQuery(const std::string& tableName, const std::vector<ColumnProperty>& columns)
{
    auto getValueTypeString = [](std::string type)
    {
        util::string::ToLower(type);
        if (type == "integer")
            return "INTEGER";

        if (type == "double")
            return "REAL";

        if (type == "text")
            return "TEXT";

        if (type == "date") // dates are stored as text in SQLITE
            return "TEXT";

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
    oss << "CREATE TABLE IF NOT EXISTS " << tableName << "(";

    for(size_t i=0; i<columns.size(); ++i)
    {
        const auto& prop = columns[i];
        std::string propStr = formatColumn(prop);
        oss << propStr;
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

        std::string query = CreateTableQuery(tableName, columnProps);
        GetImpl()->exec(query);

        std::shared_ptr<db::Table> table = std::make_shared<db::Table>(*this, tableName, columnProps);
        m_Tables.insert(std::make_pair(tableName, table));
        return table;
    }
    catch (std::exception& ex)
    {
        logger::Error("\nEXCEPTION: CreateTableFromJson: %s", ex.what());
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
            logger::Error("\nERROR: Table does not exist : %s", tableName.c_str());
            return nullptr;
        }
    }
    catch (std::exception& ex) 
    {
        logger::Error("\nEXCEPTION: GetTable() - %s", ex.what());
        return nullptr;
    }
}

END_NAMESPACE_DB

