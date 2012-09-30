/*
 * xbmcremote
 * Copyright (C) 2012  Alan Fitton

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "gtkmm.h"
#include "client.h"

#ifndef PREFERENCES_DIALOG_H_
#define PREFERENCES_DIALOG_H_

namespace XbmcRemote {

enum PreferencesPage {
  PP_CONNECTION,
  PP_DESKTOP
};

class PreferencesDialog : public Gtk::Dialog {
public:
  PreferencesDialog(Client &client);
protected:
  class PageColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:

    PageColumns()
    { add(m_col_name); add(m_col_id); }

    Gtk::TreeModelColumn<Glib::ustring> m_col_name;
    Gtk::TreeModelColumn<PreferencesPage> m_col_id;
  };

  void on_bound_sensitive_active_toggled(Gtk::CheckButton *check, Gtk::Widget *widget);
  void bind_active_sensitive(Gtk::CheckButton *check, Gtk::Widget *widget);
  Gtk::TreeModel::iterator add_page(PreferencesPage ppid, const Glib::ustring &name);
  Gtk::TreeModel::iterator populate_page_store();
  void on_client_connection_change(Client &client, Gtk::Button *connect_button, Gtk::Button *disconnect_button, bool criteria);
  Gtk::Widget* construct_connection_page();
  void connect_clicked();
  void disconnect_clicked();
  void page_selection_changed();
  bool header_draw(const Cairo::RefPtr<Cairo::Context>& cr);

  PageColumns m_pageStoreCols;
  Glib::RefPtr<Gtk::ListStore> m_refPageStore;
  Glib::RefPtr<Gio::Settings> m_refSettings;
  Gtk::Label *m_pluginLabel;
  Gtk::TreeView *m_pluginTree;
  Gtk::EventBox *m_pluginLabelBox;
  Gtk::Container *m_pluginContainer;
  Gtk::Widget *m_currentPage;
  //Gtk::Button *m_closeButton;
  Client &m_client;
};

}

#endif /* PREFERENCES_DIALOG_H_ */
