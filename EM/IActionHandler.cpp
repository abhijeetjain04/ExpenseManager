#include "pch.h"
#include "IActionHandler.h"
#include "rapidjson/document.h"

namespace em::action_handler
{
	ResultSPtr Interface::Execute(const std::string& json)
	{
        std::string commandName;
		std::unordered_set<std::string> flags;
		std::map<std::string, std::string> options;

        rapidjson::Document doc;
        if (doc.Parse(json.c_str(), json.length()).HasParseError())
            return Result::Create(ErrorCode::JSONParsingError, "Failed to parse JSON.", "");

        // get the command name.
        commandName = doc["command"].GetString();

        // get the flags.
        rapidjson::Value& jsonFlags = doc["flags"];
        assert(jsonFlags.IsArray());
        for (rapidjson::Value::ConstValueIterator iter = jsonFlags.Begin(); iter != jsonFlags.End(); iter++)
            flags.insert(iter->GetString());

        // get the options.
        rapidjson::Value& jsonOptions = doc["options"];
        assert(jsonOptions.IsArray());
        for (rapidjson::Value::ConstValueIterator itr = jsonOptions.Begin(); itr != jsonOptions.End(); ++itr)
        {
            const rapidjson::Value& option = *itr;
            assert(option.IsObject()); // each attribute is an object
            for (rapidjson::Value::ConstMemberIterator itr2 = option.MemberBegin(); itr2 != option.MemberEnd(); ++itr2)
                options[itr2->name.GetString()] = itr2->value.GetString();
        }

        return Execute(commandName, flags, options);
	}
}
