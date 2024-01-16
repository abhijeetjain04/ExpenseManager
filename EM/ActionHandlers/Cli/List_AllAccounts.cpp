#include "EM/pch.h"
#include "List_AllAccounts.h"
#include "EM/DatabaseManager.h"
#include "EM/DBTables.h"
#include "EM/Conditions.h"
#include "EM/Renderer_TextTable.h"
#include "EM/ConfigManager.h"

#include "DBHandler/Util.h"

namespace em::action_handler::cli
{

    em::action_handler::ResultSPtr List_AllAccounts::ProcessDBTable(const db::ConditionGroup& condGroup, const db::Clause_OrderBy& orderBy)
    {
        std::unordered_map<std::string, std::vector<DBModel_Expense>> rows;

        double totalExpense = 0.0; // store the total expense from all tables.
        const Map_DBTables& expenseTables = em::DatabaseManager::GetInstance().GetExpenseTables();
        for (auto iter = expenseTables.begin(); iter != expenseTables.end(); iter++)
        {
            const std::string& tableName = iter->first;
            const std::shared_ptr<DBTable_Expense> expenseTable = iter->second;
            std::vector<DBModel_Expense> tableRows;
            if (!expenseTable->Select(tableRows, condGroup, orderBy))
                return Result::Create(StatusCode::DBError, std::format("Failed to retrieve from table!", tableName));

            // sort according to price, highest to lowest
            std::sort(tableRows.begin(), tableRows.end(),
                [](const DBModel_Expense& e1, const DBModel_Expense& e2)
                {
                    return e1.Price > e2.Price;
                });

            const std::string& accountName = RetrieveMatchingAccountName(tableName);
            rows.insert(std::make_pair(accountName, std::move(tableRows)));

            totalExpense += expenseTable->SumOf("price", condGroup);
        }

        Renderer_ExpenseTable::Render(rows, totalExpense);

        return Result::Create(StatusCode::Success);
    }

    std::string List_AllAccounts::RetrieveMatchingAccountName(const std::string& tableName) const
    {
        const std::vector<std::string>& accountNames = em::ConfigManager::GetInstance().GetValidAccountNames();
        for (const std::string& accountName : accountNames)
        {
            if (tableName.starts_with(accountName))
                return accountName;
        }
    }

}