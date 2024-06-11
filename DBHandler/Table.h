#pragma once
#include "Common.h"
#include "Condition.h"
#include "ColumnProperty.h"
#include "Clause.h"

#include <format>
#include <any>


#define REGISTER_TABLE(tablename)   static const std::string GetName() { return tablename; }

BEGIN_NAMESPACE_DB


class Database_SQLite;
class Clause_OrderBy;

struct DBValue
{
    DBValue() = default;
    DBValue(const std::any& value);

    std::string asString() const;
    double asDouble() const;
    int asInt() const;
    bool asBool() const;

    operator std::string()
    {
        return asString();
    }

    template<typename ValueType>
    void operator= (const ValueType& value)
    {
        m_Value = std::any(value);
    }

    bool IsEmpty() const
    {
        return !m_Value.has_value();
    }

    friend std::ostringstream& operator<< (std::ostringstream& oss, const DBValue& value)
    {
        if (value.m_Value.type() == typeid(int))
            oss << value.asInt();
        else if (value.m_Value.type() == typeid(double))
            oss << value.asDouble();
        else if (value.m_Value.type() == typeid(std::string))
            oss << value.asString();
        else if (value.m_Value.type() == typeid(bool))
            oss << value.asBool();

        return oss;
    }

private:
    std::any m_Value;
};

class Model
{
public:
    DBValue& operator[](const std::string& name)
    {
        return m_Model[name];
    }

    const DBValue& operator[](const std::string& name) const
    {
        return m_Model.at(name);
    }

private:
    std::unordered_map<std::string, DBValue> m_Model;
};

class Table
{
public:
    Table(Database_SQLite& database, const std::string& tablename, const std::vector<ColumnProperty>& columnProps);
    virtual ~Table() = default;

    const std::string& GetName() const { return m_Name; }

    bool ExecQuery(const std::string& query);

    double SumOf(const std::string& columnName, const Condition& condition = Condition());
    bool CheckIfExists(const std::string& columnName, const std::string& value);
    bool Select(std::vector<Model>& rows, const Condition& condition = Condition(), const Clause_OrderBy& orderBy = Clause_OrderBy());
    bool Insert(const Model& model);
    bool Delete(const Condition& condition);

    void SetColumnProperties(const std::vector<ColumnProperty>& columns) { m_ColumnProperties = columns; }
    const std::vector<ColumnProperty>& GetColumnProperties() const { return m_ColumnProperties; }

protected:
    Database_SQLite& GetDatabase() const { return m_Database; }

protected:
    Database_SQLite&            m_Database;
    std::string                 m_Name;
    std::vector<ColumnProperty> m_ColumnProperties;
};

END_NAMESPACE_DB


