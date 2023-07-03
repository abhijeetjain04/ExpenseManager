#pragma once

namespace em::account
{
    class Manager;

    /**
    * This class deals with an individual account.
    */
    class Account
    {
    public:

        // Getter for the account name.
        const std::string& GetName() const
        {
            return m_Name;
        }

    private:
        Account(const std::string& name);

        std::string m_Name;

        // Accounts can be created only through the Manager class.
        friend class em::account::Manager;
    };

}
