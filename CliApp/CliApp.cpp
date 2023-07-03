#include "pch.h"
#include "EM/ActionImplementor.h"
#include "CLIParser/CLIParser.h"
#include "CLIParser/ValidCommand.h"
#include "EM/DBTable_Category.h"
#include "EM/DBTable_Expense.h"
#include "CLIParser/Utils.h"
#include "EM/ActionHandlers/Cli/ActionHandlers.h"
#include "EM/DatabaseManager.h"
#include "EM/ConfigManager.h"
#include "EM/Exceptions/Config.h"
#include "EM/Common/EnumAndStringConverter.h"
#include "EM/Account/Account.h"
#include "EM/Account/Manager.h"

#define DATABASE_FILE_NAME "expense.db"

void GetCommandString(std::string& commandStr, std::vector<std::string>& args)
{
    printf("\n\n>> ");
    std::getline(std::cin, commandStr);

    std::istringstream iss(commandStr);
    std::vector<std::string> v;
    std::string s;
    while (iss >> std::quoted(s))
        args.push_back(s);
}

void GetAccountNameInput(std::string& accountName)
{
    std::cout << "Account: ";
    std::getline(std::cin, accountName);
    accountName.erase(accountName.find_last_not_of(" ") + 1);
}

em::CmdType GetCmdType(const char* cmdString)
{

    if (strcmp(cmdString, CmdString_Cls) == 0)
        return em::CmdType::ClearScreen;

    if (strcmp(cmdString, CmdString_Help) == 0)
        return em::CmdType::Help;

    if (strcmp(cmdString, CmdString_AddCategory) == 0)
        return em::CmdType::AddCategory;

    if (strcmp(cmdString, CmdString_Add) == 0)
        return em::CmdType::Add;

    if (strcmp(cmdString, CmdString_Remove) == 0)
        return em::CmdType::Remove;

    if (strcmp(cmdString, CmdString_List) == 0)
        return em::CmdType::List;

    if (strcmp(cmdString, CmdString_Report) == 0)
        return em::CmdType::Report;

    if (strcmp(cmdString, CmdString_CompareMonths) == 0)
        return em::CmdType::CompareMonths;

    if (strcmp(cmdString, CmdString_SwitchAccount) == 0)
        return em::CmdType::SwitchAccount;

    return em::CmdType::Invalid;
}

em::CmdType GetCmdType(int argc, char** argv)
{
    if (argc < 2)
        return em::CmdType::Invalid;

    return GetCmdType(argv[1]);
}

em::CmdType GetCmdType(std::vector<std::string>& args)
{
    if (args.size() < 1)
        return em::CmdType::Invalid;

    return GetCmdType(args[0].c_str());
}

void InitializeCLI()
{
    cliParser.RegisterCommand(CmdString_Help);

    cliParser.RegisterCommand(CmdString_AddCategory)
        .AddParameter("name", { cli::OptionType::ALPHA_NUMERIC, "Name of the Category",   true,   1 });

    cliParser.RegisterCommand(CmdString_Add)
        .AddParameter("name", { cli::OptionType::TEXT,            "Name of the Expense.",             true,   1 })
        .AddParameter("category", { cli::OptionType::ALPHA_NUMERIC,   "Category of the Expense.",         true,   2 })
        .AddParameter("price", { cli::OptionType::DOUBLE,          "Price of the Expense.",            true,   3 })
        .AddParameter("date", { cli::OptionType::DATE,            "Custom Date to add the Expense." })
        .AddParameter("location", { cli::OptionType::TEXT,            "Custom Location to add the Expense." });

    cliParser.RegisterCommand(CmdString_List)
        .AddParameter("name",       { cli::OptionType::TEXT,                  "Filters by Regex for Name." })
        .AddParameter("category",   { cli::OptionType::ALPHA_NUMERIC,     "Filters by Category." })
        .AddParameter("date",       { cli::OptionType::DATE,                  "Filters by Specific Date." })
        .AddParameter("month",      { cli::OptionType::INTEGER,              "Filters by Month." })
        .AddParameter("year",       { cli::OptionType::INTEGER,               "Filters by Year." })
        .AddParameter("location",   { cli::OptionType::TEXT,              "Filters by Location." })
        .AddFlag("categories",  "Lists all the available categories.")
        .AddFlag("today",       "Lists today's Expenses.")
        .AddFlag("thisMonth",   "Lists this Month's Expenses.")
        .AddFlag("thisYear",    "Lists this Year's Expenses.")
        .AddFlag("ascending",   "Lists in ascending order of date.");

    cliParser.RegisterCommand(CmdString_Remove)
        .AddParameter("row_id", { cli::OptionType::INTEGER, "", true, 1 });

    cliParser.RegisterCommand(CmdString_Report)
        .AddParameter("month", { cli::OptionType::INTEGER, "Custom Month's Report." })
        .AddParameter("year", { cli::OptionType::INTEGER, "Custom Year's Report." })
        .AddFlag("today", "Today's Report.")
        .AddFlag("thisMonth", "This Month's Report.")
        .AddFlag("thisYear", "This Year's Report.");

    cliParser.RegisterCommand(CmdString_CompareMonths)
        .AddParameter("month1", { cli::OptionType::INTEGER, "First Month to compare",   true,   1 })
        .AddParameter("month2", { cli::OptionType::INTEGER, "Second Month to compare",  true,   2 });

    cliParser.RegisterCommand(CmdString_SwitchAccount)
        .AddParameter("accountName", { cli::OptionType::TEXT, "Name of the account to switch to.",   true,   1 });

}

void InitializeActionImplementor()
{
    if (em::account::Manager::GetInstance().IsUsingAllAccounts())
    {
        actionImpl.RegisterHandler<em::action_handler::cli::List_AllAccounts>(em::CmdType::List);
    }
    else
    {
        actionImpl.RegisterHandler<em::action_handler::cli::List>(em::CmdType::List);
        actionImpl.RegisterHandler<em::action_handler::cli::Add>(em::CmdType::Add);
        actionImpl.RegisterHandler<em::action_handler::cli::Remove>(em::CmdType::Remove);
        actionImpl.RegisterHandler<em::action_handler::cli::Report>(em::CmdType::Report);
    }

    actionImpl.RegisterHandler<em::action_handler::cli::SwitchAccount>(em::CmdType::SwitchAccount);
    actionImpl.RegisterHandler<em::action_handler::cli::AddCategory>(em::CmdType::AddCategory);
}

void InitializeDatabase()
{
    std::string accountName;
    GetAccountNameInput(accountName);

    // Account name is used as the table name to store in DB.
    const std::string& tableName = accountName;
    bool isTableName = tableName != "all";

    if (isTableName)
    {
        if (!em::account::Manager::GetInstance().AccountExists(accountName))
            throw em::exception::Config(std::format("Invalid account : {}", accountName));
    }

    em::account::Manager::GetInstance().SetCurrentAccountName(accountName);

    em::DatabaseManager::Create(DATABASE_FILE_NAME);
    em::DatabaseManager::GetInstance()->RegisterExpenseTables();
}

void InitializeAccountManager()
{
    em::account::Manager::Create();
}

void Initialize()
{
    InitializeAccountManager();
    InitializeDatabase();
    InitializeCLI();
    InitializeActionImplementor();
}

void HandleErrorStatusCode(em::CmdType cmdType, em::StatusCode code)
{
    const std::string& cmdTypStr = em::common::EnumAndStringConverter::ConvertCmdTypeEnumToString(cmdType);
    const std::string& accountName = em::account::Manager::GetInstance().GetCurrentAccount()->GetName();

    if(code == em::StatusCode::CommandDoesNotExist)
    {
        printf("%s", std::format("\nCommand: '{}' is not supported for account: '{}'", cmdTypStr, accountName).c_str());
    }
}

int main(int argc, char** argv)
{
    try
    {
        Initialize();

        bool quit = false;
        while (!quit)
        {
            std::string commandStr;
            std::vector<std::string> args;
            GetCommandString(commandStr, args);
            if (commandStr.empty() || commandStr == "quit")
                break;

            cliParser.Parse(args);

            em::CmdType cmdType = GetCmdType(args);
            if (cmdType == em::CmdType::Invalid)
            {
                printf("\nInvalid Command: %s", args[0].c_str());
                continue;
            }

            em::StatusCode status = actionImpl.PerformAction(cmdType);
            if (status != em::StatusCode::Success)
            {
                printf("\nFailed to execute command!");
                HandleErrorStatusCode(cmdType, status);
            }

            printf("\n=============================================================");
        }
    }
    catch (std::exception& e)
    {
        printf("\n%s", e.what());
    }
    catch (...)
    {
        printf("\nUnhandled Exception!");
    }

    printf("\n\n");
    std::system("pause");

    return 0;
}

