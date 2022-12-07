#pragma once

#include "Common.h"
#include "SQLite_Database.h"
#include "Table.h"


BEGIN_NAMESPACE_DB

class Condition;
class TableBase;
class Clause_OrderBy;

class QueryHandler
{
public:
    QueryHandler(TableBase& table)
        : m_Table(table)
    {}

    TableBase& GetTable() const { return m_Table; }

private:
    TableBase& m_Table;
};

/// <summary>
/// 
/// For every Database Insertion in a table to be handled, inherit from IInsertQueryHandler abstaract class.
/// Get the unformatted query by calling GenerateQueryTemplate, and format the string with the appropriate DBTable values.
/// 
/// </summary>
/// <typeparam name="Model"></typeparam>
template<typename Model>
class IInsertQueryHandler : public QueryHandler
{
public:
    using base = db::QueryHandler;

    IInsertQueryHandler(db::TableBase& table)
        : base(table)
    {
    }

    // Needs to be implemented to handle the formatting for a given DB Model.
    virtual std::string GenerateQuery(const Model& model) = 0;

    // Generates the insertion query template, which should be formatted by the overridden GenerateQuery(Model&) function.
    std::string GenerateQueryTemplate();

};

/// <summary>
/// 
/// Interface to handle the Selection of rows from a Database Table.
/// 
/// </summary>
/// <typeparam name="Model"></typeparam>
template<typename Model>
class ISelectQueryHandler : public QueryHandler
{
public:
    using base = QueryHandler;

    ISelectQueryHandler(TableBase& table)
        : base(table)
    {}

    /// <summary>
    /// 
    /// Must be overridden by the SelectQueryHandler of the DBTable.
    /// 
    /// </summary>
    /// <param name="stmt"> [in]    Object containing the result of the 'select' query (from SQLiteCpp library github) </param>
    /// <param name="rows"> [out]   Should be filled with data from stmt for the Model. </param>
    /// <returns></returns>
    virtual bool ParseResult(const std::shared_ptr<SQLite::Statement>& stmt, std::vector<Model>& rows) = 0;

    /// <summary>
    /// 
    /// Generates the query for selecting rows from the DBTable.
    /// Takes into consideration Condition/ConditionGroup and Order by clause while creating the query.
    /// 
    /// </summary>
    /// <typeparam name="ConditionType">    [in]    Condition/ConditionGroup </typeparam>
    /// <param name="cond">                 [in]    Condition/ConditionGroup </param>
    /// <param name="orderBy">              [in]    OrderBy Clause </param>
    /// <returns>   Select Query    </returns>
    template<typename ConditionType>
    std::string GenerateQuery(const ConditionType& cond, const Clause_OrderBy& orderBy = Clause_OrderBy());
};

/// <summary>
/// 
/// Interface to handle deletion of a row from a DBTable.
/// 
/// </summary>
/// <typeparam name="Model"></typeparam>
template<typename Model>
class IDeleteQueryHandler : public QueryHandler
{
public:
    using base = QueryHandler;

    IDeleteQueryHandler(TableBase& table)
        : base(table)
    {}

    std::string GenerateQuery(const Condition& condition);

};


class SumOfQueryHandler : public QueryHandler
{
public:
    using base = QueryHandler;

    SumOfQueryHandler(TableBase& table)
        :base(table)
    {
    }

    std::string GenerateQuery(const std::string& columnName, const Condition& condition = Condition());
    std::string GenerateQuery(const std::string& columnName, const ConditionGroup& condition = ConditionGroup());
};


END_NAMESPACE_DB


#include "QueryHandler.hpp"
