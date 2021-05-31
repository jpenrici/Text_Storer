#ifndef CONNECTSQLITE_H
#define CONNECTSQLITE_H

#include <sqlite3.h>
#include <stdio.h>
#include <vector>
#include <string>

namespace Connect {

using Table = std::vector<std::vector<std::string> >;

/*
 * Reference:
 *    https://www.sqlite.org
 *
 * Connects to SQLite3 database.
 * Executes and returns SQL command information.
 *
 */
class SqliteDB {

public:
    Table command(const char *database, const char *sql);
};
}

#endif // CONNECTSQLITE_H
