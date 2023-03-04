#pragma once

#include "IActionHandler.h"

namespace em::action_handler::cli
{

	class Report : public em::action_handler::Interface
	{
	public:
		virtual em::action_handler::ResultSPtr Execute(
			const std::string& commandName,
			const std::unordered_set<std::string>& flags,
			const std::map<std::string, std::string>& options) override;

	};

}

