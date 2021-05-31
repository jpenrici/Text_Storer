#include "connect_db.hpp"

namespace Connect {

Table SqliteDB::command(const char *database, const char *sql)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;

    // Output: two-dimensional vector
    Table result;

    // int sqlite3_open(const char *filename, sqlite3 **ppDb)
    rc = sqlite3_open(database, &db);
    if (rc) {
        // const char *sqlite3_errmsg(sqlite3*)
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return result;
    }

    // int sqlite3_prepare_v2(sqlite3 *db, const char *zSql, int nByte,
    //                        sqlite3_stmt **ppStmt, const char **pzTail)
    rc = sqlite3_prepare(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return result;
    }

    // int sqlite3_step(sqlite3_stmt*)
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::vector<std::string> row;
        // int sqlite3_column_count(sqlite3_stmt *pStmt)
        int column = sqlite3_column_count(stmt);
        for (int i = 0; i < column; i++) {
            // const unsigned char *sqlite3_column_text(sqlite3_stmt*, int iCol);
            row.push_back(std::string((const char *) sqlite3_column_text(stmt, i)));
        }
        result.push_back(row);
    }

    // sqlite3_finalize(sqlite3_stmt *pStmt)
    sqlite3_finalize(stmt);

    // sqlite3_close(sqlite3*)
    sqlite3_close(db);

    return result;
}
}
