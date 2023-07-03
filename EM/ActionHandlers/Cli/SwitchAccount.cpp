#include "EM/pch.h"
#include "SwitchAccount.h"
#include "../../Account/Manager.h"
#include "../../DatabaseManager.h"
#include "../../ActionImplementor.h"

namespace em::action_handler::cli
{
	em::action_handler::ResultSPtr SwitchAccount::Execute(
		const std::string& commandName,
		const std::unordered_set<std::string>& flags,
		const std::map<std::string, std::string>& options)
	{
		DBG_ASSERT(commandName == "switchAccount");

		const std::string& newAccountName = options.at("accountName");
		StatusCode switchStatus = em::account::Manager::GetInstance().SwitchAccount(newAccountName);

		// handle if account not switched.
		if (switchStatus != StatusCode::Success)
		{
			switch (switchStatus)
			{
			case StatusCode::AccountDoesNotExist:
				printf("\nAccount does not exist: %s", newAccountName.c_str());
			case StatusCode::AccountAlreadySelected:
				printf("\nAccount %s is already selected.", newAccountName.c_str());
			}

			return em::action_handler::Result::Create(switchStatus);
		}

		em::DatabaseManager::GetInstance().OnSwitchAccount();

		em::ActionImplementor::GetInstance().OnAccountSwitched();

		printf("\nAccount Switched to : %s", newAccountName.c_str());
		return em::action_handler::Result::Create(StatusCode::Success);
	}
}
