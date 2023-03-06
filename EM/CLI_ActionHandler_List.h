#pragma once

#include "IActionHandler.h"

namespace em::action_handler::cli
{

	class List : public em::action_handler::Interface
	{
	public:
		/**
		* Same as em::action_handler::Interface::Execute(...)
		*/
		virtual em::action_handler::ResultSPtr Execute(
			const std::string& commandName,
			const std::unordered_set<std::string>& flags,
			const std::map<std::string, std::string>& options) override;

	private:
		/**
		* This function handles the request of listing categories
		* 
		* @return 
		*		Object of ResultSPtr that contains the result details on execution of the function.
		*/
		em::action_handler::ResultSPtr ListCategories();

	};

}

