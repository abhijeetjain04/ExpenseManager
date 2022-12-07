#include "Common.h"
#include "Condition.h"

BEGIN_NAMESPACE_DB


Condition::Condition(
    const std::string& name, 
    const std::string& value, 
    Type type)
    : m_ParameterName(name)
    , m_ParameterValue(value)
    , m_Type(type)
{
}


std::string Condition::ToString() const
{
    std::ostringstream oss;

    std::string oper;
    switch (GetType())
    {
    case Type::eEQUALS:
        oper = " = ";
        break;
    case Type::eNOT_EQUALS:
        oper = " != ";
        break;
    case Type::eLIKE:
        oper = " LIKE ";
        break;
    }
    oss << GetName() << oper << "'" << GetValue() << "'";
    return oss.str();
}

/******************* ConditionGroup *********************/

void ConditionGroup::PopBack()
{
    m_Conditions.pop_back();
}

void ConditionGroup::Reset()
{
    while (m_Conditions.size())
        m_Conditions.pop_back();
}

ConditionGroup& ConditionGroup::And(const Condition& cond)
{
    m_Conditions.push_back(cond);
    return *this;
}

ConditionGroup& ConditionGroup::Add(const Condition& cond)
{
    m_Conditions.push_back(cond);
    return *this;
}

std::string ConditionGroup::ToString() const
{
    std::ostringstream oss;

    for (size_t i=0; i<m_Conditions.size(); ++i)
    {
        const auto& cond = m_Conditions[i];
        oss << cond.ToString();

        if (i != m_Conditions.size() - 1)
            oss << " AND ";
    }

    return oss.str();
}

/********************************************************/


END_NAMESPACE_DB