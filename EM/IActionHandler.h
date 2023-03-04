#pragma once


namespace em::action_handler
{
	struct Result;
	using ResultSPtr = std::shared_ptr<Result>;

	struct Result
	{
		ErrorCode	errorCode;
		std::string errorMessage;
		std::string resultMessage;

		static ResultSPtr Create(ErrorCode errorCode, const std::string& errMessage = "", const std::string& resMessage = "")
		{
			return std::make_shared<Result>(errorCode, errMessage, resMessage);
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
