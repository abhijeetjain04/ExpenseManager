#include "../pch.h"
#include "IActionHandler.h"
#include "JsonHelper/json.h"

namespace em::action_handler
{
	ResultSPtr Interface::Execute(const std::string& json)
	{
        std::string commandName;
		std::unordered_set<std::string> flags;
		std::map<std::string, std::string> options;

        Json::Value root;
        Json::Reader reader;
        reader.parse(json, root);

        // Set command name.
        commandName = root["command"].asString();

        // Set Flags
        const Json::Value& flagsJson = root["flags"];
        for (const Json::Value& flagJson : flagsJson)
        {
            flags.insert(flagJson.asString());
        }

        // Set Options
        const Json::Value& optionsJson = root["options"];
        const std::vector<std::string>& memberNames = optionsJson.getMemberNames();
        for (const std::string& key : memberNames)
        {
            std::string value = optionsJson[key].asString();
            options.insert(std::make_pair(key, value));
        }

        return Execute(commandName, flags, options);
	}
}
