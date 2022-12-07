#pragma once

namespace db { class Database_SQLite; }


class ReportHandler
{
public:

    enum Option
    {
        ALL,
        TODAY,
        THIS_MONTH,
        THIS_YEAR,
        MONTH
    };

    ReportHandler(std::unique_ptr<db::Database_SQLite>& database)
        : m_Database(database)
    {
    }

    ReportHandler(std::unique_ptr<db::Database_SQLite>& database, Option option, int unit)
        : m_Database(database)
        , m_Option(option)
        , m_Unit(unit)
    {
        GenerateReport(option, unit);
    }


    ErrorCode GenerateReport(Option option = Option::ALL, int month = -1);
    void Print(bool sort = true);

    const std::vector<std::pair<std::string, double>>& GetPrices() const { return m_Prices; }
    Option GetOption() const { return m_Option; }
    int GetUnit() const { return m_Unit; }
    

private:
    std::unique_ptr<db::Database_SQLite>&           m_Database;
    std::vector<std::pair<std::string, double>>     m_Prices;
    Option                                          m_Option;
    int                                             m_Unit;
};


