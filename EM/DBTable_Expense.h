#pragma once

#include "DBHandler/Table.h"
#include "DBHandler/QueryHandler.h"
#include "DBHandler/Database_SQLite.h"


struct DBModel_Expense
{
    int         RowID;
    std::string Name;
    std::string Category;
    double      Price;
    std::string Date;
};


class DBInsertQueryHandler_Expense : public db::IInsertQueryHandler<DBModel_Expense>
{
public:
    using base = db::IInsertQueryHandler<DBModel_Expense>;

    DBInsertQueryHandler_Expense(db::TableBase& table)
        : base(table)
    {
    }
    
    std::string GenerateQuery(const DBModel_Expense& model);
};


class DBSelectQueryHandler_Expense : public db::ISelectQueryHandler<DBModel_Expense>
{
public:
    using base = db::ISelectQueryHandler<DBModel_Expense>;

    DBSelectQueryHandler_Expense(db::TableBase& table)
        : base(table)
    {
    }

    bool ParseResult(const std::shared_ptr<SQLite::Statement>& stmt, std::vector<DBModel_Expense>& rows) override;
};


class DBTable_Expense : public db::Table   <DBModel_Expense, 
                                            DBInsertQueryHandler_Expense, 
                                            DBSelectQueryHandler_Expense>
{
public:
    using base = db::Table<DBModel_Expense, DBInsertQueryHandler_Expense, DBSelectQueryHandler_Expense>;

    REGISTER_TABLE("expense");

    DBTable_Expense(db::Database_SQLite& db, const std::string& tablename)
        : base(db, tablename)
    {
    }

    virtual void InitializeColumns() override;

    DBTable_Expense(const DBTable_Expense& rhs) = delete;
    void operator=(const DBTable_Expense& rhs) = delete;

};



