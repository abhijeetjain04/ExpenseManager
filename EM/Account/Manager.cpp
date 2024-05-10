#include "../pch.h"
#include "Manager.h"
#include "Account.h"
#include "../ConfigManager.h"
#include "Exceptions/InvalidAccountName.h"

namespace em::account
{
    Manager* Manager::s_Instance = nullptr;

    // private
    Manager::Manager()
    {
    }

    Manager::~Manager()
    {
        delete s_Instance;
        s_Instance = nullptr;
    }

    // public
    void Manager::Create()
    {
        DBG_ASSERT(s_Instance == nullptr);
        s_Instance = new Manager();

        const em::ValidAccountNames& names = em::ConfigManager::GetInstance().GetValidAccountNames();
        for (const std::string& name : names)
            s_Instance->AddAccount(name);
    }

    // public
    StatusCode Manager::SwitchAccount(const std::string& newAccountName)
    {
        if (m_CurrentAccountName == newAccountName)
            return StatusCode::AccountAlreadySelected;

        if (!AccountExists(newAccountName))
            return StatusCode::AccountDoesNotExist;

        SetCurrentAccountName(newAccountName);
        return StatusCode::Success;
    }

    // public
    bool Manager::AccountExists(const std::string& accountName) const
    {
        return m_Accounts.find(accountName) != m_Accounts.end();
    }

    // public
    void Manager::AddAccount(const std::string& accountName)
    {
        if (AccountExists(accountName))
            printf("\nAccount Already Added: %s", accountName.c_str());

        m_Accounts[accountName] = std::shared_ptr<Account>(new Account(accountName));
    }

    // Account
    const std::shared_ptr<Account> Manager::GetAccount(const std::string& accountName) const
    {
        if (!AccountExists(accountName))
        {
            printf("\nAccount does not exist: %s", accountName.c_str());
            return nullptr;
        }

        return m_Accounts.at(accountName);
    }

    // public
    const std::shared_ptr<Account> Manager::GetCurrentAccount() const
    {
        if (!AccountExists(m_CurrentAccountName))
            throw em::account::exceptions::InvalidAccountName(m_CurrentAccountName);

        return m_Accounts.at(m_CurrentAccountName);
    }

    // public
    bool Manager::IsUsingAllAccounts() const
    {
        return m_CurrentAccountName == "all";
    }

    // public static
    Manager& Manager::GetInstance()
    {
        DBG_ASSERT(s_Instance);
        return *s_Instance;
    }

}
