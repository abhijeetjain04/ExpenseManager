#pragma once

#include "DBHandler/Database_SQLite.h"
#include "DBTable_Expense.h"
#include "Exceptions/Database/Common.h"
#include <mutex>

#define databaseMgr (em::DatabaseManager::GetInstance())

namespace em
{


	typedef std::unordered_map<std::string, std::shared_ptr<DBTable_Expense>> Map_DBTables;

	/**
	* Singleton class that deals with the lower layers handlings of the database.
	*/
	class DatabaseManager
	{
	public:

		/**
		* Creates the static instance of the DatabaseManager.
		*
		* @params [in] dbName
		*		Name of the database that DatabaseManager handles.
		*
		* @params [in] openMode
		*		Mode in which we want to open the database.
		*/
		static void Create(const char* dbName, int openMode = db::OPEN_CREATE | db::OPEN_READWRITE);

		/**
		* This function is used to Register expense tables with the Database Manager.
		* The Database Manager will only be able to deal with an expense tables, when it is registered,
		*
		* @template T
		*		Child of DBTable_Expense.
		*/
		template<typename T>
		void RegisterExpenseTable(const std::string& tablename)
		{
			if (m_ExpenseTables.find(tablename) != m_ExpenseTables.end())
				throw em::exception::TableAlreadyRegistered(tablename);

			m_ExpenseTables[tablename] = m_Database->CreateTable<T>();
		}

		/**
		* Retrieves the logical database table from the DB.
		* If the table does not exist, a new table will be creted.
		*
		* @template_param T
		*		class inherited from class DBTable, that represents the logical table in the database.
		*/
		template<typename T>
		std::shared_ptr<T> GetTable() const
		{
			return m_Database->CreateTable<T>();
		}

		/**
		* Returns the Expense Table that is currently being used.
		*
		* Specialization of th template<T> GetTable()
		*/
		template<>
		std::shared_ptr<DBTable_Expense> GetTable() const
		{
			DBG_ASSERT(m_ExpenseTables.size() == 1);
			return m_ExpenseTables.begin()->second;
		}

		/**
		* Getter for all expense tables.
		*
		* This function will be used when we are dealing with all accounts in the database.
		*/
		const Map_DBTables& GetExpenseTables() const
		{
			return m_ExpenseTables;
		}

		/**
		* Registers the expense tables based on what account is currently selected.
		*
		* @params [in] accountName
		*		Name of the account to set expense tables for.
		*/
		void RegisterExpenseTables();

		/**
		* This function is used to switch account.
		*
		* @params [in] accountName
		*		Name of the account to switch to
		*/
		void OnSwitchAccount();

		/**
		* Getter for the singleton instance.
		*/
		static DatabaseManager& GetInstance();

	private:
		DatabaseManager(const char* dbName, int openMode);
		DatabaseManager(const DatabaseManager&) = default;
		DatabaseManager(DatabaseManager&&) = default;

		std::unique_ptr<db::Database_SQLite> m_Database;
		Map_DBTables m_ExpenseTables;

		static std::mutex s_Mutex;
		static DatabaseManager* s_Instance;

		/* Flag to keep track if the DatabaseManager instance is initialized. */
		static bool s_IsInitialized;

	};

}