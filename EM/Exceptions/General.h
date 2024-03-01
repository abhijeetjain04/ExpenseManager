#pragma once

#include <cstdarg>

#include "Base.h"

namespace em::exception
{
	class General : public em::exception::Base
	{
	public:
		General(const std::string& message)
		{
			SetMessage(message);
		}
	};
}


