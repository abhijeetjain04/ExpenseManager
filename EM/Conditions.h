#pragma once

#include "DBHandler/Condition.h"


class Condition_Category : public db::Condition
{
    using base = db::Condition;

public:
    Condition_Category(const std::string& category)
        : base("category", category, base::eEQUALS)
    {
    }
};


class Condition_Date : public db::Condition
{
    using base = db::Condition;

public:
    Condition_Date(const std::string& date)
        : base("date", date, db::Condition::eEQUALS)
    {}
};


class Condition_Month : public db::Condition
{
    using base = db::Condition;

public:
    Condition_Month(const std::string& month)
        : base("date", std::format("%-{}-%", month), db::Condition::eLIKE)
    {}

    Condition_Month(int month)
        : Condition_Month(std::to_string(month))
    {}
};


class Condition_Year : public db::Condition
{
    using base = db::Condition;

public:
    Condition_Year(const std::string& year)
        : base("date", std::format("%-{}", year), db::Condition::eLIKE)
    {}

    Condition_Year(int year)
        : Condition_Year(std::to_string(year))
    {}
};


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


class Condition_ListNameFilter : public db::Condition
{
    using base = db::Condition;

public:
    Condition_ListNameFilter(const std::string& name)
        : base("name", std::format("%{}%", name), db::Condition::eLIKE)
    {}

};
