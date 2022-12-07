#include "pch.h"
#include "ValidCommand.h"


BEGIN_NAMESPACE_CLI
/*************** ValidCommand ***************/

//public
ValidCommand::ValidCommand(const std::string& name)
    : m_CommandName(name)
    , m_NumMandatoryParameters(0)
{
}

//public
ValidCommand& ValidCommand::AddParameter(
    const std::string& name,
    const ValidParameterProperties& props)
{
    m_Parameters.insert(std::make_pair(name, props));

    if (props.IsMandatory)
        ++m_NumMandatoryParameters;

    return *this;
}

//public 
ValidCommand& ValidCommand::AddFlag(const std::string& name, const std::string& helperMessage)
{
    m_Flags.insert(std::make_pair(name, helperMessage));
    return *this;
}

//public
bool ValidCommand::HasFlag(const std::string& flag) const
{
    return m_Flags.find(flag) != m_Flags.end();
}

//private
ValidParamMap::const_iterator ValidCommand::GetParameterAtIndex(size_t index) const
{
    return std::find_if(m_Parameters.begin(), m_Parameters.end(),
        [index](auto iter)
        {
            return iter.second.InOrderIndex == index;
        });
}

/*******************************************/

END_NAMESPACE_CLI