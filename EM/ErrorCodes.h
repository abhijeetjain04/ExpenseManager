#pragma once

#define ERROR_LOG(x, ...)   printf("\n<ERROR> %s </ERROR>" , std::format(x, __VA_ARGS__).c_str())

enum class ErrorCode
{
    Success,

    CommandDoesNotExist,
    InvalidNumMandatoryArgs,
    FlagDoesNotExist,
    ParameterNameDoesNotExist,
    InvalidParameterValue,

    CategoryDoesNotExist,

    DBError,

    Invalid
};

