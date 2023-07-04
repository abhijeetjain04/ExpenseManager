#include "pch.h"
#include "DBTable_Category.h"
#include "DBHandler/Util.h"


namespace em
{


    void DBTable_Category::InitializeColumns()
    {
        m_ColumnProperties = {
            db::ColumnProperty_RowID(),
            {
                "name",
                db::ColumnProperty::TEXT,
                false,
                true,
                true
            },
        };
    }

    std::string DBInsertQueryHandler_Category::GenerateQuery(const DBModel_Category& model)
    {
        const std::string& unformattedQuery = base::GenerateQueryTemplate();
        return std::vformat(unformattedQuery, std::make_format_args(model.Name));
    }


    bool DBSelectQueryHandler_Category::ParseResult(
        const std::shared_ptr<SQLite::Statement>& stmt,
        std::vector<DBModel_Category>& rows)
    {
        try
        {
            int count = 0;
            while (stmt->executeStep())
            {
                int index = 0;
                DBModel_Category& row = rows.emplace_back();
                row.RowID = stmt->getColumn(index++);
                row.Name = stmt->getColumn(index++).getString();
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
