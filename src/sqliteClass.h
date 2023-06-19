#include <string>
#include <functional>
#include "sqlite3.h"

/// @brief  A thin header only C++ wrapper for the sqlite C API
class sqliteClass
{
public:
    /// @brief open database
    /// @param filepath
    /// @return 0 for OK
    int open(const std::string &filepath)
    {
        return sqlite3_open(filepath.c_str(), &db);
    }

    /// @brief execute SQL query, no return data expected
    /// @param query 
    /// @return 0 for OK
    int exec(const std::string &query)
    {
        return sqlite3_exec(db, query.c_str(), 0, 0, &dbErrMsg);
    }

    /// @brief execute SQL query, with return data expected
    /// @param query
    /// @param rowHandler
    /// @return
    int exec(const std::string &query,
             std::function<bool(sqlite3_stmt *)> rowHandler)
    {
        sqlite3_stmt *stmt;
        int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
        while (1)
        {
            rc = sqlite3_step(stmt);
            if (rc == SQLITE_DONE)
                return 0;
            else if (rc != SQLITE_ROW)
                return rc;
            rowHandler(stmt);
        }
    }

    /// @brief read column value as integer, call from row_handler
    /// @param stmt 
    /// @param index 
    /// @return 
    int column_int(sqlite3_stmt *stmt, int index)
    {
        return sqlite3_column_int(stmt, index);
    }

private:
    sqlite3 *db;
    char *dbErrMsg;
};