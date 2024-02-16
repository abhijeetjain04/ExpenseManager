#pragma once

#include "../IActionHandler.h"

namespace db
{
	class Condition;
	class Clause_OrderBy;
}

namespace em::action_handler::cli
{

	class List : public em::action_handler::Interface
	{
	protected:
		/**
		* Same as em::action_handler::Interface::Execute(...)
		*/
		virtual em::action_handler::ResultSPtr Execute(
			const std::string& commandName,
			const std::unordered_set<std::string>& flags,
			const std::map<std::string, std::string>& options) override;

	protected:

		/**
		* This function retrieves the data from the Database Table, and renders it to the console.
		* 
		* @params [in] cond
		*		Conditions that will be used to filter the data.
		* 
		* @params [in] orderBy
		*		Determines ordering or the rows.
		* 
		* @return
		*		Object of ResultSPtr that contains the result details on execution of the function.
		*
		*/
		virtual em::action_handler::ResultSPtr ProcessDBTable(const db::Condition& cond, const db::Clause_OrderBy& orderBy);

		/**
		* This function handles the request of listing categories
		* 
		* @return 
		*		Object of ResultSPtr that contains the result details on execution of the function.
		*/
		em::action_handler::ResultSPtr ListCategories();

	};

}

