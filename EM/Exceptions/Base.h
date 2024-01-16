#pragma once

#include <exception>
#include <cstdarg>
#include <format>

#define EXCEPTION_CLASS(class_name) \
	class class_name : public em::exception::Base \
	{ \
	public: \
		class_name(const std::string& message) \
			: Base(message, __FILE__, __FUNCTION__) \
		{ \
		} \
	}; \

namespace em::exception
{

	class Base : public std::exception
	{

	public:
		Base() : Base("")
		{}

		Base(const std::string& message, const std::string& fileName = "", const std::string& functionName = "")
			: std::exception()
		{
			SetMessage(message, fileName, functionName);
		}

		virtual char const* what() const override
		{
			return m_Message.c_str();
		}

	protected:
		void SetMessage(const std::string& message, const std::string& fileName = "", const std::string& functionName = "")
		{
			m_Message = std::format("Exception {}, {} : {}", fileName, functionName, message);
		}

	private:
		std::string m_Message;
	};
}


