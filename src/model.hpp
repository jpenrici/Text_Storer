#ifndef __MODELUSER_H__
#define __MODELUSER_H__

#include "connect_db.hpp"
#include <vector>
#include <string>

using namespace std;

class TextModel {

public:
    TextModel();
    ~TextModel() {}

    bool add(string text1, string text2);
    vector<vector<string> > search(string field, string value);
    vector<vector<string> > read(string field, string value);
    vector<vector<string> > read(int id);
    vector<vector<string> > read();
    bool update(int id, string text1, string text2);
    bool remove(int id);

private:
    Connect::SqliteDB conn;
    const char *database = "storage.db";

};

#endif // __MODELUSER_H__