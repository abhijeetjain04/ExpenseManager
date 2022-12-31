#include "pch.h"
#include "ReportHandler.h"
#include "DBHandler/Util.h"
#include "DBTable_Category.h"
#include "DBTable_Expense.h"
#include "Conditions.h"
#include "ErrorCodes.h"
#include "TextTable.h"


//public
ErrorCode ReportHandler::GenerateReport(Option option, int month)
{
    db::ConditionGroup condGroup;

    switch (option)
    {
    case Option::TODAY:
        condGroup.Add(Condition_Date(db::util::GetCurrentDate()));
        break;
    case Option::THIS_MONTH:
        condGroup.Add(Condition_Month(db::util::GetThisMonth()));
        break;
    case Option::THIS_YEAR:
        condGroup.Add(Condition_Month(db::util::GetThisYear()));
        break;
    case Option::MONTH:
        condGroup.Add(Condition_Month(std::to_string(month)));
        break;
    }

    std::vector<DBModel_Category> categories;
    auto categoryTable = m_Database->CreateTable<DBTable_Category>();
    categoryTable->Select(categories);

    auto expenseTable = m_Database->CreateTable<DBTable_Expense>();
    for (const DBModel_Category& category : categories)
    {
        condGroup.Add(Condition_Category(category.Name));

        std::vector<DBModel_Expense> expenses;
        expenseTable->Select(expenses, condGroup);

        double total = 0.0;
        for (const DBModel_Expense& expense : expenses)
            total += expense.Price;

        m_Prices.push_back(std::make_pair(category.Name, total));

        // need to pop the last condition so that category can be changed
        condGroup.PopBack();
    }

    return ErrorCode::Success;
}

//public
void ReportHandler::Print(bool sort)
{
    // sort prices from highest to lowest
    if (sort)
    {
        std::sort(m_Prices.begin(), m_Prices.end(),
            [](const std::pair<std::string, double>& p1, const std::pair<std::string, double>& p2)
            {
                return p1.second > p2.second;
            });
    }   

    double totalExpense = 0.0;

    TextTable t;
    t.add("Category").add("Total").endOfRow();
    for (const auto& [categoryName, price] : m_Prices)
    {
        t.add(categoryName)
            .add(std::to_string(price))
            .endOfRow();

        totalExpense += price;
    }

    printf("\n%s", t.toString().c_str());

    TextTable expenseTable;
    expenseTable.add("TOTAL").add(std::to_string(totalExpense)).endOfRow();
    printf("\n%s", expenseTable.toString().c_str());
}


