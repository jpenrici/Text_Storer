#include "model.hpp"

TextModel::TextModel()
{
    conn.command(database,
                 "CREATE TABLE IF NOT EXISTS Storage ("  \
                 "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
                 "text1 TEXT NOT NULL," \
                 "text2 TEXT NOT NULL);");
}

bool TextModel::add(string text1, string text2)
{
    if (text1 == "" || text2 == "") {
        return false;
    }

    string sql = "INSERT INTO Storage (text1, text2) VALUES ";
    sql += "('" + text1 + "','"  + text2 + "');";
    conn.command(database, sql.c_str());

    return true;
}

vector<vector<string> > TextModel::search(string field, string value)
{
    vector<vector<string> > result;
    if (field == "" || value == "") {
        result.clear();
        return result;
    }

    string sql;
    if (field == "text1") {
        sql = "SELECT * FROM Storage ";
        sql += "WHERE text1 LIKE '%" + value + "%' ;";
    } else if (field == "text2") {
        sql = "SELECT * FROM Storage ";
        sql += "WHERE text2 LIKE '%" + value + "%' ;";
    } else if (field == "all") {
        sql = "SELECT * FROM Storage ";
        sql += "WHERE text1 LIKE '%" + value + "%' ";
        sql += "OR text2 LIKE '%" + value + "%';";
    } else {
        result.clear();
        return result;
    }

    return conn.command(database, sql.c_str());
}

vector<vector<string> > TextModel::read(string field, string value)
{
    vector<vector<string> > result;
    if (field == "" || value == "") {
        result.clear();
        return result;
    }

    if (field != "ID" && field != "text1" && field != "text2") {
        result.clear();
        return result;
    }

    string sql = "SELECT * FROM Storage WHERE " + field + "='" + value + "';";
    return conn.command(database, sql.c_str());
}

vector<vector<string> > TextModel::read(int id)
{
    return read("ID", to_string(id));
}

vector<vector<string> > TextModel::read()
{
    return conn.command(database, "SELECT * FROM Storage;");
}

bool TextModel::update(int id, string text1, string text2)
{
    if (read(id).size() == 0) {
        return false;
    }

    string sql = "UPDATE Storage set text1='" + text1 + "'";
    sql += " WHERE ID=" + to_string(id) + ";";
    conn.command(database, sql.c_str());

    sql = "UPDATE Storage set text2='" + text2 + "'";
    sql += " WHERE ID=" + to_string(id) + ";";
    conn.command(database, sql.c_str());

    return true;
}

bool TextModel::remove(int id)
{
    if (read(id).size() == 0) {
        return false;
    }

    string sql = "DELETE from Storage where ID=" + to_string(id) + ";";
    conn.command(database, sql.c_str());

    return true;
}
