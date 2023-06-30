#include "pch.h"
#include "ActionImplementor.h"
#include "CLIParser/CLIParser.h"
#include "CLIParser/ValidCommand.h"
#include "DBTable_Category.h"
#include "DBTable_Expense.h"
#include "CLIParser/Utils.h"
#include "CLI_ActionHandlers.h"
#include "DatabaseManager.h"
#include "ConfigManager.h"
#include "Exception_Config.h"

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
}

em::CmdType GetCmdType(const char* cmdString)
{

    if (strcmp(cmdString, CmdString_Cls) == 0)
        return em::CmdType::CLS;

    if (strcmp(cmdString, CmdString_Help) == 0)
        return em::CmdType::HELP;

    if (strcmp(cmdString, CmdString_AddCategory) == 0)
        return em::CmdType::ADD_CATEGORY;

    if (strcmp(cmdString, CmdString_Add) == 0)
        return em::CmdType::ADD;

    if (strcmp(cmdString, CmdString_Remove) == 0)
        return em::CmdType::REMOVE;

    if (strcmp(cmdString, CmdString_List) == 0)
        return em::CmdType::LIST;

    if (strcmp(cmdString, CmdString_Report) == 0)
        return em::CmdType::REPORT;

    if (strcmp(cmdString, CmdString_CompareMonths) == 0)
        return em::CmdType::COMPARE_MONTH;

    return em::CmdType::INVALID;
}

em::CmdType GetCmdType(int argc, char** argv)
{
    if (argc < 2)
        return em::CmdType::INVALID;

    return GetCmdType(argv[1]);
}

em::CmdType GetCmdType(std::vector<std::string>& args)
{
    if (args.size() < 1)
        return em::CmdType::INVALID;

    return GetCmdType(args[0].c_str());
}

void InitializeCLI()
{
    cliParser.RegisterCommand(CmdString_Help);

    cliParser.RegisterCommand(CmdString_AddCategory)
        .AddParameter("name",   { cli::OptionType::ALPHA_NUMERIC, "Name of the Category",   true,   1});

    cliParser.RegisterCommand(CmdString_Add)
        .AddParameter("name",       { cli::OptionType::TEXT,            "Name of the Expense.",             true,   1})
        .AddParameter("category",   { cli::OptionType::ALPHA_NUMERIC,   "Category of the Expense.",         true,   2})
        .AddParameter("price",      { cli::OptionType::DOUBLE,          "Price of the Expense.",            true,   3})
        .AddParameter("date",       { cli::OptionType::DATE,            "Custom Date to add the Expense." })
        .AddParameter("location",   { cli::OptionType::TEXT,            "Custom Location to add the Expense." });

    cliParser.RegisterCommand(CmdString_List)
        .AddParameter("name",       { cli::OptionType::TEXT,            "Filters by Regex for Name." })
        .AddParameter("category",   { cli::OptionType::ALPHA_NUMERIC,   "Filters by Category."})
        .AddParameter("date",       { cli::OptionType::DATE,            "Filters by Specific Date."})
        .AddParameter("month",      { cli::OptionType::INTEGER,         "Filters by Month."})
        .AddParameter("year",       { cli::OptionType::INTEGER,         "Filters by Year."})
        .AddParameter("location",   { cli::OptionType::TEXT,            "Filters by Location." })
        .AddFlag("categories",  "Lists all the available categories.")
        .AddFlag("today",       "Lists today's Expenses.")
        .AddFlag("thisMonth",   "Lists this Month's Expenses.")
        .AddFlag("thisYear",    "Lists this Year's Expenses.")
        .AddFlag("ascending",   "Lists in ascending order of date.");

    cliParser.RegisterCommand(CmdString_Remove)
        .AddParameter("row_id", {cli::OptionType::INTEGER, "", true, 1});

    cliParser.RegisterCommand(CmdString_Report)
        .AddParameter("month",  { cli::OptionType::INTEGER, "Custom Month's Report."})
        .AddParameter("year",   { cli::OptionType::INTEGER, "Custom Year's Report." })
        .AddFlag("today",       "Today's Report.")
        .AddFlag("thisMonth",   "This Month's Report.")
        .AddFlag("thisYear",    "This Year's Report.");

    cliParser.RegisterCommand(CmdString_CompareMonths)
        .AddParameter("month1", { cli::OptionType::INTEGER, "First Month to compare",   true,   1})
        .AddParameter("month2", { cli::OptionType::INTEGER, "Second Month to compare",  true,   2 });
}

void InitializeActionImplementor()
{
    actionImpl.Initialize();
}

void SetCurrentExpenseTable(const std::string& tableName)
{
    if (tableName == "personal_expense")
    {
        em::DatabaseManager::GetInstance()->SetCurrentExpenseTable<em::DBTable_PersonalExpense>();
        return;
    }
    
    if (tableName == "household_expense")
    {
        em::DatabaseManager::GetInstance()->SetCurrentExpenseTable<em::DBTable_HouseholdExpense>();
        return;
    }

    if (tableName == "marriage_expense")
    {
        em::DatabaseManager::GetInstance()->SetCurrentExpenseTable<em::DBTable_MarriageExpense>();
        return;
    }

    DBG_ASSERT(!"Invalid Table Name");
}

void InitializeDatabase()
{
    // Account name is used as the table name to store in DB.
    std::string tableName;
    GetAccountNameInput(tableName);

    if (!em::ConfigManager::GetInstance().IsValidAccountName(tableName))
        throw em::Exception_Config(std::format("Invalid table Name : {}", tableName));

    std::string fullTableName = tableName + "_expense";

    em::DatabaseManager::Create(DATABASE_FILE_NAME);
    SetCurrentExpenseTable(fullTableName);
}

void Initialize()
{
    InitializeDatabase();
    InitializeCLI();
    InitializeActionImplementor();
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
            if (cmdType == em::CmdType::INVALID)
            {
                printf("\nInvalid Command: %s", args[0].c_str());
                continue;
            }

            if (actionImpl.PerformAction(cmdType) != em::StatusCode::Success)
                printf("\nFailed to execute command!");

            printf("\n=============================================================");
        }
    }
    catch (std::exception& e)
    {
        printf(e.what());
    }
    catch (...)
    {
        printf("Unhandled Exception!");
    }

    return 0;
}

