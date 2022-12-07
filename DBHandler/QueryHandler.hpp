#pragma once

#include "Clause.h"

BEGIN_NAMESPACE_DB

template<typename Model>
template<typename ConditionType>
std::string ISelectQueryHandler<Model>::GenerateQuery(const ConditionType& cond, const Clause_OrderBy& orderBy)
{
    std::ostringstream oss;
    oss << "SELECT * from " << GetTable().GetName();

    if (cond.IsValid())
    {
        oss << " WHERE ";
        oss << cond.ToString();
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

template<typename Model>
std::string IInsertQueryHandler<Model>::GenerateQueryTemplate()
{
    std::ostringstream oss;
    oss << "INSERT INTO " << GetTable().GetName();
    oss << " ( ";

    const auto& columns = GetTable().GetColumnProperties();
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
        oss << "'{}'";
        if (i != numColumns - 1)
            oss << ", ";
    }

    oss << " );";

    return oss.str();
}


template<typename Model>
std::string IDeleteQueryHandler<Model>::GenerateQuery(
    const Condition& condition)
{
    std::ostringstream oss;

    oss << "DELETE FROM " << GetTable().GetName() << " ";
    oss << "WHERE " << condition.ToString();
    oss << ";";

    return oss.str();
}


END_NAMESPACE_DB
