#pragma once

#include "DBHandler/Database_SQLite.h"
#include "ActionHandlers/IActionHandler.h"
#include "Common/CommonEnums.h"

#define CmdString_Cls           "cls"
#define CmdString_Help          "help"
#define CmdString_AddCategory   "addCategory"
#define CmdString_Add           "add"
#define CmdString_Remove        "remove"
#define CmdString_List          "list"
#define CmdString_Report        "report"
#define CmdString_CompareMonths "compareMonths"
#define CmdString_SwitchAccount "switchAccount"

#define actionImpl (em::ActionImplementor::GetInstance())

BEGIN_NAMESPACE_EM

/**
* Singleton class to execute the commands for cli.
*/
class ActionImplementor
{
public:
    /**
    * This function handle the actual processing of the command.
    * 
    * @params [in] cmdType
    *       Type of the command that will be executed
    * 
    * @returns
    *       StatusCode representing the result of the function.
    */
    StatusCode PerformAction(CmdType cmdType);

    /**
    * This function should be called when the account gets switched.
    */
    void OnAccountSwitched();

    /**
    * This creates the singleton instance.
    */
    static void Create();

    /**
    * Getter for the Singleton Instance. 
    */
    static ActionImplementor& GetInstance();
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

    /**
    */
    void Initialize();

    /**
    * Initializes the action handlers 
    */
    void InitializeActionHandlers();

    /**
    * This function is used to register custom handlers for CmdType.
    * 
    * @params [in] cmdType
    *       Command Type for which we are registering the handler.
    * 
    * @template_params Handler
    *       Child class of IActionHandler, which will handle the command.
    */
    template<typename Handler>
    ActionImplementor& RegisterHandler(CmdType cmdType);

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