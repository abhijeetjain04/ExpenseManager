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

/**
* List of supported commands by the application.
*/
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

/**
* Singleton class to execute the commands for cli.
*/
class ActionImplementor
{
public:
    StatusCode PerformAction(CmdType cmdType);

    /**
    * Registers the action handlers for all the supoprted commands.
    */
    void Initialize();

    /**
    * Function used to register custom handlers for CmdType.
    */
    template<typename Handler>
    ActionImplementor& RegisterHandler(CmdType cmdType);

    /* static function to get the Singleton Instance. */
    static ActionImplementor* GetInstance();
private:
    ActionImplementor();
    ActionImplementor(const ActionImplementor&) = default;

    /**
    * Displays help for the cli commands.
    */
    StatusCode DisplayHelp();

    /**
    * Will be moved in the future.
    */
    StatusCode ActionHandler_CompareMonth();

    /**
    * Helper functon to get the corresponding ActionHandler.
    * 
    * @params [in] type
    *       Type of command to get the action handler for.
    */
    em::action_handler::Interface* GetActionHandler(CmdType type) { return m_ActionHandlers[type]; }

private:

    /* DataStructure to manage the ActionHandlers for all the commands. */
    std::unordered_map<CmdType, em::action_handler::Interface*>  m_ActionHandlers;

    /* Single Instance. */
    static ActionImplementor* s_Instance;
};

template<typename Handler>
ActionImplementor& ActionImplementor::RegisterHandler(CmdType cmdType)
{
    m_ActionHandlers[cmdType] = new Handler();
    return *this;
}

END_NAMESPACE_EM