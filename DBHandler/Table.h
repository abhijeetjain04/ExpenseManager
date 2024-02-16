#pragma once
#include "Common.h"
#include "Condition.h"
#include "ColumnProperty.h"


#define REGISTER_TABLE(tablename)   static const std::string GetName() { return tablename; }

BEGIN_NAMESPACE_DB


class Database_SQLite;
class Clause_OrderBy;

class SumOfQueryHandler;

class TableBase
{
public:
    TableBase(Database_SQLite& database, const std::string& tablename);
    virtual ~TableBase() = default;

    const std::string& GetName() const { return m_Name; }

    bool ExecQuery(const std::string& query);

    void SetColumnProperties(const std::vector<ColumnProperty>& columns) { m_ColumnProperties = columns; }
    const std::vector<ColumnProperty>& GetColumnProperties() const { return m_ColumnProperties; }

protected:
    Database_SQLite& GetDatabase() const { return m_Database; }

protected:
    Database_SQLite&            m_Database;
    std::string                 m_Name;
    std::vector<ColumnProperty> m_ColumnProperties;
};



template<
    typename Model, 
    typename InsertExecutor, 
    typename SelectExecutor,
    typename DeleteExecutor = IDeleteQueryHandler<Model>>
class Table : public TableBase
{
public:
    // RAII
    Table(Database_SQLite& database, const std::string& tablename);
    virtual ~Table();

    Table(const Table&) = delete;
    Table& operator=(const Table&) = delete;

    virtual void InitializeColumns() = 0;

    double  SumOf(const std::string& columnName, const Condition& condition = Condition());
    bool    Insert(const Model& model);
    bool    Select(std::vector<Model>& Rows, const Condition& condition = Condition(), const Clause_OrderBy& orderBy = Clause_OrderBy());
    bool    Delete(const Condition& condition);
    bool    CheckIfExists(const std::string& columnName, const std::string& value);

private:

    InsertExecutor*     m_InsertExecutor;
    SelectExecutor*     m_SelectExecutor;
    DeleteExecutor*     m_DeleteExecutor;
    SumOfQueryHandler*  m_SumOfExecutor;
    
};


END_NAMESPACE_DB

#include "Table.hpp"


