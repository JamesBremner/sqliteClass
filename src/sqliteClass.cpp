#include <stdexcept>

#include "sqliteClass.h"
namespace raven {
int sqliteClass::exec(const std::string &query,
                      std::function<bool(sqliteClassStmt &)> rowHandler)
{
    sqliteClassStmt stmt(db, query);
    return exec(&stmt, rowHandler);
}

int sqliteClass::exec(
    sqliteClassStmt *stmt)
{
    while (1)
    {
        int rc = stmt->step();
        if (rc == SQLITE_DONE) {
            stmt->reset();
            return 0;
        }
    }
}
int sqliteClass::exec(
    sqliteClassStmt *stmt,
    std::function<bool(sqliteClassStmt &)> rowHandler)
{
    while (1)
    {
        int rc = stmt->step();
        if (rc == SQLITE_DONE) {
            stmt->reset();
            return 0;
        }
        else if (rc != SQLITE_ROW)
            return rc;
        rowHandler(*stmt);
    }
}

sqliteClassStmt *sqliteClass::prepare(const std::string &query)
{
    return new sqliteClassStmt(db, query);
}

int sqliteClass::bind(sqliteClassStmt *stmt, int index, int value)
{
    return stmt->bind(index, value);
}
int sqliteClass::reset(sqliteClassStmt *stmt)
{
    return stmt->reset();
}
}
