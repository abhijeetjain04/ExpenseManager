#pragma once

#include "Common.h"

BEGIN_NAMESPACE_DB

class Clause_OrderBy
{
public:

    enum Type
    {
        ASCENDING,
        DESCENDING
    };

    Clause_OrderBy() : Clause_OrderBy("") {}
    Clause_OrderBy(const std::string& columnName, Type type = Type::ASCENDING);

    void SetType(Type type) { m_Type = type; }
    bool IsDescending() const;
    bool IsValid() const;

    const std::string& GetColumnName() const { return m_ColumnName; }

private:
    std::string m_ColumnName;
    Type        m_Type;
};

END_NAMESPACE_DB

