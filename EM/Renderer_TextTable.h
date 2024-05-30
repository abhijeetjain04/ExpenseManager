#pragma once

#include "DBTable_Expense.h"
#include "DBTable_Category.h"
#include "TextTable.h"
#include "ReportHandler.h"
#include <unordered_map>

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
        static void Render(const std::string& accountName, const std::vector<DBModel_Expense>& rows, double total = 0.0)
        {
            printf("\n Total Rows : %zd", rows.size());
            TextTable_Expense t(accountName, rows);
            t.Print();

            if (total != 0.0)
            {
                TextTable_TotalExpense tx(total);
                tx.Print();
            }
        }

        /**
        * This function renders the data in table format.
        *
        * @params [in] rows
        *       A map with key as the accountName that the expenses belong to, and value as the actual expense records for the given account.
        *
        * @params [in] total
        *       This arg is used to display the 'price' parameter's total.
        */
        static void Render(const std::unordered_map<std::string, std::vector<DBModel_Expense>>& rows, double total = 0.0)
        {
            size_t totalRows = 0;
            std::for_each(rows.begin(), rows.end(),
                [&](const auto& iter)
                {
                    totalRows += iter.second.size();
                });

            printf("\n Total Rows : %zd", totalRows);
            TextTable_Expense t(rows);
            t.Print();

            if (total != 0.0)
            {
                TextTable_TotalExpense tx(total);
                tx.Print();
            }
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

        static void Render(const std::vector<ReportHandler>& reports)
        {
            TextTable_CompareReport t(reports);
            t.Print();
        }

    };

}