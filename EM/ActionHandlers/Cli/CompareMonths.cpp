#include "../../pch.h"
#include "CompareMonths.h"
#include "../../ReportHandler.h"
#include "../../Renderer_TextTable.h"

namespace em::action_handler::cli
{
    em::action_handler::ResultSPtr CompareMonths::Execute(
        const std::string& commandName,
        const std::unordered_set<std::string>& flags,
        const std::map<std::string, std::string>& options)
    {
        assert(commandName == "compareMonths");

        std::string month1 = options.at("month1");
        std::string month2 = options.at("month2");

        printf("\n comparing %s, %s", month1.c_str(), month2.c_str());
        ReportHandler report1;
        em::utils::FixMonthName(month1);
        report1.GenerateReport(ReportHandler::MONTH, month1);

        ReportHandler report2;
        em::utils::FixMonthName(month2);
        report2.GenerateReport(ReportHandler::MONTH, month2);

        Renderer_CompareReport::Render(report1, report2);

        return Result::Create(StatusCode::Success);
    }
}