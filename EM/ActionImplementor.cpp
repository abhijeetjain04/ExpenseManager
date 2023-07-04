#include "pch.h"
#include "ActionImplementor.h"
#include "Account/Manager.h"
#include "Account/Account.h"
#include "Common/CommonEnums.h"
#include "Common/EnumAndStringConverter.h"
#include "CLIParser/CLIParser.h"
#include "DBHandler/Util.h"
#include "DBTable_Expense.h"
#include "DBTable_Category.h"
#include "ActionHandlers/Cli/ActionHandlers.h"
#include "TextTable.h"
#include "Conditions.h"
#include "ReportHandler.h"
#include "Renderer_TextTable.h"
#include "ErrorMessages.h"

namespace em
{


    ActionImplementor* ActionImplementor::s_Instance = nullptr;

    // private
    ActionImplementor::ActionImplementor()
    {
    }

    // private
    void ActionImplementor::Initialize()
    {
        InitializeActionHandlers();
    }

    // private
    void ActionImplementor::InitializeActionHandlers()
    {
        // clear the action handlers before assiging them again,
        // as the same command can have different action handlers for different accounts.
        m_ActionHandlers.clear();

        if (em::account::Manager::GetInstance().IsUsingAllAccounts())
        {
            RegisterHandler<em::action_handler::cli::List_AllAccounts>(em::CmdType::List);
        }
        else
        {
            RegisterHandler<em::action_handler::cli::List>(em::CmdType::List);
            RegisterHandler<em::action_handler::cli::Add>(em::CmdType::Add);
            RegisterHandler<em::action_handler::cli::Remove>(em::CmdType::Remove);
        }

        RegisterHandler<em::action_handler::cli::Report>(em::CmdType::Report);
        RegisterHandler<em::action_handler::cli::SwitchAccount>(em::CmdType::SwitchAccount);
        RegisterHandler<em::action_handler::cli::AddCategory>(em::CmdType::AddCategory);
    }

    // public
    void ActionImplementor::OnAccountSwitched()
    {
        InitializeActionHandlers();
    }

    //public
    StatusCode ActionImplementor::PerformAction(CmdType cmdType)
    {
        if (cmdType == CmdType::ClearScreen)
        {
            system("cls");
            return StatusCode::Success;
        }

        if (cmdType == CmdType::Help)
            return DisplayHelp();

        auto actionHandler = GetActionHandler(cmdType);
        if (!actionHandler)
            return StatusCode::CommandDoesNotExist;

        em::action_handler::ResultSPtr result = actionHandler->Execute(cliParser.AsJson());
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

    // public static
    void ActionImplementor::Create()
    {
        DBG_ASSERT(s_Instance == nullptr);
        s_Instance = new ActionImplementor();
        s_Instance->Initialize();
    }

    // public static
    ActionImplementor& ActionImplementor::GetInstance()
    {
        DBG_ASSERT(s_Instance != nullptr);
        return *s_Instance;
    }


}