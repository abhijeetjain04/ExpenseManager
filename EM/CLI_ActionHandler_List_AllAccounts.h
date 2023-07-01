#pragma once

#include "CLI_ActionHandler_List.h"

namespace db
{
	class ConditionGroup;
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
		virtual em::action_handler::ResultSPtr ProcessDBTable(const db::ConditionGroup& condGroup, const db::Clause_OrderBy& orderBy) override;

	};

}

