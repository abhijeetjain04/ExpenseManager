#pragma once

BEGIN_NAMESPACE_DB


struct ColumnProperty
{
    std::string     Name;
    std::string     ValueType;
    bool            IsPrimaryKey;
    bool            IsNotNull;
    bool            IsUnique;
    bool            AutoIncrement;

    ColumnProperty(const std::string& name, const std::string& type, bool isPrimaryKey = false, bool isNotNull = false, bool isUnique = false, bool autoIncrement = false)
        : Name(name)
        , ValueType(type)
        , IsPrimaryKey(isPrimaryKey)
        , IsNotNull(isNotNull)
        , IsUnique(isUnique)
        , AutoIncrement(autoIncrement)
    {
    }
};

struct ColumnProperty_RowID : public ColumnProperty
{
    ColumnProperty_RowID() : ColumnProperty("row_id", "integer", true, false, false, true)
    {
    }
};


END_NAMESPACE_DB