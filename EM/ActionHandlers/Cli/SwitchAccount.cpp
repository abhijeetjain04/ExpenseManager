#include "EM/pch.h"
#include "SwitchAccount.h"
#include "../../Account/Manager.h"
#include "../../DatabaseManager.h"

namespace em::action_handler::cli
{
	em::action_handler::ResultSPtr SwitchAccount::Execute(
		const std::string& commandName,
		const std::unordered_set<std::string>& flags,
		const std::map<std::string, std::string>& options)
	{
		DBG_ASSERT(commandName == "switchAccount");

		const std::string& newAccountName = options.at("accountName");
		em::account::Manager::GetInstance().SwitchAccount(newAccountName);

		em::DatabaseManager::GetInstance()->OnSwitchAccount();

		printf("\nAccount Switched to : %s", newAccountName.c_str());
		return em::action_handler::Result::Create(StatusCode::Success);
	}
}
