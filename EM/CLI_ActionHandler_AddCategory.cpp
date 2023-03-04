#include "pch.h"
#include "CLI_ActionHandler_AddCategory.h"
#include "DatabaseManager.h"
#include "DBTables.h"
#include "Conditions.h"
#include "Renderer_TextTable.h"

#include "DBHandler/Util.h"

namespace em::action_handler::cli
{

    em::action_handler::ResultSPtr AddCategory::Execute(
        const std::string& commandName,
        const std::unordered_set<std::string>& flags,
        const std::map<std::string, std::string>& options)
    {
        assert(commandName == "addCategory");
        
        DBModel_Category model;
        model.Name = options.at("name");

        auto table = databaseMgr.GetTable<DBTable_Category>();
        if (!table->Insert(model))
        {
            ERROR_LOG(ERROR_DB_INSERT_CATEGORY, model.Name);
            return Result::Create(ErrorCode::DBError, std::format(ERROR_DB_INSERT_CATEGORY, model.Name));
        }

        return Result::Create(ErrorCode::Success);
    }

}