#include "EM/pch.h"
#include "List.h"
#include "EM/DatabaseManager.h"
#include "EM/DBTables.h"
#include "EM/Conditions.h"
#include "EM/Renderer_TextTable.h"
#include "EM/Account/Account.h"
#include "EM/Account/Manager.h"

#include "DBHandler/Util.h"

namespace em::action_handler::cli
{

    // protected
	em::action_handler::ResultSPtr List::Execute(
		const std::string& commandName,
		const std::unordered_set<std::string>& flags,
		const std::map<std::string, std::string>& options)
	{
		assert(commandName == "list");

		if (flags.contains("categories"))
			return ListCategories();

        db::Condition finalCondition;

        // handle dates
        if (flags.contains("thisMonth"))
            finalCondition.Add(Condition_Month::Create(db::util::GetThisMonth()));
        if (flags.contains("today"))
            finalCondition.Add(Condition_Date::Create(db::util::GetCurrentDate()));
        if (flags.contains("thisYear"))
            finalCondition.Add(Condition_Year::Create(db::util::GetThisYear()));
        if (options.contains("date"))
        {
            std::string date = options.at("date");
            finalCondition.Add(Condition_Date::Create(date));
        }

        if (options.contains("month") && options.contains("year"))
        {
            std::string month = options.at("month");
            utils::FixMonthName(month);

            std::string year = options.at("year");
            finalCondition.Add(Condition_Month::Create(month, year));
        }
        else if (options.contains("month"))
        {
            std::string month = options.at("month");
            utils::FixMonthName(month);
            finalCondition.Add(Condition_Month::Create(month));
        }
        else if (options.contains("year"))
        {
            std::string year = options.at("year");
            finalCondition.Add(Condition_Year::Create(year));
        }

        // handle name
        if (options.contains("name"))
        {
            std::string name = options.at("name");
            finalCondition.Add(Condition_ListNameFilter::Create(name));
        }

        // handle location
        if (options.contains("location"))
        {
            std::string location = options.at("location");
            finalCondition.Add(Condition_LocationFilter::Create(location));
        }

        // handle categories
        if (options.contains("category"))
        {
            db::Condition* categoryConditions = new db::Condition(db::Condition::OR);
            std::vector<std::string> categories;
            em::utils::string::SplitString(options.at("category"), ',', categories);
            for (const std::string category : categories)
            {
                // check if the category is valid.
                if (!databaseMgr.GetTable<DBTable_Category>()->CheckIfExists("name", category))
                {
                    return em::action_handler::Result::Create(
                        StatusCode::CategoryDoesNotExist,
                        std::format(ERROR_CATEGORY_DOES_NOT_EXIST, category));
                }

                categoryConditions->Add(Condition_Category::Create(category));
            }

            finalCondition.Add(categoryConditions);
        }

        db::Clause_OrderBy orderBy("date", db::Clause_OrderBy::DESCENDING);
        if (flags.contains("ascending"))
            orderBy.SetType(db::Clause_OrderBy::ASCENDING);

        return ProcessDBTable(finalCondition, orderBy);
	}

    // protected
    em::action_handler::ResultSPtr List::ProcessDBTable(const db::Condition& dbCondition, const db::Clause_OrderBy& orderBy)
    {
        std::vector<DBModel_Expense> rows;

        auto expenseTable = databaseMgr.GetTable<DBTable_Expense>();
        if (!expenseTable->Select(rows, dbCondition, orderBy))
            return Result::Create(StatusCode::DBError, "Failed to retrieve from table!");

        // sort according to price, highest to lowest
        std::sort(rows.begin(), rows.end(),
            [](const DBModel_Expense& e1, const DBModel_Expense& e2)
            {
                return e1.Price > e2.Price;
            });

        const std::string& currentAccountName = em::account::Manager::GetInstance().GetCurrentAccount()->GetName();

        double totalExpense = expenseTable->SumOf("price", dbCondition);
        Renderer_ExpenseTable::Render(currentAccountName, rows, totalExpense);

        return Result::Create(StatusCode::Success);
    }

    // protected
	em::action_handler::ResultSPtr List::ListCategories()
	{
		std::vector<DBModel_Category> rows;

		auto table = DatabaseManager::GetInstance().GetTable<DBTable_Category>();
		if (!table->Select(rows))
		{
			ERROR_LOG(ERROR_DB_SELECT_CATEGORY);
			return em::action_handler::Result::Create(StatusCode::DBError, ERROR_DB_SELECT_CATEGORY);
		}

		Renderer_CategoryTable::Render(rows);
		return em::action_handler::Result::Create(StatusCode::Success);
	}

}