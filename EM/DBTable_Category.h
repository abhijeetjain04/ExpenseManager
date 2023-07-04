#pragma once

#include "DBHandler/Table.h"
#include "DBHandler/QueryHandler.h"
#include "DBHandler/Database_SQLite.h"

namespace em
{


    /**
    * This class represents the Data Model for a row in DBTable_Category.
    */
    struct DBModel_Category
    {
        int         RowID;
        std::string Name;
    };

    /**
    * This class Handles the insertion query into DBTable_Category.
    */
    class DBInsertQueryHandler_Category : public db::IInsertQueryHandler<DBModel_Category>
    {
    public:
        using base = db::IInsertQueryHandler<DBModel_Category>;

        /**
        * Constructor
        *
        * @params [in] table
        *       Reference to the DBTable class's object in which the new row will be inserted.
        */
        DBInsertQueryHandler_Category(db::TableBase& table)
            : base(table)
        {
        }

        /**
        * This function generates the sql query that will eventually be executed for inserting a new row in the table.
        *
        * @params [in] model
        *       class DBModel_Category object that contains then data that needs to be added to the table.
        *
        * @returns
        *       std::string object representing the query that will be executed.
        */
        std::string GenerateQuery(const DBModel_Category& model);
    };

    /**
    * This class Handles the selection query into DBTable_Expense.
    */
    class DBSelectQueryHandler_Category : public db::ISelectQueryHandler<DBModel_Category>
    {
    public:
        using base = db::ISelectQueryHandler<DBModel_Category>;

        /**
        * Constructor
        *
        * @params [in] table
        *       Reference to the DBTable class's object in which the new row will be inserted.
        */
        DBSelectQueryHandler_Category(db::TableBase& table)
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
        bool ParseResult(const std::shared_ptr<SQLite::Statement>& stmt, std::vector<DBModel_Category>& rows) override;
    };

    /**
    * This class represents the 'Expense' table from the Database.
    * Object of this class should only be created through DatabaseManager class.
    */
    class DBTable_Category : public db::Table   <DBModel_Category,
        DBInsertQueryHandler_Category,
        DBSelectQueryHandler_Category>
    {
    public:
        using base = db::Table<DBModel_Category, DBInsertQueryHandler_Category, DBSelectQueryHandler_Category>;

        REGISTER_TABLE("categories");

        /**
        * Constructor
        *
        * @params [in] db
        *       Object of the Database Library's DataStructure which represents the entire Database.
        *
        * @param [in] tablename
        *       Name of the table.
        */
        DBTable_Category(db::Database_SQLite& db, const std::string& tablename)
            : base(db, tablename)
        {
        }

        /**
        * Initializes the logical column Data Structure for this DBTable.
        */
        virtual void InitializeColumns() override;

        DBTable_Category(const DBTable_Category& rhs) = delete;
        void operator=(const DBTable_Category& rhs) = delete;
    };

}