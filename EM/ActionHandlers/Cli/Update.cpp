#include "EM/pch.h"
#include "Update.h"
#include "EM/DatabaseManager.h"
#include "EM/Conditions.h"
#include "EM/ConfigManager.h"
#include "EM/Exceptions/General.h"

#include "DBHandler/Util.h"
#include "DBHandler/Table.h"
#include "Utilities/StringUtils.h"
#include "Utilities/GeneralUtils.h"

namespace em::action_handler::cli
{

    ResultSPtr Update::Execute(
        const std::string& commandName,
        const std::unordered_set<std::string>& flags,
        const std::map<std::string, std::string>& options)
    {
        assert(commandName == "update");

        int rowId = std::atoi(options.at("row_id").c_str());
        std::string attributeName = options.at("attributeName");
        std::string attributeValue = options.at("attributeValue");

        // if category is getting update, validate if its a valid category
        if (attributeName == "category")
        {
            auto categoryTable = databaseMgr.GetTable("categories");
            if (!categoryTable->CheckIfExists("name", attributeValue))
            {
                ERROR_LOG(ERROR_CATEGORY_DOES_NOT_EXIST, attributeValue);
                return Result::Create(StatusCode::DBError, std::format(ERROR_CATEGORY_DOES_NOT_EXIST, attributeValue));
            }
        }

        // check price, must be a double
        if (attributeName == "price")
        {
            if (!utils::general::IsDouble(attributeValue))
            {
                ERROR_LOG(ERROR_INVALID_ATTRIBUTE_TYPE, attributeValue);
                return Result::Create(StatusCode::Invalid, std::format(ERROR_INVALID_ATTRIBUTE_TYPE, attributeValue));
            }
        }

        auto expenseTable = databaseMgr.GetTable(databaseMgr.GetCurrentExpenseTableName());

        db::Model origModel;
        expenseTable->SelectById(origModel, rowId);

        db::Model newModel = origModel;
        newModel[attributeName] = attributeValue;

        if (!expenseTable->Update(origModel, newModel))
        {
            ERROR_LOG(ERROR_DB_UPDATE_EXPENSE, newModel["name"].asString());
            return Result::Create(StatusCode::DBError, std::format(ERROR_DB_UPDATE_EXPENSE, newModel["name"].asString()));
        }

        return Result::Create(StatusCode::Success);
    }

}