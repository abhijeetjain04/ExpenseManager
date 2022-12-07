#pragma once

#include "DBHandler/Database_SQLite.h"

#define CmdString_Help          "help"
#define CmdString_AddCategory   "addCategory"
#define CmdString_Add           "add"
#define CmdString_Remove        "remove"
#define CmdString_List          "list"
#define CmdString_Report        "report"
#define CmdString_CompareMonths "compareMonths"


namespace db { class Database_SQLite; }

struct DBModel_Expense;

enum class CmdType
{
    HELP,
    ADD_CATEGORY,
    ADD,
    REMOVE,
    LIST,
    REPORT,
    COMPARE_MONTH,

    INVALID
};


#define actionImpl (*ActionImplementor::GetInstance())

// singleton
class ActionImplementor
{
public:
    void Initialize(const char* dbName, int openMode = db::OPEN_CREATE | db::OPEN_READWRITE);
    ErrorCode PerformAction(CmdType cmdType);
    static ActionImplementor* GetInstance();

private:
    ActionImplementor();
    ActionImplementor(const ActionImplementor&) = default;

    ErrorCode DisplayHelp();
    ErrorCode ActionHandler_AddCategory();
    ErrorCode ActionHandler_Add();
    ErrorCode ActionHandler_List();
    ErrorCode ActionHandler_ListCategories();
    ErrorCode ActionHandler_Remove();
    ErrorCode ActionHandler_Report();
    ErrorCode ActionHandler_CompareMonth();

    template<typename T>
    std::shared_ptr<T> GetTable() const
    {
        return m_Database->CreateTable<T>();
    }


private:
    std::unique_ptr<db::Database_SQLite> m_Database;

    static ActionImplementor* s_Instance;

};

