#pragma once

#include "EM/Common/CommonEnums.h"

#define MAP_ENTRY(enum_name, string_value) {enum_name::string_value, #string_value}

namespace em::common
{
	class EnumAndStringConverter
	{
	public:
		static const std::string& ConvertCmdTypeEnumToString(em::CmdType cmdType);

	private:
		inline static std::unordered_map<em::CmdType, std::string> s_MapCmdTypeEnumToString
		{
			{em::CmdType::Help, "Help"},
			{em::CmdType::AddCategory, "AddCategory"},
			{em::CmdType::Add, "Add"},
			{em::CmdType::Remove, "Remove"},
			{em::CmdType::List, "List"},
			{em::CmdType::Report, "Report"},
			{em::CmdType::CompareMonths, "CompareMonths"},
			{em::CmdType::ClearScreen, "ClearScreen"},
			{em::CmdType::Invalid, "Invalid"},
			MAP_ENTRY(em::CmdType, Invalid)
		};


	};
}


