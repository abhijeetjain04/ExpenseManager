#include "../../pch.h"
#include "CompareMonths.h"
#include "../../ReportHandler.h"
#include "../../Renderer_TextTable.h"
#include "DBHandler/Util.h"
#include "EM/DatabaseManager.h"
#include "EM/Conditions.h"
#include "EM/ReportHandler.h"

#include <numeric>

namespace em::action_handler::cli
{
    em::action_handler::ResultSPtr CompareMonths::Execute(
        const std::string& commandName,
        const std::unordered_set<std::string>& flags,
        const std::map<std::string, std::string>& options)
    {
        assert(commandName == "compareMonths");

        if (flags.contains("thisYear"))
            return CompareAllMonths(db::util::GetThisYear());

        if (options.contains("year"))
            return CompareAllMonths(options.at("year"));
        
        if (!options.contains("month1") || !options.contains("month2"))
            return Result::Create(StatusCode::DisplayHelp);

        std::string month1 = options.at("month1");
        std::string month2 = options.at("month2");

        printf("\n Comparing %s, %s", month1.c_str(), month2.c_str());
        ReportHandler report1;
        em::utils::FixMonthName(month1);
        report1.GenerateReport(ReportHandler::MONTH, month1);

        ReportHandler report2;
        em::utils::FixMonthName(month2);
        report2.GenerateReport(ReportHandler::MONTH, month2, "-1", true);

        Renderer_CompareReport::Render({ report1, report2 });

        return Result::Create(StatusCode::Success);
    }

    em::action_handler::ResultSPtr CompareMonths::CompareAllMonths(const std::string& year)
    {
        std::vector<ReportHandler> reports;
        for (int month = 1; month <= 12; ++month)
        {
            std::string monthAsString = std::to_string(month);
            em::utils::FixMonthName(monthAsString);

            ReportHandler report;
            report.GenerateReport(ReportHandler::MONTH_AND_YEAR, monthAsString, year, true);
            if(report.GetPrices().size() > 0)
                reports.push_back(std::move(report));
        }

        Renderer_CompareReport::Render(reports);

        return Result::Create(StatusCode::Success);
    }
}