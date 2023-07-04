#pragma once

#include "DBHandler/Condition.h"
#include "DBHandler/Util.h"

namespace em
{


    /**
    * This class represents the condition used to handle the filteration on 'list' command based on category.
    */
    class Condition_Category : public db::Condition
    {
        using base = db::Condition;

    public:
        Condition_Category(const std::string& category)
            : base("category", category, base::eEQUALS)
        {
        }
    };

    /**
    * This class represents the condition used to handle the filteration on 'list' command based on date.
    */
    class Condition_Date : public db::Condition
    {
        using base = db::Condition;

    public:
        Condition_Date(const std::string& date)
            : base("date", date, db::Condition::eEQUALS)
        {}
    };

    /**
    * Condition for current year's month.
    */
    class Condition_Month : public db::Condition
    {
        using base = db::Condition;

    public:
        Condition_Month(const std::string& month, const std::string year = db::util::GetThisYear())
            : base("date", std::format("{}-{}-%", year, month), db::Condition::eLIKE)
        {}
    };

    /**
    * This class represents the condition used to handle the filteration on 'list' command based on year.
    */
    class Condition_Year : public db::Condition
    {
        using base = db::Condition;

    public:
        Condition_Year(const std::string& year)
            : base("date", std::format("{}-%", year), db::Condition::eLIKE)
        {}

    };

    /**
    * This class represents the condition used to handle the filteration on 'list' command based on month and year.
    */
    class Condition_MonthAndYear : public db::Condition
    {
        using base = db::Condition;

    public:
        Condition_MonthAndYear(const std::string& month, const std::string& year)
            : base("date", std::format("{}-{}-%", year, month), db::Condition::eLIKE)
        {}

    };

    /**
    * This class represents the condition used to remove a row from table based on the rowID.
    */
    class Condition_DeleteRow : public db::Condition
    {
        using base = db::Condition;

    public:
        Condition_DeleteRow(const std::string& rowID)
            : base("row_id", rowID, db::Condition::eEQUALS)
        {}

        Condition_DeleteRow(int rowID)
            : Condition_DeleteRow(std::to_string(rowID))
        {}
    };


    /**
    * This class represents the condition used to handle the filteration on 'list' command based on name.
    */
    class Condition_ListNameFilter : public db::Condition
    {
        using base = db::Condition;

    public:
        Condition_ListNameFilter(const std::string& name)
            : base("name", std::format("%{}%", name), db::Condition::eLIKE)
        {}

    };

    /**
    * This class represents the condition used to handle the filteration on 'list' command based on location.
    */
    class Condition_LocationFilter : public db::Condition
    {
        using base = db::Condition;

    public:
        Condition_LocationFilter(const std::string& location)
            : base("location", location, db::Condition::eEQUALS)
        {}
    };

}
