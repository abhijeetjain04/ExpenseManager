#pragma once

#include "DBHandler/Database_SQLite.h"
#include "DBTable_Expense.h"
#include "Exceptions/Database/Common.h"

#define databaseMgr (*em::DatabaseManager::GetInstance())

BEGIN_NAMESPACE_EM

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
	static void Create(const char* dbName, bool useAllAccounts = false, int openMode = db::OPEN_CREATE | db::OPEN_READWRITE);

	/**
	* Returns flag used to determine if we are considering all accounts for DB queries.
	*/
	bool IsUsingAllAccounts() const;

	/**
	* This function is used to set the expense table that will be used to perform the actions on.
	*
	* In case all accounts are to be queried, m_ExpenseTable will be a nullptr.
	*
	* @template T
	*		child of DBTable_Expense, that  will be used to perform DB actions.
	*/
	template<typename T>
	void RegisterTables()
	{
		m_ExpenseTable = m_Database->CreateTable<T>();
	}

	template<typename T>
	void RegisterTable(const std::string& tablename)
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
	*/
	template<>
	std::shared_ptr<DBTable_Expense> GetTable() const
	{
		DBG_ASSERT(m_ExpenseTables.size() == 1);
		return m_ExpenseTables.begin()->second;
	}

	const Map_DBTables& GetExpenseTables() const
	{
		return m_ExpenseTables;
	}

	/**
	* Getter for the singleton instance.
	*/
	static DatabaseManager* GetInstance();

private:
	DatabaseManager(const char* dbName, bool useAllAccounts, int openMode);
	DatabaseManager(const DatabaseManager&) = default;
	DatabaseManager(DatabaseManager&&) = default;

	std::unique_ptr<db::Database_SQLite> m_Database;
	std::shared_ptr<DBTable_Expense> m_ExpenseTable;
	Map_DBTables m_ExpenseTables;
	bool m_UseAllAccounts;
		
	static std::mutex s_Mutex;
	static DatabaseManager* s_Instance;

	/* Flag to keep track if the DatabaseManager instance is initialized. */
	static bool s_IsInitialized;

};


END_NAMESPACE_EM