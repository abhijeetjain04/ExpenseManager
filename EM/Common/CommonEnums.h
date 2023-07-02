#pragma once

namespace em
{
    /**
    * List of supported commands by the application.
    */
    enum class CmdType
    {
        Help,
        AddCategory,
        Add,
        Remove,
        List,
        Report,
        CompareMonths,
        ClearScreen,
        Invalid
    };

}

