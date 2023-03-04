#include "pch.h"
#include "DatabaseManager.h"

// private
DatabaseManager* DatabaseManager::s_Instance = nullptr;
bool DatabaseManager::s_IsInitialized = false;
std::mutex DatabaseManager::s_Mutex;

// public
DatabaseManager* DatabaseManager::GetInstance()
{
	if (!s_IsInitialized)
		DBG_ASSERT(!"DatabaseManager Not Initialized yet!");
	return s_Instance;
}

// public
DatabaseManager::DatabaseManager(const char* dbName, int openMode)
{
	m_Database = std::make_unique<db::Database_SQLite>(dbName, openMode);
}

// public
void DatabaseManager::Create(const char* dbName, int openMode)
{
	if (s_Instance == nullptr)
	{
		s_Mutex.lock();

		if (s_Instance == nullptr)
		{
			s_Instance = new DatabaseManager(dbName, openMode);
			s_IsInitialized = true;
		}

		s_Mutex.unlock();
	}
}


