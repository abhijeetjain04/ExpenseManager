#pragma once

#include <exception>
#include <cstdarg>

namespace em::exception
{
	class Base : public std::exception
	{

	public:
		Base() : Base("")
		{}

		Base(const std::string& message)
			: std::exception()
			, m_Message("EXCEPTION: " + message)
		{}

		virtual char const* what() const override
		{
			return m_Message.c_str();
		}

	protected:
		void SetMessage(const std::string& message)
		{
			m_Message = message;
		}

	private:
		std::string m_Message;
	};
}


