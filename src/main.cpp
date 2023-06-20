#include <iostream>
#include <string>

#include "sqliteClass.h"

int main(int argc, char *argv[])
{
    // construct test database

    raven::sqliteClass DB;
    if (DB.open("test.dat"))
    {
        std::cout << "cannot open database\n";
        exit(2);
    }
    DB.exec("CREATE TABLE IF NOT EXISTS like ( userid, likeid );");
    DB.exec("DELETE FROM like;");
    DB.exec("INSERT INTO like VALUES "
            "(1,1),(1,2),"
            "(2,2),(2,3),"
            "(3,3),(3,1),"
            "(4,3),(4,1);");

    // query with bound values
    int owner = 1;
    std::string ownerInterests = "1,2";
    std::string query = "SELECT userid,likeid "
                        "FROM like "
                        "WHERE userid != ? "
                        " AND likeid IN ( " +
                        ownerInterests + " );";
    auto stmt = DB.prepare( query );
    DB.bind(stmt,1,owner);
    int found = 0;
    DB.exec(stmt,
            [&](raven::sqliteClassStmt& stmt) -> bool
            {
                std::cout << stmt.column_int(0)
                          << " | " << stmt.column_int(0)
                          << "\n";
                found++;
                return true;
            });
    std::cout << found << " rows found\n";
    if (found != 3)
    {
        std::cout << "error\n";
    }

    // query
    owner = 2;
    query = "SELECT userid,likeid "
            "FROM like "
            "WHERE userid != " +
            std::to_string(owner) +
            " AND likeid IN ( " + ownerInterests + " );";

    found = 0;
    DB.exec(query,
            [&](raven::sqliteClassStmt& stmt) -> bool
            {
                std::cout << stmt.column_int(0)
                          << " | " << stmt.column_int(0)
                          << "\n";
                found++;
                return true;
            });
    std::cout << found << " rows found\n";
    if (found != 4)
    {
        std::cout << "error\n";
    }
    return 0;
}