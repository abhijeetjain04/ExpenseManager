#include "pch.h"
#include "DatabaseManager.h"
#include "Account/Account.h"
#include "Account/Manager.h"

namespace em
{

    // private
    DatabaseManager* DatabaseManager::s_Instance = nullptr;
    bool DatabaseManager::s_IsInitialized = false;
    std::mutex DatabaseManager::s_Mutex;

    // public
    DatabaseManager& DatabaseManager::GetInstance()
    {
        if (!s_IsInitialized)
            DBG_ASSERT(!"DatabaseManager Not Initialized yet!");
        return *s_Instance;
    }

	DatabaseManager::~DatabaseManager()
	{
        delete s_Instance;
        s_Instance = nullptr;
	}

	// public
    DatabaseManager::DatabaseManager(const char* dbName, int openMode)
    {
        m_Database = std::make_unique<db::Database_SQLite>(dbName, openMode);
    }

    // public static
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

    // public
    void DatabaseManager::RegisterExpenseTables()
    {
        // get the account that is currently in use.
        const std::shared_ptr<em::account::Account>& account = em::account::Manager::GetInstance().GetCurrentAccount();
        const std::string& accountName = account->GetName();

        m_ExpenseTables.clear();
        if (accountName == "all")
        {
            RegisterExpenseTable<em::DBTable_PersonalExpense>("personal_expense");
            RegisterExpenseTable<em::DBTable_HouseholdExpense>("household_expense");
            RegisterExpenseTable<em::DBTable_MarriageExpense>("marriage_expenses");
            return;
        }

        // expeses for different accounts are stored as different tables in the database.
        const std::string& tableName = accountName;
        std::string fullTableName = tableName + "_expense";
        if (fullTableName == "personal_expense")
        {
            RegisterExpenseTable<em::DBTable_PersonalExpense>(tableName);
            return;
        }

        if (fullTableName == "household_expense")
        {
            RegisterExpenseTable<em::DBTable_HouseholdExpense>(tableName);
            return;
        }

        if (fullTableName == "marriage_expense")
        {
            RegisterExpenseTable<em::DBTable_MarriageExpense>(tableName);
            return;
        }

        DBG_ASSERT(!"Invalid Table Name");
    }


    // public
    void DatabaseManager::OnSwitchAccount()
    {
        RegisterExpenseTables();
    }

}
