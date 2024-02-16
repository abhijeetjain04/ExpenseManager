#pragma once

class Condition;


BEGIN_NAMESPACE_DB

#ifdef template_args
    #undef template_args
#endif
#define template_args typename Model, typename InsertExecutor, typename SelectExecutor, typename DeleteExecutor


template<template_args>
Table<template_args>::Table(
    Database_SQLite& database,
    const std::string& tablename)
    : TableBase(database, tablename)
{
    m_InsertExecutor    = new InsertExecutor(*this);
    m_SelectExecutor    = new SelectExecutor(*this);
    m_DeleteExecutor    = new DeleteExecutor(*this);
    m_SumOfExecutor     = new SumOfQueryHandler(*this);
}


template<template_args>
Table<template_args>::~Table()
{
    if (m_InsertExecutor)
        delete m_InsertExecutor;
    if (m_SelectExecutor)
        delete m_SelectExecutor;
    if (m_DeleteExecutor)
        delete m_DeleteExecutor;
    if (m_SumOfExecutor)
        delete m_SumOfExecutor;
}


template<template_args>
bool Table<template_args>::Insert(
    const Model& model)
{
    std::string query = m_InsertExecutor->GenerateQuery(model);
    return ExecQuery(query);
}

template<template_args>
bool Table<template_args>::Select(
    std::vector<Model>& rows, 
    const Condition& condition,
    const Clause_OrderBy& orderBy)
{
    std::string query = m_SelectExecutor->GenerateQuery(condition, orderBy);
    SQLITE_LOG_QUERY(query.c_str());
    auto stmt = GetDatabase().Select(query);
    return m_SelectExecutor->ParseResult(stmt, rows);
}

template<template_args>
bool Table<template_args>::CheckIfExists(const std::string& columnName, const std::string& value)
{
    if (columnName.empty())
        return false;

    std::vector<Model> rows;
    Select(rows, Condition(columnName, value, Condition::Type::EQUALS));
    return rows.size() >= 1;
}


template<template_args>
bool Table<template_args>::Delete(const Condition& condition)
{
    std::string query = m_DeleteExecutor->GenerateQuery(condition);
    return ExecQuery(query);
}

template<template_args>
double Table<template_args>::SumOf(const std::string& columnName, const Condition& condition)
{
    std::string query = m_SumOfExecutor->GenerateQuery(columnName, condition);
    return GetDatabase().ExecAndGet(query).getDouble();
}

END_NAMESPACE_DB
