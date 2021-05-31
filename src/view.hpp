/*
 * Reference:
 *    https://www.gtkmm.org
 */
#ifndef __GTKMM_APP_H__
#define __GTKMM_APP_H__

#include <gtkmm.h>

class GUIApp : public Gtk::ApplicationWindow {

public:
    GUIApp(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refGlade);
    virtual ~GUIApp();

protected:
    Glib::RefPtr<Gtk::Builder> m_refGlade;
};

#endif // __GTKMM_APP_H__
