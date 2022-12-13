#include "pch.h"
#include "ActionImplementor.h"
#include "CLIParser/CLIParser.h"
#include "CLIParser/ValidCommand.h"
#include "DBTable_Category.h"
#include "CLIParser/Utils.h"

#define DATABASE_FILE_NAME "expense.db"


CmdType GetCmdType(const char* cmdString)
{
    if (strcmp(cmdString, CmdString_Help) == 0)
        return CmdType::HELP;

    if (strcmp(cmdString, CmdString_AddCategory) == 0)
        return CmdType::ADD_CATEGORY;

    if (strcmp(cmdString, CmdString_Add) == 0)
        return CmdType::ADD;

    if (strcmp(cmdString, CmdString_Remove) == 0)
        return CmdType::REMOVE;

    if (strcmp(cmdString, CmdString_List) == 0)
        return CmdType::LIST;

    if (strcmp(cmdString, CmdString_Report) == 0)
        return CmdType::REPORT;

    if (strcmp(cmdString, CmdString_CompareMonths) == 0)
        return CmdType::COMPARE_MONTH;

    return CmdType::INVALID;
}

CmdType GetCmdType(int argc, char** argv)
{
    if (argc < 2)
        return CmdType::INVALID;

    return GetCmdType(argv[1]);
}

void InitializeCLI()
{
    cliParser.SetHelperMessage("Helper Message");

    cliParser.RegisterCommand(CmdString_Help);

    cliParser.RegisterCommand(CmdString_AddCategory)
        .AddParameter("name",   { cli::OptionType::ALPHA_NUMERIC, "Name of the Category",   true,   1});

    cliParser.RegisterCommand(CmdString_Add)
        .AddParameter("name",       { cli::OptionType::TEXT,            "Name of the Expense.",             true,   1})
        .AddParameter("category",   { cli::OptionType::ALPHA_NUMERIC,   "Category of the Expense.",         true,   2})
        .AddParameter("price",      { cli::OptionType::DOUBLE,          "Price of the Expense.",            true,   3})
        .AddParameter("date",       { cli::OptionType::DATE,            "Custom Date to add the Expense." });

    cliParser.RegisterCommand(CmdString_List)
        .AddParameter("name",       { cli::OptionType::TEXT,            "Filters by Regex for Name." })
        .AddParameter("category",   { cli::OptionType::ALPHA_NUMERIC,   "Filters by Category."})
        .AddParameter("date",       { cli::OptionType::DATE,            "Filters by Specific Date."})
        .AddParameter("month",      { cli::OptionType::INTEGER,         "Filters by Month."})
        .AddParameter("year",       { cli::OptionType::INTEGER,         "Filters by Year."})
        .AddFlag("categories",  "Lists all the available categories.")
        .AddFlag("today",       "Lists today's Expenses.")
        .AddFlag("thisMonth",   "Lists this Month's Expenses.")
        .AddFlag("thisYear",    "Lists this Year's Expenses.")
        .AddFlag("ascending",   "Lists in ascending order of date.");

    cliParser.RegisterCommand(CmdString_Remove)
        .AddParameter("row_id", {cli::OptionType::INTEGER, "", true, 1});

    cliParser.RegisterCommand(CmdString_Report)
        .AddParameter("month",  { cli::OptionType::INTEGER, "Custom Month's Report."})
        .AddFlag("today",       "Today's Report.")
        .AddFlag("thisMonth",   "This Month's Report.")
        .AddFlag("thisYear",    "This Year's Report.");

    cliParser.RegisterCommand(CmdString_CompareMonths)
        .AddParameter("month1", { cli::OptionType::INTEGER, "First Month to compare",   true,   1})
        .AddParameter("month2", { cli::OptionType::INTEGER, "Second Month to compare",  true,   2 });
}


void InitializeActionImplementor()
{
    actionImpl.Initialize(DATABASE_FILE_NAME);
}

void Initialize()
{
    InitializeCLI();
    InitializeActionImplementor();
}


int main(int argc, char** argv)
{
    cliParser.SetExeName(argv[0]);

    Initialize();

    if (!cliParser.Parse(argv, argc))
    {
        ERROR_LOG(ERROR_CLI_PARSING);
        return -1;
    }

    CmdType cmdType = GetCmdType(argc, argv);
    DBG_ASSERT(cmdType != CmdType::INVALID);

    if (actionImpl.PerformAction(cmdType) != ErrorCode::Success)
        return -1;

    return 0;
}


