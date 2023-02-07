#include "pch.h"
#include "CLIParser.h"
#include "ValidCommand.h"
#include "Utils.h"



BEGIN_NAMESPACE_CLI


/*************** CLIParser ***************/

std::mutex CLIParser::s_Mutex;
CLIParser* CLIParser::s_Instance = nullptr;

CLIParser* CLIParser::GetInstance()
{
    if (s_Instance == nullptr)
    {
        s_Mutex.lock();
        if(s_Instance == nullptr)
            s_Instance = new CLIParser();
        s_Mutex.unlock();
    }
    return s_Instance;
}

//public
ValidCommand& CLIParser::RegisterCommand(const std::string& commandName)
{
    return GetValidCommands().emplace_back(commandName);
}

// public 
CLIParser::Param CLIParser::GetParam(const std::string& paramName) const
{
    auto iter = m_CommandParams.find(paramName);
    if (iter == m_CommandParams.end())
        return Param();

    return Param(iter->second);
}

//public
bool CLIParser::HasFlag(const std::string& flagName) const
{
    return m_Flags.find(flagName) != m_Flags.end();
}

//public 
bool CLIParser::HasParameter(const std::string& paramName) const
{
    return m_CommandParams.find(paramName) != m_CommandParams.end();
}

//public
bool CLIParser::Parse(int argc, char** argv)
{
    SetExeName(argv[0]);
        
    argv++; argc--; // skip the .exe name

    std::vector<std::string> args;
    for (int i = 0; i < argc; ++i)
        args.push_back(argv[i]);

    return Parse(args);
}

// public
void CLIParser::GenerateErrorMessage(const std::string& commandName) const 
{
    ValidCommand validCommand;
    if (!GetValidCommand(commandName, validCommand))
    {
        printf("\nInvalid Command : %s", commandName.c_str());
        DisplayHelp();
        return;
    }

    std::ostringstream oss;

    oss << "\nUsage: \t" << commandName << " ";
    const auto& validParams = validCommand.GetParameters();
    for (const auto [propName, prop] : validParams)
    {
        if (!prop.IsMandatory)
            continue;
        
        oss << "<" << propName << ">";
        oss << " ";
    }

    oss << "<options...> <flags...> ";

    // optional params
    
    bool firstOptionalRead = false;
    for (const auto [propName, prop] : validParams)
    {
        if (prop.IsMandatory)
            continue;

        if (!firstOptionalRead)
        {
            oss << "\n";
            oss << "Optional Params:";
            firstOptionalRead = true;
        }

        oss << "\n\t -" << propName << "  \t\t";
        oss << "- " << prop.HelperMessage;
    }

    // flags
    const auto& flags = validCommand.GetFlags();
    if (flags.size())
    {
        oss << "\nFlags: ";

        for (const auto [name, helperMessage] : flags)
        {
            oss << "\n\t--" << name << "  \t\t";
            oss << "- " << helperMessage;
        }
    }

    oss << "\n--------------------------------------------------------------------";
    printf("\n%s", oss.str().c_str());
}

// public
void CLIParser::DisplayHelp() const
{
    for (const ValidCommand& cmd : m_ValidCommands)
        GenerateErrorMessage(cmd.GetName());
}

//private
bool CLIParser::Parse(const std::vector<std::string>& args)
{
    if (args.size() < 1)
        return false;

    m_CurrentCommandName = args[0];
    ValidCommand validCommand;
    if (!GetValidCommand(m_CurrentCommandName, validCommand) || !Validate(args, validCommand))
    {
        PrintHelperMessage();
        return false;
    }

    return true;
}

//private
bool CLIParser::ValidateArg(
    const std::string& userInput, 
    const ValidParameterProperties& paramProp) const
{
    bool isValid = true;

    switch (paramProp.Type)
    {
    case OptionType::TEXT:
        isValid = true;
        break;
    case OptionType::DOUBLE:
        if (!util::IsDouble(userInput))
            isValid = false;
        break;

    case OptionType::ALPHA_NUMERIC:
        if (!util::IsAlphaNumeric(userInput))
            isValid = false;
        break;

    case OptionType::INTEGER:
        if (!util::IsInteger(userInput))
            isValid = false;
        break;
    case OptionType::DATE:
        if (!util::IsDate(userInput))
            isValid = false;
        break;

    default:
        isValid = false;
        break;
    }

    return isValid;
}

//private
bool CLIParser::Validate(
    const std::vector<std::string>& args, 
    const ValidCommand& validCommand)
{
    size_t numMandatoryOptions = validCommand.GetNumMandatoryParameters();
    if (numMandatoryOptions > (args.size() - 1)) // size - 1, because 1st arg is the command name
        return false;

    size_t argIndex = 1;
    size_t mandatoryOptionIndex = 1;

    bool isValid = true;
    while (argIndex < args.size())
    {
        const std::string& currArg = args[argIndex];

        bool isOptionalParam = currArg[0] == '-' && currArg[1] != '-'; // '-' means an optional argument
        bool isFlag = (currArg[0] == '-' && currArg[1] == '-');
        if (isFlag) // is a flag
        {
            const std::string& option = args[argIndex];
            const std::string& flagName = currArg.substr(2, currArg.size() - 1); // remove the first '--' chars

            const auto& flags = validCommand.GetFlags();
            if (!validCommand.HasFlag(flagName))
            {
                isValid = false;
                break;
            }

            m_Flags.insert(flagName);
        }
        else if (isOptionalParam)
        {
            ++argIndex;
            if (argIndex >= args.size())
            {
                isValid = false;
                break;
            }

            const std::string& option = args[argIndex];
            const std::string& paramName = currArg.substr(1, currArg.size() - 1); // remove the first '-' char

            const cli::ValidParamMap& validParams = validCommand.GetParameters();
            cli::ValidParamMap::const_iterator iter = validParams.find(paramName);
            if (iter == validParams.end())
            {
                isValid = false;
                break;
            }

            const ValidParameterProperties& paramProp = iter->second;
            if (!ValidateArg(option, paramProp))
            {
                isValid = false;
                break;
            }

            m_CommandParams[paramName] = option;
        }
        else // isMandatory
        {
            if (mandatoryOptionIndex > numMandatoryOptions) // more mandatory args are passed
            {
                isValid = false;
                break;
            }

            const std::string& option = args[argIndex];

            auto iter = validCommand.GetParameterAtIndex(mandatoryOptionIndex);

            const ValidParameterProperties& paramProp = iter->second;
            if (!ValidateArg(option, paramProp))
                break;

            const std::string& paramName = iter->first;
            m_CommandParams[paramName] = option;

            ++mandatoryOptionIndex;
        }

        ++argIndex;
    }

    return isValid;
}

//private
bool CLIParser::GetValidCommand(const std::string& commandName, ValidCommand& validCommand) const
{
    for (const auto& vc : m_ValidCommands)
    {
        if (vc.GetName() == commandName)
        {
            validCommand = vc;
            return true;
        }
    }

    return false;
}

//private
void CLIParser::PrintHelperMessage()
{
    GenerateErrorMessage(m_CurrentCommandName);
}

/*****************************************/

END_NAMESPACE_CLI


