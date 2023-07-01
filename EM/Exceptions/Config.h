#pragma once

#include <cstdarg>

#include "Exceptions/Base.h"

namespace em::exception
{
	class Config : public em::exception::Base
	{
	public:
		Config(const std::string& message)
			: em::exception::Base(message)
		{
		}
	};
}


