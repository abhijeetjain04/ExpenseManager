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



/**
* This is a Singleton class that will be used to parse the command line arguments for the application.
* It supports parsing of 2 types of arguments, namely params and flags.
* 
* -param param_value
* --flag
*/
class CLIParser
{
public:

    /**
    * This struct represents the return value that is asked from the CLIParser.
    * It has helper functions to retrieve the value as different basic types.
    */
    struct Param
    {
        Param(): m_Value("") {}
        Param(const std::string& value) : m_Value(value) {}

        int AsInt() { return std::stoi(m_Value); }
        double AsDouble() { return std::stod(m_Value); }
        const std::string& AsString() { return m_Value; }

    private:
        const std::string& m_Value;
    };

    /**
    * Returns the Singleton Instance pointer of the CLIParser object.
    */
    static CLIParser* GetInstance();

    /**
    * This function has to be used to register a command that the parser should accept aas being valid.
    * 
    * @param [in] commandName
    *       Name of the command to be registered
    *       
    * @return
    *       Reference to the ValidCommand object, to allow chaining.
    * 
    */
    ValidCommand& RegisterCommand(const std::string& commandName);

    /**
    * This function needs to be called to start the parsing the command line arguments provided to the application.
    * Prior to calling this function, we need to make sure that the valid commands are registered with the CLIParser.
    * 
    * @param [in] argc
    *       Number of cli arguments passed to the application.
    * 
    * @param [in] argv
    *       A string array, representing the cli arguments.
    * 
    * @return
    *       True, if the parsing was successful.
    *       False, otherwise.
    */
    bool Parse(int argc, char** argv);

    /**
    * Helper function to parse the cli arguments, which takes args in the form of a vector and has the .exe arg removed.
    *
    * @param [in] args
    *       CLI arguments passed to the application.
    *
    * @return
    *       True, if parsing was successful.
    */
    bool Parse(const std::vector<std::string>& args);

    /**
    * This function returns the value of the parameter.
    * 
    * @param [in] paramName
    *       Name of the parameter for which the value needs to be retrieved.
    * 
    * @return
    *       struct Param object, representing the value of the parameter.
    */
    Param GetParam(const std::string& paramName) const;

    /**
    * Checks if the flag was specified in the cli arguments.
    * 
    * @param [in] flagName
    *       Name of the flag that needs to be checked.
    * 
    * @return
    *       True, if the Flag is present in the cli arguments.
    */
    bool HasFlag(const std::string& flagName) const;

    /**
    * Checks if the Parameter was specified in the cli arguments.
    * 
    * @param [in] paramName
    *       Name of the parameter that needs to be checked.
    * 
    * @return
    *       True, if the parameter was specified in the cli arguments.
    */
    bool HasParameter(const std::string& paramName) const;

    /**
    * Generates the appropriate message on how to use the command for all the registered commands.
    */
    void DisplayHelp() const;

    /**
    * Returns all the cli command, options, flags as a json string.
    */
    std::string AsJson();


private:
    CLIParser() = default;
    CLIParser(const CLIParser&) = delete;

    /**
    * If the parsing fails, this function is used to generate the appropriate message on how to use the command.
    * 
    * @param [in] commandName
    *       Name of the command for which we need to generate the message.
    */
    void GenerateErrorMessage(const std::string& commandName) const;

    /**
    * Generates the appropriate message on how to use the command for 'm_CurrentCommand'.
    */
    void PrintHelperMessage();

    /**
    * Helper function to valid that a given userInput is of the valid type, is it was mentioned while registering the command.
    * 
    * @param [in] userInput
    *       The user input that needs to be validated.
    * 
    * @param [in] paramProp
    *       'struct ValidParameterProperties' that contains info required to determine whether 'userInput' is valid or not.
    */
    bool ValidateArg(const std::string& userInput, const ValidParameterProperties& paramProp) const;

    /**
    * This function validates wherther all the mandatory parameters are passed with the cli arguments for a given 'struct ValidCommand' object.
    * 
    * @param [in] args
    *       CLI arguments passed to the application.
    * 
    * @param [in] validCommand
    *       'struct ValidCommand' object containing data related to determine whether a command is valid.
    * 
    * @return
    *       True, if all the mandatory parameter and valid flags are passed in the cli arguments.
    */
    bool Validate(const std::vector<std::string>& args, const ValidCommand& validCommand);

    /**
    * This function is used to retrieve the valid Command object in the form of 'struct ValidCommand'/
    * 
    * @param [in] commandName
    *       Name of the command that we need to retrieve for.
    * 
    * @param [out] validCommand
    *       'struct ValidCommand' object that will be filled the appropriate data.
    * 
    * @returns
    *       True, if the valid command was found.
    */
    bool GetValidCommand(const std::string& commandName, ValidCommand& validCommand) const;
    
    /**
    * Helper function to get the Valid Commands.
    */
    std::vector<ValidCommand>& GetValidCommands() { return m_ValidCommands; }

    /**
    * Setter to save the name of the exe file that is runninng.
    */
    void SetExeName(const std::string& name) { m_ExeName = name; }

private:
    std::vector<ValidCommand>                       m_ValidCommands;
    std::unordered_map<std::string, std::string>    m_CommandParams;
    std::unordered_set<std::string>                 m_Flags;
    std::string                                     m_ExeName;
    std::string                                     m_CurrentCommandName;

    static std::mutex s_Mutex;
    static CLIParser* s_Instance;
};


END_NAMESPACE_CLI

