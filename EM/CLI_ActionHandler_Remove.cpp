#include "pch.h"
#include "CLI_ActionHandler_Remove.h"
#include "DatabaseManager.h"
#include "DBTables.h"
#include "Conditions.h"
#include "Renderer_TextTable.h"

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

        if (!table->Delete(Condition_DeleteRow(rowID)))
        {
            ERROR_LOG(ERROR_DB_REMOVE_EXPENSE, rowID);
            return Result::Create(ErrorCode::DBError, std::format(ERROR_DB_REMOVE_EXPENSE, rowID));
        }

        return Result::Create(ErrorCode::Success);
    }

}