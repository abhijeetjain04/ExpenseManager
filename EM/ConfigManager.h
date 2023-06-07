#pragma once

BEGIN_NAMESPACE_EM

typedef std::vector<std::string> ValidDatabaseNames;

/**
* This Singleton class handles everything related to the Configuration data 
* that will be contained in config.json file.
*/
class ConfigManager
{
	/**
	* struct used to store config data.
	*/
	struct Data
	{
		std::string DefaultLocation;
		ValidDatabaseNames ValidDatabaseNames;
	};

public:
	ConfigManager();

	const std::string& GetDefaultLocation() const;
	bool IsValidDatabaseName(const std::string& dbName) const;
	ValidDatabaseNames GetValidDatabaseNames() const;

	static ConfigManager& GetInstance();

private:
	/**
	* This function will read the config.json file and store the relevant data.
	*/
	void Initialize();

	Data m_Data;
	static ConfigManager* s_Instance;
};

END_NAMESPACE_EM