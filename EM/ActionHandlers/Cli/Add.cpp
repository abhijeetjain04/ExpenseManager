#include "EM/pch.h"
#include "Add.h"
#include "EM/DatabaseManager.h"
#include "EM/Conditions.h"
#include "EM/ConfigManager.h"

#include "DBHandler/Util.h"
#include "DBHandler/Table.h"

namespace em::action_handler::cli
{

    ResultSPtr Add::Execute(
        const std::string& commandName,
        const std::unordered_set<std::string>& flags,
        const std::map<std::string, std::string>& options)
    {
        assert(commandName == "add");

        auto categoryTable = databaseMgr.GetTable("categories");
        auto expenseTable = databaseMgr.GetTable("household_expense");

        db::Model model;
        // validate if the category exists
        model["category"] = options.at("category");
        if (!categoryTable->CheckIfExists("name", model["category"]))
            return Result::Create(StatusCode::CategoryDoesNotExist, std::format(ERROR_CATEGORY_DOES_NOT_EXIST, model["category"].asString()));

        model["name"] = options.at("name");
        model["price"] = std::stod(options.at("price"));

        model["location"] = options.contains("location") ? options.at("location") : ConfigManager::GetInstance().GetDefaultLocation();

        if (flags.contains("yesterday"))
            model["date"] = db::util::GetYesterdayDate();
        else
            model["date"] = options.contains("date") ? options.at("date") : db::util::GetCurrentDate();

        if (!expenseTable->Insert(model))
        {
            ERROR_LOG(ERROR_DB_INSERT_EXPENSE, model["name"].asString());
            return Result::Create(StatusCode::DBError, std::format(ERROR_DB_INSERT_EXPENSE, model["name"].asString()));
        }

        return Result::Create(StatusCode::Success);
    }

}