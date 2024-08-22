#pragma once

#include "List.h"

namespace db
{
	class Condition;
	class Clause_OrderBy;
}

namespace em::action_handler::cli
{

	class List_AllAccounts : public em::action_handler::cli::List
	{
		using base = em::action_handler::cli::List;

	protected:

		/**
		* Same as em::action_handler::cli::List
		*/
		virtual em::action_handler::ResultSPtr ProcessDBTable(
			const db::Condition& cond, 
			const db::Clause_OrderBy& orderBy, 
			bool showTags = false, 
			bool showAccount = false,
			bool showLocation = false) override;

	private:
		/**
		* Helper Function. As tableName in database is stored as '{accountName}_expense', this function will retrieve only the accountName part of the tableName.
		* 
		* @params [in] tableName
		*		Name of the table in the database.
		*/
		std::string RetrieveMatchingAccountName(const std::string& tableName) const;
	};

}

