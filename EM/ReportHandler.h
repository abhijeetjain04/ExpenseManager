#pragma once

namespace db { class Database_SQLite; }

BEGIN_NAMESPACE_EM

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

    ReportHandler(std::unique_ptr<db::Database_SQLite>& database)
        : m_Database(database)
    {
    }

    ErrorCode GenerateReport(Option option = Option::ALL, const std::string& month = "-1", const std::string& year = "-1");
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


END_NAMESPACE_EM