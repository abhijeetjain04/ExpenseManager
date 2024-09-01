#pragma once

constexpr auto ERROR_DB_SELECT_CATEGORY             = "Failed to retrieve Categories.";
constexpr auto ERROR_DB_SELECT_TAG                  = "Failed to retrieve Tags.";
constexpr auto ERROR_DB_INSERT_CATEGORY             = "Failed to add Category: '{}'";
constexpr auto ERROR_DB_INSERT_EXPENSE              = "Failed to add Expense: '{}'";
constexpr auto ERROR_DB_UPDATE_EXPENSE              = "Failed to update Expense: '{}'";
constexpr auto ERROR_DB_REMOVE_EXPENSE              = "Failed to remove Expense: '{}'";
constexpr auto ERROR_DB_INSERT_TAG                  = "Failed to add Tag: '{}'";

constexpr auto ERROR_CATEGORY_DOES_NOT_EXIST        = "Category does not exist: '{}'";
constexpr auto ERROR_CATEGORY_ALREADY_EXIST         = "Category already exist: '{}'";
constexpr auto ERROR_TAG_DOES_NOT_EXIST             = "Tag does not exist: '{}'";
constexpr auto ERROR_INVALID_ATTRIBUTE_TYPE         = "Invalid Attribute Type: '{}'";

// Account
constexpr auto ERROR_ACCOUNT_DOES_NOT_EXIST         = "Account does not exist: '{}'.";
constexpr auto ERROR_ACCOUNT_ALREADY_SELECTED       = "Account '{}' is already selected.";

constexpr auto ERROR_ACTIONHANDLER_EXECUTE          = "'{}'";

constexpr auto ERROR_CLI_PARSING                    = "Failed to parse the cli.";
