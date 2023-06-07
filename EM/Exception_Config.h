#pragma once

#include <exception>
#include <cstdarg>

BEGIN_NAMESPACE_EM

class Exception_Config : public std::exception
{

public:
	Exception_Config(const std::string& message)
		: std::exception()
	{
		m_Message = "EXCEPTION_CONFIG => " + message;
	}

	virtual char const* what() const override
	{
		return m_Message.c_str();
	}

private:
	std::string m_Message;
};

END_NAMESPACE_EM