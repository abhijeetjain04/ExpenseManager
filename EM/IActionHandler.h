#pragma once


namespace em::action_handler
{
	struct Result;
	using ResultSPtr = std::shared_ptr<Result>;

	struct Result
	{
		StatusCode	statusCode;
		std::string message;

		static ResultSPtr Create(StatusCode statusCode, const std::string& message = "")
		{
			return std::make_shared<Result>(statusCode, message);
		}
	};


	class Interface
	{
	public:
		ResultSPtr Execute(const std::string& json);

	protected:
		virtual ResultSPtr Execute(
			const std::string& commandName,
			const std::unordered_set<std::string>& flags, 
			const std::map<std::string, std::string>& options) = 0;

	};

}
