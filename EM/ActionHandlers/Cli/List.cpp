#include "EM/pch.h"
#include "List.h"
#include "EM/DatabaseManager.h"
#include "EM/Conditions.h"
#include "EM/Renderer_TextTable.h"
#include "EM/Account/Account.h"
#include "EM/Account/Manager.h"

#include "DBHandler/Util.h"
#include "DBHandler/Table.h"

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
        
        if (flags.contains("yesterday"))
        {
            std::string date = db::util::GetYesterdayDate();
            finalCondition.Add(Condition_Date::Create(date));
        }
        else if (options.contains("date"))
        {
            std::string date = options.at("date");
            finalCondition.Add(Condition_Date::Create(date));
        }

        if (options.contains("month") && options.contains("year"))
        {
            std::string month = options.at("month");
            utils::date::FixMonthName(month);

            std::string year = options.at("year");
            finalCondition.Add(Condition_Month::Create(month, year));
        }
        else if (options.contains("month"))
        {
            std::string month = options.at("month");
            utils::date::FixMonthName(month);
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
            auto result = AppendCategoryCondition(finalCondition, options.at("category"));
            if (result->statusCode != StatusCode::Success)
                return result;
        }
        else if (options.contains("ignoreCategory"))
        {
            std::vector<std::string> categories;
            em::utils::string::SplitString(options.at("ignoreCategory"), categories);
            for (const std::string& category : categories) 
                finalCondition.Add(Condition_IgnoreCategory::Create(category));
        }

        db::Clause_OrderBy orderBy("date", db::Clause_OrderBy::DESCENDING);
        if (flags.contains("ascending"))
            orderBy.SetType(db::Clause_OrderBy::ASCENDING);

        return ProcessDBTable(finalCondition, orderBy);
	}

    // protected
    em::action_handler::ResultSPtr List::ProcessDBTable(const db::Condition& dbCondition, const db::Clause_OrderBy& orderBy)
    {
        std::vector<db::Model> rows;

        const std::string tableName = databaseMgr.GetCurrentExpenseTableName();
        auto expenseTable = databaseMgr.GetTable(tableName);
        if (!expenseTable->Select(rows, dbCondition, orderBy))
            return Result::Create(StatusCode::DBError, "Failed to retrieve from table!");

        // sort according to price, highest to lowest
        std::sort(rows.begin(), rows.end(),
            [](db::Model& e1, db::Model& e2)
            {
                return e1["price"].asDouble() > e2["price"].asDouble();
            });

        const std::string& currentAccountName = em::account::Manager::GetInstance().GetCurrentAccount()->GetName();

        double totalExpense = expenseTable->SumOf("price", dbCondition);
        Renderer_ExpenseTable::Render(currentAccountName, rows, totalExpense);

        return Result::Create(StatusCode::Success);
    }

    // protected
	em::action_handler::ResultSPtr List::ListCategories()
	{
		std::vector<db::Model> rows;

		auto table = databaseMgr.GetTable("categories");
		if (!table->Select(rows))
		{
			ERROR_LOG(ERROR_DB_SELECT_CATEGORY);
			return em::action_handler::Result::Create(StatusCode::DBError, ERROR_DB_SELECT_CATEGORY);
		}

		Renderer_CategoryTable::Render(rows);
		return em::action_handler::Result::Create(StatusCode::Success);
	}

    // private
    em::action_handler::ResultSPtr List::AppendCategoryCondition(
        db::Condition& finalCondition, 
        const std::string& categoriesStr) const
    {
        db::Condition* categoryConditions = new db::Condition(db::Condition::RelationshipType::OR);
        std::vector<std::string> categories;
        em::utils::string::SplitString(categoriesStr, categories);

        auto table = databaseMgr.GetTable("categories");
        for (const std::string category : categories)
        {
            // check if the category is valid.
            if (!table->CheckIfExists("name", category))
            {
                return em::action_handler::Result::Create(
                    StatusCode::CategoryDoesNotExist,
                    std::format(ERROR_CATEGORY_DOES_NOT_EXIST, category));
            }

            categoryConditions->Add(Condition_Category::Create(category));
        }

        finalCondition.Add(categoryConditions);

        return em::action_handler::Result::Create(StatusCode::Success);
    }
}