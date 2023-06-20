#include <string>
#include <functional>
#include "sqlite3.h"

class sqliteClassStmt;

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
    /// @param rowHandler called on each row returned
    /// @return
    int exec(
        const std::string &query,
        std::function<bool(sqliteClassStmt&)> rowHandler);

    // sqliteClassStmt * prepare(const std::string &query);

    // int bind( sqliteClassStmt * stmt, int index, int value );


    /// @brief execute statement, with return data expected
    /// @param stmt
    /// @param rowHandler called on each row returned
    /// @return
    int exec(
        sqliteClassStmt & stmt,
        std::function<bool(sqliteClassStmt&)> rowHandler);

    /// @brief read column value as integer, call from row_handler
    /// @param stmt
    /// @param index
    /// @return
    int column_int(sqlite3_stmt *stmt, int index)
    {
        return sqlite3_column_int(stmt, index);
    }

    sqlite3 *getdb()
    {
        return db;
    }

private:
    sqlite3 *db;
    char *dbErrMsg;
};

class sqliteClassStmt
{
public:
    sqliteClassStmt(sqliteClass &db, const std::string &query)
    {
        int rc = sqlite3_prepare_v2(db.getdb(), query.c_str(), -1, &stmt, 0);
        if( rc )    
            throw std::runtime_error(
                "sqlite3_prepare " + query            );
    }
    ~sqliteClassStmt()
    {
        sqlite3_finalize( stmt );
    }
    int step()
    {
        return sqlite3_step(stmt);
    }
    int bind( int index, int value )
    {
        return sqlite3_bind_int( stmt, index, value );
    }
    int column_int( int index )
    {
        return sqlite3_column_int( stmt, index );
    }

private:
    sqlite3_stmt *stmt;
};