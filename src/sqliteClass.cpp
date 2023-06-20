#include <stdexcept>

#include "sqliteClass.h"

int sqliteClass::exec(const std::string &query,
                      std::function<bool(sqliteClassStmt &)> rowHandler)
{
    sqliteClassStmt stmt(*this, query);
    return exec(stmt, rowHandler);

}

int sqliteClass::exec(
    sqliteClassStmt& stmt,
    std::function<bool(sqliteClassStmt &)> rowHandler)
{
    while (1)
    {
        int rc = stmt.step();
        if (rc == SQLITE_DONE)
            return 0;
        else if (rc != SQLITE_ROW)
            return rc;
        rowHandler(stmt);
    }
}

// sqliteClassStmt *sqliteClass::prepare(const std::string &query)
// {
//     return new sqliteClassStmt(*this, query);
// }

// int sqliteClass::bind(sqliteClassStmt *stmt, int index, int value)
// {
//     return stmt->bind(index, value);
// }