#pragma once

namespace em::account
{
    class Account;

    /**
    * This Singleton class deals with everything related to accounts
    */
    class Manager
    {
    public:
        /**
        * Creates the singleton instance.
        */
        static void Create();

        /**
        * Getter for the singleton instance.
        */
        static Manager& GetInstance();

        /**
        * This function is used to switch accounts.
        * 
        * @params [in] accountName
        *       Name of the account that we want to switch to.
        * 
        * @returns 
        *       True, if account was switched successfully.
        */
        StatusCode SwitchAccount(const std::string& newAccountName);

        /**
        * Returns true if the account exists with the account manager.
        * 
        * @params [in] accountName
        *       Name of the account to be checked.
        */
        bool AccountExists(const std::string& accountName) const;

        /**
        * Returns the account corresponding to the account name.
        * Returns nullptr if the account does not exist.
        * 
        * @params [in] accountName
        *       Name of the account to be fetched.
        */
        const std::shared_ptr<Account> GetAccount(const std::string& accountName) const;

        /**
        * Setter for the current account name.
        * 
        * @params [in] accountName
        *       Name of the account that will be used from now on.
        */
        void SetCurrentAccountName(const std::string& accountName)
        {
            m_CurrentAccountName = accountName;
        }

        /**
        * Returns the account that is currently in use.
        */
        const std::shared_ptr<Account> GetCurrentAccount() const;

        /**
        * Returns flag used to determine if we are considering all accounts for DB queries.
        */
        bool IsUsingAllAccounts() const;

		/**
		* Destructor 
		*/
        ~Manager();

    private:
        Manager();
        Manager(const Manager&) = default;

        void AddAccount(const std::string& accountName);

    private:
        // Map of <account name, Account class>.
        std::unordered_map<std::string, std::shared_ptr<Account>> m_Accounts;
        std::string m_CurrentAccountName;
        static Manager* s_Instance;
    };
}
