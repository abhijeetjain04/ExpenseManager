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

        Condition_Category(const std::string& category)
            : base("category", category, base::eEQUALS)
        {
        }
        
    public:
        static Condition_Category* Create(const std::string& category) { return new Condition_Category(category); }
    };

    /**
    * This class represents the condition used to handle the filteration on 'list' command based on date.
    */
    class Condition_Date : public db::Condition
    {
        using base = db::Condition;

        Condition_Date(const std::string& date)
            : base("date", date, db::Condition::eEQUALS)
        {}

    public:
        static Condition_Date* Create(const std::string& date) { return new Condition_Date(date); }
    };

    /**
    * Condition for current year's month.
    */
    class Condition_Month : public db::Condition
    {
        using base = db::Condition;

        Condition_Month(const std::string& month, const std::string year)
            : base("date", std::format("{}-{}-%", year, month), db::Condition::eLIKE)
        {}

    public:
        static Condition_Month* Create(const std::string& month, const std::string year = db::util::GetThisYear())
        {
            return new Condition_Month(month, year);
        }
    };

    /**
    * This class represents the condition used to handle the filteration on 'list' command based on year.
    */
    class Condition_Year : public db::Condition
    {
        using base = db::Condition;

        Condition_Year(const std::string& year)
            : base("date", std::format("{}-%", year), db::Condition::eLIKE)
        {}

    public:
        static Condition_Year* Create(const std::string& year)
        {
            return new Condition_Year(year);
        }
    };

    /**
    * This class represents the condition used to handle the filteration on 'list' command based on month and year.
    */
    class Condition_MonthAndYear : public db::Condition
    {
        using base = db::Condition;

        Condition_MonthAndYear(const std::string& month, const std::string& year)
            : base("date", std::format("{}-{}-%", year, month), db::Condition::eLIKE)
        {}

    public:
        static Condition_MonthAndYear* Create(const std::string& month, const std::string& year)
        {
            return new Condition_MonthAndYear(month, year);
        }
    };

    /**
    * This class represents the condition used to remove a row from table based on the rowID.
    */
    class Condition_DeleteRow : public db::Condition
    {
        using base = db::Condition;

        Condition_DeleteRow(const std::string& rowID)
            : base("row_id", rowID, db::Condition::eEQUALS)
        {}

    public:
        static Condition_DeleteRow* Create(int rowID)
        {
            return new Condition_DeleteRow(std::to_string(rowID));
        }
        static Condition_DeleteRow* Create(const std::string& rowID)
        {
            return new Condition_DeleteRow(rowID);
        }
    };


    /**
    * This class represents the condition used to handle the filteration on 'list' command based on name.
    */
    class Condition_ListNameFilter : public db::Condition
    {
        using base = db::Condition;

        Condition_ListNameFilter(const std::string& name)
            : base("name", std::format("%{}%", name), db::Condition::eLIKE)
        {}

    public:
        static Condition_ListNameFilter* Create(const std::string& name)
        {
            return new Condition_ListNameFilter(name);
        }
    };

    /**
    * This class represents the condition used to handle the filteration on 'list' command based on location.
    */
    class Condition_LocationFilter : public db::Condition
    {
        using base = db::Condition;

        Condition_LocationFilter(const std::string& location)
            : base("location", location, db::Condition::eEQUALS)
        {}

    public:
        static Condition_LocationFilter* Create(const std::string& location)
        {
            return new Condition_LocationFilter(location);
        }
    };

}
