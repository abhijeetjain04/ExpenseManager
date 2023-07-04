#pragma once

#include "DBTable_Expense.h"
#include "DBTable_Category.h"
#include "TextTable.h"
#include "ReportHandler.h"

namespace em
{


    /**
    * Helper class that can be used to display data from DBTable_Expense in the form of table for CLI.
    */
    class Renderer_ExpenseTable
    {
    public:

        /**
        * This function renders the data in table format.
        *
        * @params [in] rows
        *       Rows representing the rows from DBTable_Expense that needs to be displayed in the table.
        *
        * @params [in] total
        *       This arg is used to display the 'price' parameter's total.
        */
        static void Render(const std::vector<DBModel_Expense>& rows, double total = 0.0)
        {
            printf("\n Total Rows : %zd", rows.size());
            TextTable_Expense t(rows);
            t.Print();

            TextTable_TotalExpense tx(total);
            tx.Print();
        }

    };

    /**
    * Helper class that can be used to display data from DBTable_Category in the form of table for CLI.
    */
    class Renderer_CategoryTable
    {
    public:

        /**
        * This function renders the data in table format.
        *
        * @params [in] rows
        *       Rows representing the rows from DBTable_Category that needs to be displayed in the table.
        */
        static void Render(const std::vector<DBModel_Category>& rows)
        {
            printf("\n Total Rows : %zd", rows.size());
            TextTable_Category t(rows);
            t.Print();
        }

    };

    class Renderer_CompareReport
    {
    public:

        static void Render(const ReportHandler& rh1, const ReportHandler& rh2)
        {
            printf("\n Total Rows : %zd", rh1.GetPrices().size());
            TextTable_CompareReport t(rh1, rh2);
            t.Print();
        }

    };

}