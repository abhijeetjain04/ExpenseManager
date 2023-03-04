#pragma once

#include "DBTable_Expense.h"
#include "DBTable_Category.h"
#include "TextTable.h"
#include "ReportHandler.h"

BEGIN_NAMESPACE_EM

class Renderer_ExpenseTable
{
public:

    static void Render(const std::vector<DBModel_Expense>& rows, double total = 0)
    {
        printf("\n Total Rows : %zd", rows.size());
        TextTable_Expense t(rows);
        t.Print();

        TextTable_TotalExpense tx(total);
        tx.Print();
    }

};

class Renderer_CategoryTable
{
public:

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

END_NAMESPACE_EM