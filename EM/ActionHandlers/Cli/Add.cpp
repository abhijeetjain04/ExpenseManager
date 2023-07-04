#include "EM/pch.h"
#include "Add.h"
#include "EM/DatabaseManager.h"
#include "EM/DBTables.h"
#include "EM/Conditions.h"

#include "DBHandler/Util.h"

namespace em::action_handler::cli
{

    ResultSPtr Add::Execute(
        const std::string& commandName,
        const std::unordered_set<std::string>& flags,
        const std::map<std::string, std::string>& options)
    {
        assert(commandName == "add");

        auto categoryTable = databaseMgr.GetTable<DBTable_Category>();
        auto expenseTable = databaseMgr.GetTable<DBTable_Expense>();

        DBModel_Expense model;
        // validate if the category exists
        model.Category = options.at("category");
        if (!categoryTable->CheckIfExists("name", model.Category))
            return Result::Create(StatusCode::CategoryDoesNotExist, std::format(ERROR_CATEGORY_DOES_NOT_EXIST, model.Category));

        model.Name = options.at("name");
        model.Price = std::stod(options.at("price"));
        model.Date = options.contains("date") ? options.at("date") : "";
        model.Location = options.contains("location") ? options.at("location") : "";

        if (!expenseTable->Insert(model))
        {
            ERROR_LOG(ERROR_DB_INSERT_EXPENSE, model.Name);
            return Result::Create(StatusCode::DBError, std::format(ERROR_DB_INSERT_EXPENSE, model.Name));
        }

        return Result::Create(StatusCode::Success);
    }

}