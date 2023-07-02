#include "pch.h"
#include "DatabaseManager.h"

BEGIN_NAMESPACE_EM

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
DatabaseManager::DatabaseManager(const char* dbName, const std::string& accountName, int openMode)
	: m_AccountName(accountName)
{
	m_UseAllAccounts = (accountName == "all");
	m_Database = std::make_unique<db::Database_SQLite>(dbName, openMode);
}

// public static
void DatabaseManager::Create(const char* dbName, const std::string& accountName, int openMode)
{
	if (s_Instance == nullptr)
	{
		s_Mutex.lock();

		if (s_Instance == nullptr)
		{
			s_Instance = new DatabaseManager(dbName, accountName, openMode);
			s_IsInitialized = true;
		}

		s_Mutex.unlock();
	}
}

// public
bool DatabaseManager::IsUsingAllAccounts() const
{
	return m_UseAllAccounts;
}

END_NAMESPACE_EM
