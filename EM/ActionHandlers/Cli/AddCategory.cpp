#include "../../pch.h"
#include "AddCategory.h"
#include "EM/DatabaseManager.h"
#include "EM/Conditions.h"

#include "DBHandler/Util.h"
#include "DBHandler/Table.h"

namespace em::action_handler::cli
{

    em::action_handler::ResultSPtr AddCategory::Execute(
        const std::string& commandName,
        const std::unordered_set<std::string>& flags,
        const std::map<std::string, std::string>& options)
    {
        assert(commandName == "addCategory");
        
        db::Model model;
        model["name"] = options.at("name");

        auto table = databaseMgr.GetTable("categories");
        if (!table->Insert(model))
        {
            ERROR_LOG(ERROR_DB_INSERT_CATEGORY, model["name"].asString());
            return Result::Create(StatusCode::DBError, std::format(ERROR_DB_INSERT_CATEGORY, model["name"].asString()));
        }

        return Result::Create(StatusCode::Success);
    }

}