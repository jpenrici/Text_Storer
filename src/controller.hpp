#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "view.hpp"
#include "model.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>

using namespace std;


class Controller {

public:
    ~Controller() {}

    int Gui();

    static Controller *Instance()
    {
        if (instance == 0) {
            instance = new Controller();
            return instance;
        }

        return instance;
    }

private:
    Controller() {}

    // Singleton
    static Controller *instance;

    // INFORM
    const vector<string> INFO {
        "There's something wrong! Check data.",     // ERROR_UNDEFINED
        "Successful action!",                       // OK
        "There's something wrong! Check text.",     // ERROR_text
        "There's something wrong! Check id.",       // ERROR_ID
    };

    enum Error {
        ERROR_UNDEFINED,
        NO_ERROR,
        ERROR_TEXT,
        ERROR_ID,
    };

    // MODEL CONTROL
    TextModel info;
    int add_info(string text1, string text2);
    int update_info(string id, string text1, string text2);
    int remove_info(string id);
    vector<vector<string> > read_info();
    vector<vector<string> > search_info(string field, string value);

    // GUI VIEW CONTROL
    GUIApp *pGui;
    Gtk::Button *pBtn_insert;
    Gtk::Button *pBtn_update;
    Gtk::Button *pBtn_delete;
    Gtk::Button *pBtn_export;
    Gtk::Button *pBtn_search;
    Gtk::Button *pBtn_clean;
    Gtk::Label *pLbl_id;
    Gtk::Label *pLbl_text1;
    Gtk::Label *pLbl_text2;
    Gtk::Label *pLbl_inform;
    Gtk::Label *pLbl_status;
    Gtk::SearchEntry *pTxt_search;
    Gtk::Entry *pTxt_text1;
    Gtk::Entry *pTxt_text2;
    Gtk::TreeView *pTreeView;
    Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
    Glib::RefPtr<Gtk::TreeSelection> m_TreeSelection;

    void gui_insert();
    void gui_update();
    void gui_delete();
    void gui_export();
    void gui_search();
    void gui_view();
    void gui_clean();
    void gui_select_row_treeView();
    void gui_update_treeView(vector<vector<string> > result);

    class ModelColumns : public Gtk::TreeModel::ColumnRecord {

    public:
        ModelColumns()
        {
            add(m_col_0);
            add(m_col_1);
            add(m_col_2);
        };
        virtual ~ModelColumns() {};

        Gtk::TreeModelColumn<Glib::ustring> m_col_0;
        Gtk::TreeModelColumn<Glib::ustring> m_col_1;
        Gtk::TreeModelColumn<Glib::ustring> m_col_2;
    };

    ModelColumns m_Columns;

    const string CSV_PATH = "csvStorage.csv";
    const string DELIM = ";";
};

#endif // __CONTROLLER_H__
