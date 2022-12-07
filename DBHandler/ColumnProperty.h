#pragma once

BEGIN_NAMESPACE_DB


struct ColumnProperty
{
    enum eValueType
    {
        INTEGER,
        DOUBLE,
        TEXT,
        DATE
    };

    std::string     Name;
    eValueType      ValueType;
    bool            IsPrimaryKey;
    bool            IsNotNull;
    bool            IsUnique;
    bool            AutoIncrement;

    ColumnProperty(const std::string& name, eValueType type, bool isPrimaryKey = false, bool isNotNull = false, bool isUnique = false, bool autoIncrement = false)
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
    ColumnProperty_RowID() : ColumnProperty("row_id", eValueType::INTEGER, true, false, false, true)
    {
    }
};


END_NAMESPACE_DB