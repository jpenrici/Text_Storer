#ifndef __MODELUSER_H__
#define __MODELUSER_H__

#include "connect_db.hpp"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class TextModel {

public:
    TextModel();
    ~TextModel() {}

    void set(string database);
    string get();

    bool add(string text1, string text2);
    vector<vector<string> > search(string field, string value);
    vector<vector<string> > read(string field, string value);
    vector<vector<string> > read(int id);
    vector<vector<string> > read();
    bool update(int id, string text1, string text2);
    bool remove(int id);

private:
    Connect::SqliteDB conn;
    string current_database;
    const string DEFAULT_DB = "storage.db"; // default

};

#endif // __MODELUSER_H__