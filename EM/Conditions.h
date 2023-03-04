#pragma once

#include "DBHandler/Condition.h"

BEGIN_NAMESPACE_EM

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


class Condition_Year : public db::Condition
{
    using base = db::Condition;

public:
    Condition_Year(const std::string& year)
        : base("date", std::format("{}-%", year), db::Condition::eLIKE)
    {}

};

class Condition_MonthAndYear : public db::Condition
{
    using base = db::Condition;

public:
    Condition_MonthAndYear(const std::string& month, const std::string& year)
        : base("date", std::format("{}-{}-%", year, month), db::Condition::eLIKE)
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

class Condition_LocationFilter : public db::Condition
{
    using base = db::Condition;

public:
    Condition_LocationFilter(const std::string& location)
        : base("location", location, db::Condition::eEQUALS)
    {}
};

END_NAMESPACE_EM
