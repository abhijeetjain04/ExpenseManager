#pragma once

namespace db { class Database_SQLite; }
namespace db { class Condition; }

namespace em
{

    struct DBModel_Category;
    struct DBModel_Expense;

    class ReportHandler
    {
    public:

        enum Option
        {
            ALL,
            TODAY,
            MONTH,
            YEAR,
            MONTH_AND_YEAR
        };

        ReportHandler();

        StatusCode GenerateReport(Option option = Option::ALL, const std::string& month = "-1", const std::string& year = "-1", bool includeZeroExpense = false);
        void Print(bool sort = true);

        const std::unordered_map<std::string, double>& GetPrices() const { return m_Prices; }
        Option GetOption() const { return m_Option; }
        int GetUnit() const { return m_Unit; }

    private:
        void GenerateCondition(db::Condition& cond, Option option, const std::string& month, const std::string& year) const;

        std::unordered_map<std::string, double>         m_Prices;
        Option                                          m_Option;
        int                                             m_Unit;
    };

}