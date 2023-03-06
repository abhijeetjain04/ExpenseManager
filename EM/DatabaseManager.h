#pragma once

#include "DBHandler/Database_SQLite.h"

#define databaseMgr (*DatabaseManager::GetInstance())


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
	* Getter for the singleton instance.
	*/
	static DatabaseManager* GetInstance();

private:
	DatabaseManager(const char* dbName, int openMode);
	DatabaseManager(const DatabaseManager&) = default;
	DatabaseManager(DatabaseManager&&) = default;

	std::unique_ptr<db::Database_SQLite> m_Database;

	static std::mutex s_Mutex;
	static DatabaseManager* s_Instance;

	/* Flag to keep track if the DatabaseManager instance is initialized. */
	static bool s_IsInitialized;

};

