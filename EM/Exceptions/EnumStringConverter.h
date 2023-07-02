#pragma once

#include <cstdarg>

#include "Base.h"

namespace em::exception
{
	class EnumStringConverter : public em::exception::Base
	{
	public:
		EnumStringConverter(const std::string& from, const std::string& to)
		{
			SetMessage(std::format("Failed to Convert from {} to {}.", from, to));
		}
	};
}


