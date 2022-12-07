#pragma once

#include "Common.h"

#include <mutex>

#define cliParser (*cli::CLIParser::GetInstance())


/*
    A key-value pair is defined as follows:
        -key value

    A flag is defined as follows:
        --flag
*/


BEGIN_NAMESPACE_CLI


class ValidCommand;
struct ValidParameterProperties;

// singleton
class CLIParser
{
public:
    static CLIParser* GetInstance();

    ValidCommand& RegisterCommand(const std::string& commandName);
    bool Parse(char** argv, int argc);
    void SetHelperMessage(const std::string& message) { m_HelperMessage = message; }
    bool GetParam(const std::string& paramName, std::string& outValue) const;
    bool HasFlag(const std::string& flagName) const;
    bool HasParameter(const std::string& paramName) const;
    void SetExeName(const std::string& name) { m_ExeName = name; }
    void GenerateErrorMessage(const std::string& commandName) const;
    void DisplayHelp() const;

private:
    CLIParser() = default;
    CLIParser(const CLIParser&) = delete;

    bool Parse(const std::vector<std::string>& args);
    void PrintHelperMessage();
    bool ValidateArg(const std::string& userInput, const ValidParameterProperties& paramProp) const;
    bool Validate(const std::vector<std::string>& args, const ValidCommand& validCommand);
    bool GetValidCommand(const std::string& commandName, ValidCommand& vc) const;
    std::vector<ValidCommand>& GetValidCommands() { return m_ValidCommands; }

private:
    std::vector<ValidCommand>                       m_ValidCommands;
    std::unordered_map<std::string, std::string>    m_CommandParams;
    std::unordered_set<std::string>                 m_Flags;
    std::string                                     m_HelperMessage;
    std::string                                     m_ExeName;
    std::string                                     m_CurrentCommandName;

    static std::mutex s_Mutex;
    static CLIParser* s_Instance;
};


END_NAMESPACE_CLI

