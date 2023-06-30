#pragma once

#include "DBHandler/Table.h"
#include "DBHandler/QueryHandler.h"
#include "DBHandler/Database_SQLite.h"

BEGIN_NAMESPACE_EM

/**
* This class represents the Data Model for a row in DBTable_Expense.
*/
struct DBModel_Expense
{
    int         RowID;
    std::string Name;
    std::string Category;
    double      Price;
    std::string Date;
    std::string Location;
};

/**
* This class Handles the insertion query into DBTable_Expense.
*/
class DBInsertQueryHandler_Expense : public db::IInsertQueryHandler<DBModel_Expense>
{
public:
    using base = db::IInsertQueryHandler<DBModel_Expense>;

    /**
    * Constructor
    * 
    * @params [in] table
    *       Reference to the DBTable class's object in which the new row will be inserted.
    */
    DBInsertQueryHandler_Expense(db::TableBase& table)
        : base(table)
    {
    }
    
    /**
    * This function generates the sql query that will eventually be executed for inserting a new row in the table.
    * 
    * @params [in] model
    *       class DBModel_Expense object that contains then data that needs to be added to the table.
    * 
    * @returns
    *       std::string object representing the query that will be executed.
    */
    std::string GenerateQuery(const DBModel_Expense& model);
};

/**
* This class Handles the selection query into DBTable_Expense.
*/
class DBSelectQueryHandler_Expense : public db::ISelectQueryHandler<DBModel_Expense>
{
public:
    using base = db::ISelectQueryHandler<DBModel_Expense>;

    /**
    * Constructor
    *
    * @params [in] table
    *       Reference to the DBTable class's object in which the new row will be inserted.
    */
    DBSelectQueryHandler_Expense(db::TableBase& table)
        : base(table)
    {
    }

    /**
    * This function will be called after the select query result is fetched.
    * This function should be used to convert data to DBModel class's object
    * 
    * @params [in] stmt
    *       Database handler library's Data structure that contains the select query result data.
    */
    bool ParseResult(const std::shared_ptr<SQLite::Statement>& stmt, std::vector<DBModel_Expense>& rows) override;
};


#define DB_EXPENSE_TABLE(classname, db_tablename) \
class classname : public DBTable_Expense \
{ \
    using base = DBTable_Expense; \
 \
public: \
    REGISTER_TABLE(db_tablename) \
 \
    classname(db::Database_SQLite& db, const std::string& tablename) \
    : base(db, tablename) \
    { \
    } \
};


/**
* This class represents the 'Expense' table from the Database.
* Object of this class should only be created through DatabaseManager class.
*/
class DBTable_Expense : public db::Table   <DBModel_Expense, 
                                            DBInsertQueryHandler_Expense, 
                                            DBSelectQueryHandler_Expense>
{
public:
    using base = db::Table<DBModel_Expense, DBInsertQueryHandler_Expense, DBSelectQueryHandler_Expense>;

    REGISTER_TABLE("expense");

    /**
    * Constructor
    * 
    * @params [in] db
    *       Object of the Database Library's DataStructure which represents the entire Database.
    * 
    * @param [in] tablename
    *       Name of the table.
    */
    DBTable_Expense(db::Database_SQLite& db, const std::string& tablename)
        : base(db, tablename)
    {
    }

    /**
    * Initializes the logical column Data Structure for this DBTable.
    */
    virtual void InitializeColumns() override;

    DBTable_Expense(const DBTable_Expense& rhs) = delete;
    void operator=(const DBTable_Expense& rhs) = delete;

};

DB_EXPENSE_TABLE(DBTable_PersonalExpense, "personal_expense");
DB_EXPENSE_TABLE(DBTable_HouseholdExpense, "household_expense");
DB_EXPENSE_TABLE(DBTable_MarriageExpense, "marriage_expense");


END_NAMESPACE_EM

