#pragma once

#define ERROR_LOG(x, ...)   printf("\n<ERROR> %s </ERROR>" , std::format(x, __VA_ARGS__).c_str())

BEGIN_NAMESPACE_EM

enum class StatusCode
{
    Success,

    CommandDoesNotExist,
    InvalidNumMandatoryArgs,
    FlagDoesNotExist,
    ParameterNameDoesNotExist,
    InvalidParameterValue,

    CategoryDoesNotExist,

    DBError,

    JSONParsingError,

    NotImplementedYet,

    Invalid
};


END_NAMESPACE_EM