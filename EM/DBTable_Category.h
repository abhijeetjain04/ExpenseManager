#pragma once

#include "DBHandler/Table.h"
#include "DBHandler/QueryHandler.h"
#include "DBHandler/Database_SQLite.h"

BEGIN_NAMESPACE_EM

struct DBModel_Category
{
    int         RowID;
    std::string Name;
};


class DBInsertQueryHandler_Category : public db::IInsertQueryHandler<DBModel_Category>
{
public:
    using base = db::IInsertQueryHandler<DBModel_Category>;

    DBInsertQueryHandler_Category(db::TableBase& table)
        : base(table)
    {
    }

    std::string GenerateQuery(const DBModel_Category& model);
};


class DBSelectQueryHandler_Category : public db::ISelectQueryHandler<DBModel_Category>
{
public:
    using base = db::ISelectQueryHandler<DBModel_Category>;

    DBSelectQueryHandler_Category(db::TableBase& table)
        : base(table)
    {
    }

    bool ParseResult(const std::shared_ptr<SQLite::Statement>& stmt, std::vector<DBModel_Category>& rows) override;
};


class DBTable_Category : public db::Table   <DBModel_Category,
    DBInsertQueryHandler_Category,
    DBSelectQueryHandler_Category>
{
public:
    using base = db::Table<DBModel_Category, DBInsertQueryHandler_Category, DBSelectQueryHandler_Category>;

    REGISTER_TABLE("categories");

    DBTable_Category(db::Database_SQLite& db, const std::string& tablename)
        : base(db, tablename)
    {
    }

    virtual void InitializeColumns() override;

    DBTable_Category(const DBTable_Category& rhs) = delete;
    void operator=(const DBTable_Category& rhs) = delete;

};


END_NAMESPACE_EM