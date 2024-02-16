#include "pch.h"
#include "QueryHandler.h"

BEGIN_NAMESPACE_DB

std::string SumOfQueryHandler::GenerateQuery(const std::string& columnName, const Condition& condition)
{
    std::ostringstream oss;

    oss << "select sum(" << columnName << ") ";
    oss << "from " << GetTable().GetName();

    if (condition.IsValid())
    {
        oss << " WHERE ";
        oss << condition.ToString();
    }

    oss << ";";

    return oss.str();
}

END_NAMESPACE_DB