#  A thin, header only, C++ wrapper for the sqlite C API

To use this successfully you will need

 - Genereal knowledge of C++17 or later coding
 - Knowledge of the SQLite C API.  This does not hide the API, it is meant to enable succint C++ code without any loss of performance
 - Knowledge of C++17 lambda.  Lambdas are used for executing prepared statements.

## Example Usage
```
#include <iostream>
#include <string>

#include "sqliteClass.h"

int main(int argc, char *argv[])
{
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
    int owner = 1;
    std::string ownerInterests = "1,2";
    std::string query = "SELECT userid,likeid "
                        "FROM like "
                        "WHERE userid != " +
                        std::to_string(owner) +
                        " AND likeid IN ( " + ownerInterests + " );";
    int found = 0;
    DB.exec(query,
            [&](sqlite3_stmt *stmt) -> bool
            {
                std::cout << DB.column_int( stmt, 0 )
                    << " | " <<  DB.column_int( stmt, 0 )
                    << "\n";
                found++;
                return true;
            });
    std::cout << found << " rows found\n";
    return 0;
}
```
