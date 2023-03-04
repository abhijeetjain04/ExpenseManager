#pragma once

#include "DBHandler/Database_SQLite.h"

#define databaseMgr (*DatabaseManager::GetInstance())

class DatabaseManager
{
public:

	static void Create(const char* dbName, int openMode = db::OPEN_CREATE | db::OPEN_READWRITE);

	template<typename T>
	std::shared_ptr<T> GetTable() const
	{
		return m_Database->CreateTable<T>();
	}

	static DatabaseManager* GetInstance();

private:
	DatabaseManager(const char* dbName, int openMode);
	DatabaseManager(const DatabaseManager&) = default;
	DatabaseManager(DatabaseManager&&) = default;

	std::unique_ptr<db::Database_SQLite> m_Database;

	static std::mutex s_Mutex;
	static DatabaseManager* s_Instance;
	static bool s_IsInitialized;


};

