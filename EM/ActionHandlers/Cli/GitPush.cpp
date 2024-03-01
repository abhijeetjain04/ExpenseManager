#include "EM/pch.h"
#include "GitPush.h"
#include "git2/git2.h"

namespace em::action_handler::cli
{

    // protected
	em::action_handler::ResultSPtr GitPush::Execute(
		const std::string& commandName,
		const std::unordered_set<std::string>& flags,
		const std::map<std::string, std::string>& options)
	{
		assert(commandName == "git-push");
		assert(!"Not Implemented!");

        return em::action_handler::Result::Create(StatusCode::Success);
	}
}