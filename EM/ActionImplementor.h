#pragma once

#include "DBHandler/Database_SQLite.h"
#include "IActionHandler.h"

#define CmdString_Help          "help"
#define CmdString_AddCategory   "addCategory"
#define CmdString_Add           "add"
#define CmdString_Remove        "remove"
#define CmdString_List          "list"
#define CmdString_Report        "report"
#define CmdString_CompareMonths "compareMonths"

#define actionImpl (*em::ActionImplementor::GetInstance())

BEGIN_NAMESPACE_EM

enum class CmdType
{
    HELP,
    ADD_CATEGORY,
    ADD,
    REMOVE,
    LIST,
    REPORT,
    COMPARE_MONTH,

    INVALID
};

// singleton
class ActionImplementor
{
public:
    ErrorCode PerformAction(CmdType cmdType);

    template<typename Handler>
    ActionImplementor& RegisterHandler(CmdType cmdType);

    static ActionImplementor* GetInstance();
private:
    ActionImplementor();
    ActionImplementor(const ActionImplementor&) = default;

    ErrorCode DisplayHelp();
    ErrorCode ActionHandler_CompareMonth();

    em::action_handler::Interface* GetActionHandler(CmdType type) { return m_ActionHandlers[type]; }

private:
    std::unordered_map<CmdType, em::action_handler::Interface*>  m_ActionHandlers;

    static ActionImplementor* s_Instance;
};

template<typename Handler>
ActionImplementor& ActionImplementor::RegisterHandler(CmdType cmdType)
{
    m_ActionHandlers[cmdType] = new Handler();
    return *this;
}

END_NAMESPACE_EM