#include "EM/pch.h"
#include "Remove.h"
#include "EM/DatabaseManager.h"
#include "EM/DBTables.h"
#include "EM/Conditions.h"
#include "EM/Renderer_TextTable.h"
#include "EM/Account/Account.h"
#include "EM/Account/Manager.h"

#include "DBHandler/Util.h"

namespace em::action_handler::cli
{

    em::action_handler::ResultSPtr Remove::Execute(
        const std::string& commandName,
        const std::unordered_set<std::string>& flags,
        const std::map<std::string, std::string>& options)
    {
        DBG_ASSERT(commandName == "remove");
        auto table = databaseMgr.GetTable<DBTable_Expense>();

        std::string rowID = options.at("row_id");

        auto condition = Condition_DeleteRow(rowID);

        std::vector<DBModel_Expense> rows;
        if (!table->Select(rows, condition))
        {
            ERROR_LOG(ERROR_DB_REMOVE_EXPENSE, rowID);
            return Result::Create(StatusCode::DBError, std::format(ERROR_DB_REMOVE_EXPENSE, rowID));
        }

        const std::string& currentAccountName = em::account::Manager::GetInstance().GetCurrentAccount()->GetName();

        printf("\nFollowing row will be deleted: ");
        Renderer_ExpenseTable::Render(currentAccountName, rows);
        printf("\nProceed? : ");

        int userInput = 0;
        std::cin >> userInput;
        if (userInput == 0)
        {
            printf("\nSkipped deletion.");
            return Result::Create(StatusCode::Success);
        }

        if (!table->Delete(condition))
        {
            ERROR_LOG(ERROR_DB_REMOVE_EXPENSE, rowID);
            return Result::Create(StatusCode::DBError, std::format(ERROR_DB_REMOVE_EXPENSE, rowID));
        }

        return Result::Create(StatusCode::Success);
    }

}