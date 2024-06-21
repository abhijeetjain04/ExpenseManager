#include "pch.h"
#include "QueryGenerator.h"
#include "SQLite_Database.h"
#include "Table.h"

BEGIN_NAMESPACE_DB

std::string QueryGenerator::InsertQuery(const Table& table, const Model& model)
{
    std::ostringstream oss;
    oss << "INSERT INTO " << table.GetName();
    oss << " ( ";

    const std::vector<ColumnProperty>& columns = table.GetColumnProperties();
    size_t numColumns = columns.size();

    bool hasRowID = columns[0].Name == "row_id";
    size_t i = hasRowID ? 1 : 0;
    for (; i < columns.size(); ++i)
    {
        const auto& column = columns[i];

        oss << column.Name;
        if (i != numColumns - 1)
            oss << ", ";
    }

    oss << " ) VALUES ( ";

    i = hasRowID ? 1 : 0;
    for (; i < numColumns; ++i)
    {
        const ColumnProperty& colProp = columns[i];
        oss << "'";
        if (model.contains(colProp.Name)) // if the column value is not provided, enter empty value.
            oss << model.at(colProp.Name);
        oss << "'";
        if (i != numColumns - 1)
            oss << ", ";
    }

    oss << " );";

    return oss.str();
}

std::string QueryGenerator::SelectQuery(const Table& table, const Condition& condition, const Clause_OrderBy& orderBy)
{
    std::ostringstream oss;
    oss << "SELECT * from " << table.GetName();

    if (condition.IsValid())
    {
        oss << " WHERE ";
        oss << condition.ToString();
    }
    if (orderBy.IsValid())
    {
        oss << " ORDER BY ";
        oss << orderBy.GetColumnName();

        if (orderBy.IsDescending())
            oss << " DESC";
    }

    oss << ";";

    return oss.str();
}

std::string QueryGenerator::DeleteQuery(const Table& table, const Condition& condition)
{
    std::ostringstream oss;

    oss << "DELETE FROM " << table.GetName() << " ";
    oss << "WHERE " << condition.ToString();
    oss << ";";

    return oss.str();

}

std::string QueryGenerator::SumOfQuery(const Table& table, const std::string& columnName, const Condition& condition)
{
    std::ostringstream oss;

    oss << "select sum(" << columnName << ") ";
    oss << "from " << table.GetName();

    if (condition.IsValid())
    {
        oss << " WHERE ";
        oss << condition.ToString();
    }

    oss << ";";

    return oss.str();

}

END_NAMESPACE_DB
