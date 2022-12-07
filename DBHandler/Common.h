#pragma once

#include <stdio.h>

#include <assert.h>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include <sstream>
#include <format>

#define NAMESPACE_DB db
#define BEGIN_NAMESPACE_DB  namespace NAMESPACE_DB {
#define END_NAMESPACE_DB    }
#define USING_NAMESPACE_DB  using namespace NAMESPACE_DB;


#define NAMESPACE_UTIL util
#define BEGIN_NAMESPACE_DB_UTIL BEGIN_NAMESPACE_DB  namespace NAMESPACE_UTIL {
#define END_NAMESPACE_DB_UTIL                       END_NAMESPACE_DB }
#define USING_NAMESPACE_DB_UTIL                     using namespace NAMESPACE_DB::NAMESPACE_UTIL

#if defined SQLITE_LOGS_ENABLED
    #define SQLITE_EXCEPTION(e) printf("\nSQLite::Exception : %s", e.what()); \
                                assert(false);
    #define SQLITE_LOG_QUERY(query) printf("\n--- SQLITE QUERY --- : %s", query);
#else
    #define SQLITE_EXCEPTION(e)     (void)(e); assert(false);
    #define SQLITE_LOG_QUERY(query) void(query);
#endif

