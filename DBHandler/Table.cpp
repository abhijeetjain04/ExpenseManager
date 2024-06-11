#include "Table.h"
#include "Database_SQLite.h"
#include "Clause.h"
#include "JsonHelper/json.h"
#include "QueryGenerator.h"
#include "sqlite3.h"
#include <iostream>

BEGIN_NAMESPACE_DB

/**************************************************/
/*                  DBValue                      */
/**************************************************/

DBValue::DBValue(const std::any& value)
    : m_Value(value)
{
}

std::string DBValue::asString() const
{
    return std::any_cast<std::string>(m_Value);
}

double DBValue::asDouble() const
{
    return std::any_cast<double>(m_Value);
}

int DBValue::asInt() const
{
    return std::any_cast<int>(m_Value);
}

bool DBValue::asBool() const
{
    return std::any_cast<bool>(m_Value);
}



/**************************************************/
/*                 TableBase                      */
/**************************************************/

Table::Table(Database_SQLite& database, const std::string& tablename, const std::vector<ColumnProperty>& columnProps)
    : m_Database(database)
    , m_Name(tablename)
    , m_ColumnProperties(columnProps)
{
}


bool Table::ExecQuery(const std::string& query)
{
    return GetDatabase().ExecQuery(query);
}

double Table::SumOf(const std::string& columnName, const Condition& condition)
{
    std::string query = QueryGenerator::SumOfQuery(*this, columnName, condition);
    return GetDatabase().ExecAndGet(query).getDouble();
}

bool Table::Select(std::vector<Model>& rows, const Condition& condition, const Clause_OrderBy& orderBy)
{
    try
    {
        std::string query = QueryGenerator::SelectQuery(*this, condition, orderBy);
        std::shared_ptr<SQLite::Statement> stmt = GetDatabase().Select(query);

        int count = 0;
        while (stmt->executeStep())
        {
            int index = 0;
            Model& row = rows.emplace_back();

            int columnCount = stmt->getColumnCount();
            while (index < columnCount)
            {
                SQLite::Column column = stmt->getColumn(index);

                switch (column.getType())
                {
                case SQLITE_INTEGER:
                    row[column.getName()] = std::any(column.getInt());
                    break;
                case SQLITE_FLOAT:
                    row[column.getName()] = std::any(column.getDouble());
                    break;
                case SQLITE_TEXT:
                    row[column.getName()] = std::any(column.getString());
                    break;
                }

                ++index;
            }
        }

        return true;
    }
    catch (std::exception& ex)
    {
        printf("\nEXCEPTION: TableBase::Select - %s", ex.what());
        return false;
    }
}

bool Table::Insert(const Model& model)
{
    const std::string& query = QueryGenerator::InsertQuery(*this, model);
    return ExecQuery(query);
}

bool Table::Delete(const Condition& condition)
{
    return ExecQuery(QueryGenerator::DeleteQuery(*this, condition));
}

bool Table::CheckIfExists(const std::string& columnName, const std::string& value)
{
    if (columnName.empty())
        return false;

    std::vector<Model> rows;
    Select(rows, Condition(columnName, value, Condition::Type::EQUALS));
    return rows.size() >= 1;
}

END_NAMESPACE_DB

