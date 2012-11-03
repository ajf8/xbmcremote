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

#ifndef BROWSE_MODEL_H_
#define BROWSE_MODEL_H_

#include <gtkmm.h>

namespace XbmcRemote {

#define TYPE_SOURCE (1 << 0)
#define TYPE_PARENT (1 << 1)
#define TYPE_FILE (1 << 2)
#define TYPE_DIRECTORY (1 << 3)
#define TYPE_ROOT (1 << 4)

class BrowseBreadcrumb {
public:
  Glib::ustring name;
  Glib::ustring path;
  unsigned int type;
};

class BrowseModel;
typedef sigc::signal<void, BrowseModel&, BrowseBreadcrumb&> BreadcrumbAddedSignal;
typedef sigc::signal<void, BrowseModel&> BreadcrumbPopSignal;

class BrowseModel : public Gtk::ListStore {
public:
  void add_breadcrumb(BrowseBreadcrumb& crumb);
  void pop_breadcrumb();
  void pop_breadcrumbs(unsigned int n);

  BreadcrumbAddedSignal signal_breadcrumb_added();
  BreadcrumbPopSignal signal_breadcrumb_pop();
protected:
  BrowseModel();
  std::vector<BrowseBreadcrumb> m_breadcrumbs;
  BreadcrumbAddedSignal m_signal_breadcrumb_added;
  BreadcrumbPopSignal m_signal_breadcrumb_pop;
};

class BrowseModelColumns : public Gtk::TreeModel::ColumnRecord
{
public:
  BrowseModelColumns() {
    add(m_col_label);
    add(m_col_path);
    add(m_col_thumbnail);
    add(m_col_type);
    add(m_col_playcount);
  }

  Gtk::TreeModelColumn<Glib::ustring> m_col_thumbnail;
  Gtk::TreeModelColumn<Glib::ustring> m_col_label;
  Gtk::TreeModelColumn<Glib::ustring> m_col_path;
  Gtk::TreeModelColumn<unsigned int> m_col_type;
  Gtk::TreeModelColumn<unsigned int> m_col_playcount;
};

}

#endif /* BROWSE_MODEL_H_ */
