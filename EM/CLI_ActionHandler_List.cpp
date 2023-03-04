#include "pch.h"
#include "CLI_ActionHandler_List.h"
#include "DatabaseManager.h"
#include "DBTables.h"
#include "Conditions.h"
#include "Renderer_TextTable.h"

#include "DBHandler/Util.h"

namespace em::action_handler::cli
{

	em::action_handler::ResultSPtr List::Execute(
		const std::string& commandName,
		const std::unordered_set<std::string>& flags,
		const std::map<std::string, std::string>& options)
	{
		assert(commandName == "list");

		if (flags.contains("categories"))
			return ListCategories();

        db::ConditionGroup condGroup;

        // handle dates
        if (flags.contains("thisMonth"))
            condGroup.Add(Condition_Month(db::util::GetThisMonth()));
        if (flags.contains("today"))
            condGroup.Add(Condition_Date(db::util::GetCurrentDate()));
        if (flags.contains("thisYear"))
            condGroup.Add(Condition_Year(db::util::GetThisYear()));
        if (options.contains("date"))
        {
            std::string date = options.at("date");
            condGroup.Add(Condition_Date(date));
        }

        if (options.contains("month") && options.contains("year"))
        {
            std::string month = options.at("month");
            utils::FixMonthName(month);

            std::string year = options.at("year");
            condGroup.Add(Condition_Month(month, year));
        }
        else if (options.contains("month"))
        {
            std::string month = options.at("month");
            utils::FixMonthName(month);
            condGroup.Add(Condition_Month(month));
        }
        else if (options.contains("year"))
        {
            std::string year = options.at("year");
            condGroup.Add(Condition_Year(year));
        }

        // handle categories
        if (options.contains("category"))
        {
            std::string category = options.at("category");
            // check if the category is valid.
            if (!databaseMgr.GetTable<DBTable_Category>()->CheckIfExists("name", category))
            {
                return em::action_handler::Result::Create(
                    ErrorCode::CategoryDoesNotExist, 
                    std::format(ERROR_CATEGORY_DOES_NOT_EXIST, category));
            }

            condGroup.Add(Condition_Category(category));
        }

        // handle name
        if (options.contains("name"))
        {
            std::string name = options.at("name");
            condGroup.Add(Condition_ListNameFilter(name));
        }

        // handle location
        if (options.contains("location"))
        {
            std::string location = options.at("location");
            condGroup.Add(Condition_LocationFilter(location));
        }

        auto table = databaseMgr.GetTable<DBTable_Expense>();
        std::vector<DBModel_Expense> rows;

        db::Clause_OrderBy orderBy("date", db::Clause_OrderBy::DESCENDING);
        if (flags.contains("ascending"))
            orderBy.SetType(db::Clause_OrderBy::ASCENDING);

        if (!table->Select(rows, condGroup, orderBy))
            return Result::Create(ErrorCode::DBError, "Failed to retrieve from table!");

        // sort according to price, highest to lowest

        std::sort(rows.begin(), rows.end(),
            [](const DBModel_Expense& e1, const DBModel_Expense& e2)
            {
                return e1.Price > e2.Price;
            });

        double totalExpense = table->SumOf("price", condGroup);
        Renderer_ExpenseTable::Render(rows, totalExpense);

		return Result::Create(ErrorCode::Success);
	}

	em::action_handler::ResultSPtr List::ListCategories()
	{
		std::vector<DBModel_Category> rows;

		auto table = DatabaseManager::GetInstance()->GetTable<DBTable_Category>();
		if (!table->Select(rows))
		{
			ERROR_LOG(ERROR_DB_SELECT_CATEGORY);
			return em::action_handler::Result::Create(ErrorCode::DBError, ERROR_DB_SELECT_CATEGORY);
		}

		Renderer_CategoryTable::Render(rows);
		return em::action_handler::Result::Create(ErrorCode::Success);
	}

}