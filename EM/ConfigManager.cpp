#include "pch.h"
#include "ConfigManager.h"
#include "Utils.h"
#include "json.h"
#include "Exceptions/Config.h"

#include <fstream>

namespace em
{


	ConfigManager* ConfigManager::s_Instance = nullptr;

	// public
	ConfigManager::ConfigManager()
	{
		Initialize();
	}

	ConfigManager::~ConfigManager()
	{
		delete s_Instance;
		s_Instance = nullptr;
	}

	// public
	bool ConfigManager::HasDefaultAccount() const
	{
		return m_Data.DefaultAccountName.has_value();
	}

	// public
	const std::string& ConfigManager::GetDefaultAccount() const
	{
		if (!HasDefaultAccount())
			throw std::exception("Default account name not present in config.json");

		return m_Data.DefaultAccountName.value();
	}

	// public
	const std::string& ConfigManager::GetDefaultLocation() const
	{
		return m_Data.DefaultLocation;
	}

	// public
	bool ConfigManager::IsValidAccountName(const std::string& dbName) const
	{
		const std::vector<std::string>& validDbNames = m_Data.ValidAccountNames;
		return std::find(validDbNames.begin(), validDbNames.end(), dbName) != validDbNames.end();
	}

	// public
	ValidAccountNames ConfigManager::GetValidAccountNames() const
	{
		return m_Data.ValidAccountNames;
	}

	// private
	void ConfigManager::Initialize()
	{
		try
		{
			std::string configFilePath = utils::GetConfigFilePath();
			std::ifstream inputStream(configFilePath);

			Json::Value root;
			inputStream >> root;

			// store default Account name
			if (root["default"].isMember("account"))
				m_Data.DefaultAccountName = root["default"]["account"].asString();

			// store default location
			m_Data.DefaultLocation = root["default"]["location"].asString();

			// store valid database names
			DBG_ASSERT(root.isMember("validDatabaseNames"));
			for (const auto& name : root["validDatabaseNames"])
				m_Data.ValidAccountNames.push_back(name.asString());

		}
		catch (std::exception& e)
		{
			throw em::exception::Config(e.what());
		}

	}

	// public
	ConfigManager& ConfigManager::GetInstance()
	{
		if (s_Instance == nullptr)
			s_Instance = new ConfigManager();

		return *s_Instance;
	}

}