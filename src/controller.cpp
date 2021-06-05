#include "controller.hpp"

/*
 * GUI VIEW CONTROL
 */
Controller *Controller::instance = 0;

int Controller::Gui()
{
    auto app = Gtk::Application::create("org.gtkmm.example");

    auto refBuilder = Gtk::Builder::create();
    try {
        refBuilder->add_from_file("TextStorerGui.glade");
    }
    catch (const Glib::FileError &ex) {
        cerr << "FileError: " << ex.what() << endl;
        return EXIT_FAILURE;
    }
    catch (const Glib::MarkupError &ex) {
        cerr << "MarkupError: " << ex.what() << endl;
        return EXIT_FAILURE;
    }
    catch (const Gtk::BuilderError &ex) {
        cerr << "BuilderError: " << ex.what() << endl;
        return EXIT_FAILURE;
    }

    pGui = nullptr;
    pBtn_insert = nullptr;
    pBtn_update = nullptr;
    pBtn_delete = nullptr;
    pBtn_export = nullptr;
    pBtn_search = nullptr;
    pBtn_clean = nullptr;
    pBtn_file = nullptr;
    pLbl_id = nullptr;
    pLbl_text1 = nullptr;
    pLbl_text2 = nullptr;
    pLbl_inform = nullptr;
    pLbl_status = nullptr;
    pTxt_search = nullptr;
    pTxt_text1 = nullptr;
    pTxt_text2 = nullptr;
    pTreeView = nullptr;

    try {
        refBuilder->get_widget_derived("appWindow", pGui);
        refBuilder->get_widget("btn_insert", pBtn_insert);
        refBuilder->get_widget("btn_update", pBtn_update);
        refBuilder->get_widget("btn_delete", pBtn_delete);
        refBuilder->get_widget("btn_export", pBtn_export);
        refBuilder->get_widget("btn_search", pBtn_search);
        refBuilder->get_widget("btn_clean", pBtn_clean);
        refBuilder->get_widget("btn_file", pBtn_file);
        refBuilder->get_widget("lbl_id", pLbl_id);
        refBuilder->get_widget("lbl_text1", pLbl_text1);
        refBuilder->get_widget("lbl_text2", pLbl_text2);
        refBuilder->get_widget("lbl_inform", pLbl_inform);
        refBuilder->get_widget("lbl_status", pLbl_status);
        refBuilder->get_widget("txt_search", pTxt_search);
        refBuilder->get_widget("txt_text1", pTxt_text1);
        refBuilder->get_widget("txt_text2", pTxt_text2);
        refBuilder->get_widget("treeView", pTreeView);

        pBtn_insert->signal_clicked().connect(sigc::mem_fun(*this,
                                              &Controller::gui_insert));

        pBtn_update->signal_clicked().connect(sigc::mem_fun(*this,
                                              &Controller::gui_update));

        pBtn_delete->signal_clicked().connect(sigc::mem_fun(*this,
                                              &Controller::gui_delete));

        pBtn_export->signal_clicked().connect(sigc::mem_fun(*this,
                                              &Controller::gui_export));

        pBtn_search->signal_clicked().connect(sigc::mem_fun(*this,
                                              &Controller::gui_search));

        pBtn_clean->signal_clicked().connect(sigc::mem_fun(*this,
                                             &Controller::gui_clean));

        pBtn_file->signal_selection_changed().connect(sigc::mem_fun(*this,
                &Controller::gui_file));

        pLbl_text1->set_text(LABEL_TEXT1);
        pLbl_text2->set_text(LABEL_TEXT2);

        m_refTreeModel = Gtk::ListStore::create(m_Columns);
        pTreeView->set_model(m_refTreeModel);

        m_TreeSelection = pTreeView->get_selection();
        m_TreeSelection->signal_changed().connect(sigc::mem_fun(*this,
                &Controller::gui_select_row_treeView));

        auto filter_sqlite3 = Gtk::FileFilter::create();
        filter_sqlite3->set_name("SQLite3 files");
        filter_sqlite3->add_pattern("*.db");
        pBtn_file->add_filter(filter_sqlite3);

        // auto filter_any = Gtk::FileFilter::create();
        // filter_any->set_name("Any files");
        // filter_any->add_pattern("*");
        // pBtn_file->add_filter(filter_any);

    }
    catch (exception &e) {
        cout << "There was an unexpected error.\n" << e.what() << endl;
        return EXIT_FAILURE;
    }

    if (pGui) {
        cout << "GUI started." << endl;
        gui_clean();
        app->run(*pGui);
        cout << "GUI stoped." << endl;
    }

    delete pBtn_insert;
    delete pBtn_update;
    delete pBtn_delete;
    delete pBtn_export;
    delete pBtn_search;
    delete pBtn_clean;
    delete pBtn_file;
    delete pLbl_id;
    delete pLbl_text1;
    delete pLbl_text2;
    delete pLbl_inform;
    delete pLbl_status;
    delete pTxt_search;
    delete pTxt_text1;
    delete pTxt_text2;
    delete pTreeView;
    delete pGui;

    return EXIT_SUCCESS;
}

void Controller::gui_insert()
{
    string title = pTxt_text1->get_text();
    string subject = pTxt_text2->get_buffer()->get_text();
    int result = add_info(title, subject);

    if (result == 1) {
        gui_clean();
    }
    pLbl_status->set_text(INFO[result]);
}

void Controller::gui_update()
{
    string id = pLbl_id->get_text();
    string title = pTxt_text1->get_text();
    string subject = pTxt_text2->get_buffer()->get_text();
    int result = update_info(id, title, subject);

    if (result == 1) {
        gui_clean();
    }
    pLbl_status->set_text(INFO[result]);
}

void Controller::gui_search()
{
    string search = pTxt_search->get_text();
    vector<vector<string> > result = search_info("all", search);
    vector<vector<string> > table {{"ID", pLbl_text1->get_text(), pLbl_text2->get_text()}};
    table.insert(end(table), begin(result), end(result));
    gui_update_treeView(table);
}

void Controller::gui_view()
{
    vector<vector<string> > result = read_info();
    vector<vector<string> > table {{"ID", pLbl_text1->get_text(), pLbl_text2->get_text()}};
    table.insert(end(table), begin(result), end(result));
    gui_update_treeView(table);
}

void Controller::gui_delete()
{
    string id = pLbl_id->get_text();
    int result = remove_info(id);

    if (result == 1) {
        gui_clean();
    }
    pLbl_status->set_text(INFO[result]);
}

void Controller::gui_export()
{
    vector<vector<string> > result = read_info();

    if (result.empty()) {
        pLbl_status->set_text("Nothing to export.");
        return;
    }

    vector<vector<string> > table;
    table.push_back({"#ID", pLbl_text1->get_text(), pLbl_text2->get_text()});
    table.insert(end(table), begin(result), end(result));
    ofstream csvOut(csv_path);

    unsigned int start = 1;
    if (EXPORT_ALL) {
        start = 0;
    }

    string str;
    for (auto item : table) {
        str = "";
        for (unsigned int i = start; i < item.size(); ++i) {
            str += item[i];
            if (i < item.size() - 1) {
                str += DELIM;
            }
        }
        csvOut << str << '\n';
    }
    csvOut.close();

    // Output
    str = "saved: " +  csv_path;
    pLbl_status->set_text(str);
    cout << str << '\n';
}

void Controller::gui_clean()
{
    gui_view();

    pTxt_text1->set_sensitive(true);
    pTxt_text2->set_sensitive(true);

    pBtn_insert->set_sensitive(true);
    pBtn_update->set_sensitive(false);
    pBtn_delete->set_sensitive(false);

    pTxt_text1->set_text("");
    pTxt_text2->get_buffer()->set_text("");

    pLbl_id->set_text("");
}

void Controller::gui_file()
{
    string database = pBtn_file->get_filename();

    int result = set_db(database);
    if (result) {
        gui_clean();
    }

    cout << "Current: " << database << '\n';
}

void Controller::gui_select_row_treeView()
{
    pTxt_text1->set_sensitive(true);
    pTxt_text2->set_sensitive(true);

    pBtn_insert->set_sensitive(false);
    pBtn_update->set_sensitive(true);
    pBtn_delete->set_sensitive(true);

    auto iter = m_TreeSelection->get_selected();
    if (iter) {
        auto row = *iter;
        pLbl_id->set_text(row[m_Columns.m_col_0]);
        pTxt_text1->set_text(row[m_Columns.m_col_1]);
        pTxt_text2->get_buffer()->set_text(row[m_Columns.m_col_2]);
    }

    pLbl_inform->set_text("Update or Remove info.");
}

void Controller::gui_update_treeView(vector<vector<string> > result)
{
    m_TreeSelection->unselect_all();
    m_refTreeModel->clear();

    pLbl_inform->set_text(get_db());
    if (result.size() < 2) {
        pLbl_status->set_text("No info to display.");
        return;
    }
    else {
        pLbl_status->set_text(to_string(result.size() - 1) + " values found.");
    }

    Gtk::TreeModel::Row row;
    for (unsigned int i = 1; i < result.size(); ++i) {
        row = *(m_refTreeModel->append());
        row[m_Columns.m_col_0] = result[i][0];
        row[m_Columns.m_col_1] = result[i][1];
        row[m_Columns.m_col_2] = result[i][2];
    }

    pTreeView->remove_all_columns();
    if (result.size() != 0) {
        pTreeView->append_column(result[0][0], m_Columns.m_col_0);
        pTreeView->append_column(result[0][1], m_Columns.m_col_1);
        pTreeView->append_column(result[0][2], m_Columns.m_col_2);
    }
}

/*
 * MODEL CONTROL
 */
string Controller::get_db()
{
    return info.get();
}

int Controller::set_db(string database)
{
    if (database == "") {
        return ERROR_UNDEFINED;
    }

    info.set(database);
    return NO_ERROR;
}

int Controller::add_info(string title, string subject)
{
    if (title == "" || subject == "") {
        return ERROR_TEXT;
    }

    return info.add(title, subject);
}

vector<vector<string> > Controller::search_info(string field, string value)
{
    vector<vector<string> > result;
    if (field == "" || value == "") {
        result.clear();
        return result;
    }

    return info.search(field, value);
}

vector<vector<string> > Controller::read_info()
{
    return info.read();
}

int Controller::update_info(string id, string title, string subject)
{
    if (id == "") {
        return ERROR_ID;
    }

    if (title == "" || subject == "") {
        return ERROR_TEXT;
    }

    return info.update(stoi(id), title, subject);
}

int Controller::remove_info(string id)
{
    if (id == "") {
        return ERROR_ID;
    }

    return info.remove(stoi(id));
}
