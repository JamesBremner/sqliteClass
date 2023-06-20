#include <string>
#include <functional>
#include "sqlite3.h"

namespace raven {
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

    /// @brief Prepare a statement
    /// @param query 
    /// @return prepared statement
    sqliteClassStmt * prepare(const std::string &query);

    /// @brief Bind value to prepared statement place holder
    /// @param stmt prepared statement
    /// @param index index of place holder
    /// @param value 
    /// @return 0 for OK
    int bind( sqliteClassStmt * stmt, int index, int value );


    /// @brief execute prpared statement, with return data expected
    /// @param stmt
    /// @param rowHandler called on each row returned
    /// @return 0 for OK
    ///
    /// When all rows have been read, the prepared statement is reset
    int exec(
        sqliteClassStmt * stmt,
        std::function<bool(sqliteClassStmt&)> rowHandler);

    /// @brief reset prepared statement
    /// @param stmt 
    /// @return 0 for OK
    int reset( 
        sqliteClassStmt * stmt);

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

class sqliteClassStmt
{
public:
    sqliteClassStmt(sqlite3 *db, const std::string &query)
    {
        int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
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
    int reset()
    {
        return sqlite3_reset( stmt );
    }
    int column_int( int index )
    {
        return sqlite3_column_int( stmt, index );
    }
    std::string column_string( int index ) {
        return std::string((char *)sqlite3_column_text(stmt, index));
    }
    double column_double( int index )
    {
        return sqlite3_column_double( stmt, index );
    }

private:
    sqlite3_stmt *stmt;
};
}