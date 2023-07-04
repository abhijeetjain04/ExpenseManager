#include "pch.h"
#include "DBTable_Expense.h"
#include "DBHandler/Util.h"
#include "Utils.h"
#include "ConfigManager.h"

namespace em
{

    void DBTable_Expense::InitializeColumns()
    {
        m_ColumnProperties = {
            db::ColumnProperty_RowID(),
            {
                "name",
                db::ColumnProperty::TEXT,
                false,
                true,
                false
            },
            {
                "category",
                db::ColumnProperty::TEXT,
                false,
                true,
                false
            },
            {
                "price",
                db::ColumnProperty::DOUBLE,
                false,
                true,
                false
            },
            {
                "date",
                db::ColumnProperty::DATE,
                false,
                false,
                false
            },
            {
                "location",
                db::ColumnProperty::TEXT,
                false,
                true,
                false
            }
        };
    }

    std::string DBInsertQueryHandler_Expense::GenerateQuery(const DBModel_Expense& model)
    {
        const std::string& unformattedQuery = base::GenerateQueryTemplate();
        std::string date = model.Date;
        if (date.empty())
            date = db::util::GetCurrentDate();
        std::string location = model.Location;
        if (location.empty())
            location = ConfigManager::GetInstance().GetDefaultLocation();

        return std::vformat(unformattedQuery, std::make_format_args(model.Name, model.Category, model.Price, date, location));
    }


    bool DBSelectQueryHandler_Expense::ParseResult(
        const std::shared_ptr<SQLite::Statement>& stmt,
        std::vector<DBModel_Expense>& rows)
    {
        try
        {
            int count = 0;
            while (stmt->executeStep())
            {
                int index = 0;
                DBModel_Expense& row = rows.emplace_back();
                row.RowID = stmt->getColumn(index++);
                row.Name = stmt->getColumn(index++).getString();
                row.Category = stmt->getColumn(index++).getString();
                row.Price = stmt->getColumn(index++).getDouble();
                row.Date = stmt->getColumn(index++).getString();
                row.Location = stmt->getColumn(index++).getString();
            }
        }
        catch (SQLite::Exception& e)
        {
            SQLITE_EXCEPTION(e);
            return false;
        }

        return true;
    }

}