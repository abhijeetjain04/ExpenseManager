#include "pch.h"
#include "ActionImplementor.h"
#include "CLIParser/CLIParser.h"
#include "CLIParser/ValidCommand.h"
#include "DBTable_Category.h"
#include "CLIParser/Utils.h"
#include "CLI_ActionHandlers.h"
#include "DatabaseManager.h"
#include "ConfigManager.h"
#include "Exception_Config.h"

#define DATABASE_FILE_NAME "expense.db"

void GetCommandString(std::string& commandStr, std::vector<std::string>& args)
{
    std::cout << ">> ";
    std::getline(std::cin, commandStr);

    size_t pos = 0;
    std::string delimiter = " ";
    std::string token;
    while ((pos = commandStr.find(delimiter)) != std::string::npos) {
        token = commandStr.substr(0, pos);
        std::cout << token << std::endl;
        args.push_back(token);
        commandStr.erase(0, pos + delimiter.length());
    }
    args.push_back(commandStr);
}

void GetDatabaseNameInput(std::string& dbName)
{
    std::cout << "Database Name: ";
    std::getline(std::cin, dbName);
}

em::CmdType GetCmdType(const char* cmdString)
{
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

void InitializeDatabase()
{
    std::string dbName;
    GetDatabaseNameInput(dbName);

    if (!em::ConfigManager::GetInstance().IsValidDatabaseName(dbName))
        throw em::Exception_Config(std::format("Invalid database Name : {}", dbName));

    std::string databaseName = dbName + "_expense.db";

    DatabaseManager::Create(databaseName.c_str());
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
            DBG_ASSERT(cmdType != em::CmdType::INVALID);

            if (actionImpl.PerformAction(cmdType) != em::StatusCode::Success)
                return -1;
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


