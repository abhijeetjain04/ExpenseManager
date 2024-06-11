#pragma once

#include "Common.h"


BEGIN_NAMESPACE_DB

class Condition;
class Table;
class Clause_OrderBy;
class Model;

class QueryGenerator
{
public:

    static std::string InsertQuery(const Table& table, const Model& model);
    static std::string SelectQuery(const Table& table, const Condition& condition, const Clause_OrderBy& orderBy);
    static std::string SumOfQuery(const Table& table, const std::string& columnName, const Condition& condition);
    static std::string DeleteQuery(const Table& table, const Condition& condition);
};

END_NAMESPACE_DB