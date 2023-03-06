#include "pch.h"
#include "ActionImplementor.h"
#include "CLIParser/CLIParser.h"
#include "DBHandler/Util.h"
#include "DBTable_Expense.h"
#include "DBTable_Category.h"
#include "CLI_ActionHandlers.h"
#include "TextTable.h"
#include "Conditions.h"
#include "ReportHandler.h"
#include "Renderer_TextTable.h"
#include "ErrorMessages.h"

BEGIN_NAMESPACE_EM

ActionImplementor* ActionImplementor::s_Instance = nullptr;

// private
ActionImplementor::ActionImplementor()
{
}

// public
ActionImplementor* ActionImplementor::GetInstance()
{
    if (s_Instance == nullptr)
        s_Instance = new ActionImplementor();

    return s_Instance;
}

// public
void ActionImplementor::Initialize()
{
    RegisterHandler<em::action_handler::cli::List>(em::CmdType::LIST);
    RegisterHandler<em::action_handler::cli::Add>(em::CmdType::ADD);
    RegisterHandler<em::action_handler::cli::Remove>(em::CmdType::REMOVE);
    RegisterHandler<em::action_handler::cli::AddCategory>(em::CmdType::ADD_CATEGORY);
    RegisterHandler<em::action_handler::cli::Report>(em::CmdType::REPORT);
}

//public
StatusCode ActionImplementor::PerformAction(CmdType cmdType)
{
    if (cmdType == CmdType::HELP)
        return DisplayHelp();

    auto actionHandler = GetActionHandler(cmdType);
    DBG_ASSERT(actionHandler || !"Action handler does not exist for this cmdType!");
    if (!actionHandler)
        return StatusCode::CommandDoesNotExist;

    em::action_handler::ResultSPtr result = actionHandler->Execute(cliParser.AsJson());
    if (result->statusCode != StatusCode::Success)
    {
        ERROR_LOG(ERROR_ACTIONHANDLER_EXECUTE, result->message);
        DBG_ASSERT(false);
    }

    return result->statusCode;
}

// private
StatusCode ActionImplementor::DisplayHelp()
{
    cliParser.DisplayHelp();
    return StatusCode::Success;
}

// private
StatusCode ActionImplementor::ActionHandler_CompareMonth()
{
    std::string month1 = cliParser.GetParam("month1").AsString();

    std::string month2 = cliParser.GetParam("month2").AsString();

    printf("\n comparing %s, %s", month1.c_str(), month2.c_str());
    ReportHandler report1;
    report1.GenerateReport(ReportHandler::MONTH, month1);

    ReportHandler report2;
    report2.GenerateReport(ReportHandler::MONTH, month2);

    Renderer_CompareReport::Render(report1, report2);

    return StatusCode::Success;
}

END_NAMESPACE_EM