#pragma once

BEGIN_NAMESPACE_DB


class Condition
{
public:

    enum Type
    {
        eEQUALS,
        eNOT_EQUALS,
        eLIKE,

        eINVALID
    };

    Condition() : Condition("", "", Type::eINVALID) {}
    Condition(const std::string& name, const std::string& value, Type type);

    virtual std::string ToString() const;

    const std::string& GetName() const { return m_ParameterName; }
    const std::string& GetValue() const { return m_ParameterValue; }
    Type GetType() const { return m_Type; }
    bool IsValid() const { return !GetName().empty() && !GetValue().empty(); }
    void Reset() { *this = Condition(); }
private:
    std::string m_ParameterName;
    std::string m_ParameterValue;
    Type        m_Type;
};


class ConditionGroup
{
public:

    enum Operator
    {
        AND,
        OR
    };

    void                Reset();
    ConditionGroup&     And(const Condition& cond);
    ConditionGroup&     Add(const Condition& cond);
    void                PopBack();
    std::string         ToString() const;
    bool                IsValid() const { return m_Conditions.size() > 0; }

private:
    std::vector<Condition> m_Conditions;

};


END_NAMESPACE_DB
