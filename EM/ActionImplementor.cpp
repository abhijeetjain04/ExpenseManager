#include "pch.h"
#include "ActionImplementor.h"
#include "CLIParser/CLIParser.h"
#include "DBTable_Expense.h"
#include "DBTable_Category.h"
#include "DBHandler/Util.h"
#include "TextTable.h"
#include "Conditions.h"
#include "ReportHandler.h"
#include "Renderer_TextTable.h"
#include "ErrorMessages.h"



ActionImplementor* ActionImplementor::s_Instance = nullptr;

//private
ActionImplementor::ActionImplementor()
{
}

//public
ActionImplementor* ActionImplementor::GetInstance()
{
    if (s_Instance == nullptr)
        s_Instance = new ActionImplementor();

    return s_Instance;
}

//public
void ActionImplementor::Initialize(const char* dbName, int openMode)
{
    m_Database = std::make_unique<db::Database_SQLite>(dbName, openMode);
}

//public
ErrorCode ActionImplementor::PerformAction(CmdType cmdType)
{
    switch (cmdType)
    {
    case CmdType::HELP:
        return DisplayHelp();
    case CmdType::ADD_CATEGORY:
        return ActionHandler_AddCategory();
    case CmdType::ADD:
        return ActionHandler_Add();
    case CmdType::LIST:
        return ActionHandler_List();
    case CmdType::REMOVE:
        return ActionHandler_Remove();
    case CmdType::REPORT:
        return ActionHandler_Report();
    case CmdType::COMPARE_MONTH:
        return ActionHandler_CompareMonth();
    }

    return ErrorCode::Invalid;
}


ErrorCode ActionImplementor::DisplayHelp()
{
    cliParser.DisplayHelp();
    return ErrorCode::Success;
}


ErrorCode ActionImplementor::ActionHandler_AddCategory()
{
    DBModel_Category model;
    cliParser.GetParam("name", model.Name);

    auto table = GetTable<DBTable_Category>();
    if (!table->Insert(model))
    {
        ERROR_LOG(ERROR_DB_INSERT_CATEGORY, model.Name);
        return ErrorCode::DBError;
    }

    return ErrorCode::Success;
}


ErrorCode ActionImplementor::ActionHandler_Add()
{
    auto categoryTable = GetTable<DBTable_Category>();
    auto expenseTable = GetTable<DBTable_Expense>();

    DBModel_Expense model;
    // validate if the category exists
    cliParser.GetParam("category", model.Category);
    if (!categoryTable->CheckIfExists("name", model.Category))
    {
        ERROR_LOG(ERROR_CATEGORY_DOES_NOT_EXIST, model.Category);
        return ErrorCode::CategoryDoesNotExist;
    }

    cliParser.GetParam("name", model.Name);

    std::string price;
    cliParser.GetParam("price", price);
    model.Price = std::stod(price);
    cliParser.GetParam("date", model.Date);
    cliParser.GetParam("location", model.Location);

    if (!expenseTable->Insert(model))
    {
        ERROR_LOG(ERROR_DB_INSERT_EXPENSE, model.Name);
        return ErrorCode::DBError;
    }

    return ErrorCode::Success;
}

// private
ErrorCode ActionImplementor::ActionHandler_List()
{
    if (cliParser.HasFlag("categories"))
        return ActionHandler_ListCategories();

    db::ConditionGroup condGroup;

    // handle dates
    if (cliParser.HasFlag("thisMonth"))
        condGroup.Add(Condition_Month(db::util::GetThisMonth()));
    if (cliParser.HasFlag("today"))
        condGroup.Add(Condition_Date(db::util::GetCurrentDate()));
    if (cliParser.HasFlag("thisYear"))
        condGroup.Add(Condition_Year(db::util::GetThisYear()));
    if (cliParser.HasParameter("date"))
    {
        std::string date;
        cliParser.GetParam("date", date);
        condGroup.Add(Condition_Date(date));
    }

    if (cliParser.HasParameter("month") && cliParser.HasParameter("year"))
    {
        std::string month;
        cliParser.GetParam("month", month);
        int monthInInt = std::atoi(month.c_str());
        month = (monthInInt < 10 ? "0" + std::to_string(monthInInt) : std::to_string(monthInInt));

        std::string year;
        cliParser.GetParam("year", year);
        condGroup.Add(Condition_Month(month, year));
    }
    else if (cliParser.HasParameter("month"))
    {
        std::string month;
        cliParser.GetParam("month", month);

        int monthInInt = std::atoi(month.c_str());
        month = (monthInInt < 10 ? "0" + std::to_string(monthInInt) : std::to_string(monthInInt));

        condGroup.Add(Condition_Month(month));
    }
    else if (cliParser.HasParameter("year"))
    {
        std::string year;
        cliParser.GetParam("year", year);
        condGroup.Add(Condition_Year(year));
    }

    // handle categories
    if (cliParser.HasParameter("category"))
    {
        std::string category;
        cliParser.GetParam("category", category);
        // check if the category is valid.
        if (!GetTable<DBTable_Category>()->CheckIfExists("name", category))
        {
            ERROR_LOG(ERROR_CATEGORY_DOES_NOT_EXIST, category);
            return ErrorCode::CategoryDoesNotExist;
        }

        condGroup.Add(Condition_Category(category));
    }

    // handle name
    if(cliParser.HasParameter("name"))
    {
        std::string name;
        cliParser.GetParam("name", name);
        condGroup.Add(Condition_ListNameFilter(name));
    }

    // handle location
    if (cliParser.HasParameter("location"))
    {
        std::string location;
        cliParser.GetParam("location", location);
        condGroup.Add(Condition_LocationFilter(location));
    }

    auto table = GetTable<DBTable_Expense>();
    std::vector<DBModel_Expense> rows;

    db::Clause_OrderBy orderBy("date", db::Clause_OrderBy::DESCENDING);
    if (cliParser.HasFlag("ascending"))
        orderBy.SetType(db::Clause_OrderBy::ASCENDING);

    if (!table->Select(rows, condGroup, orderBy))
        return ErrorCode::DBError;

    // sort according to price, highest to lowest

    std::sort(rows.begin(), rows.end(),
        [](const DBModel_Expense& e1, const DBModel_Expense& e2)
        {
            return e1.Price > e2.Price;
        });

    double totalExpense = table->SumOf("price", condGroup);
    Renderer_ExpenseTable::Render(rows, totalExpense);
    return ErrorCode::Success;
}

//private
ErrorCode ActionImplementor::ActionHandler_ListCategories()
{
    std::vector<DBModel_Category> rows;

    auto table = GetTable<DBTable_Category>();
    if (!table->Select(rows))
    {
        ERROR_LOG(ERROR_DB_SELECT_CATEGORY);
        return ErrorCode::DBError;
    }

    Renderer_CategoryTable::Render(rows);
    return ErrorCode::Success;
}

//private
ErrorCode ActionImplementor::ActionHandler_Remove()
{
    auto table = GetTable<DBTable_Expense>();

    std::string rowID;
    cliParser.GetParam("row_id", rowID);

    if (!table->Delete(Condition_DeleteRow(rowID)))
    {
        ERROR_LOG(ERROR_DB_REMOVE_EXPENSE, rowID);
        return ErrorCode::DBError;
    }

    return ErrorCode::Success;
}

//private
ErrorCode ActionImplementor::ActionHandler_Report()
{
    ReportHandler report(m_Database);

    if (cliParser.HasFlag("today"))
    {
        report.GenerateReport(ReportHandler::TODAY);
        report.Print();
        return ErrorCode::Success;
    }

    std::string month = "-1";
    std::string year = "-1";
    ReportHandler::Option option = ReportHandler::ALL;

    // check if month and year both are specified
    if (cliParser.HasParameter("month") && cliParser.HasParameter("year"))
    {
        cliParser.GetParam("month", month);
        cliParser.GetParam("year", year);
        option = ReportHandler::MONTH_AND_YEAR;
    }
    // check if only month is specified.
    else if (cliParser.HasParameter("month"))
    {
        cliParser.GetParam("month", month);
        option = ReportHandler::MONTH;
    }
    // check if only year is specified.
    else if (cliParser.HasParameter("year"))
    {
        cliParser.GetParam("year", year);
        option = ReportHandler::YEAR;
    }
    // check if --thisMonth is specified
    else if (cliParser.HasFlag("thisMonth"))
    {
        option = ReportHandler::MONTH;
        month = db::util::GetThisMonth();
    }
    // check if --thisYear is specified
    else if (cliParser.HasFlag("thisYear"))
    {
        option = ReportHandler::YEAR;
        year = db::util::GetThisYear();
    }

    report.GenerateReport(option, month, year);
    report.Print();

    return ErrorCode::Success;
}

//private
ErrorCode ActionImplementor::ActionHandler_CompareMonth()
{
    std::string month1;
    cliParser.GetParam("month1", month1);

    std::string month2;
    cliParser.GetParam("month2", month2);

    printf("\n comparing %s, %s", month1.c_str(), month2.c_str());
    ReportHandler report1(m_Database);
    report1.GenerateReport(ReportHandler::MONTH, month1);

    ReportHandler report2(m_Database);
    report2.GenerateReport(ReportHandler::MONTH, month2);

    Renderer_CompareReport::Render(report1, report2);

    return ErrorCode::Success;
}
