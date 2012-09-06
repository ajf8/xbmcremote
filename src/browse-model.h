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

class BrowseModelColumns : public Gtk::TreeModel::ColumnRecord
{
public:
  BrowseModelColumns() {
    add(m_col_thumbnail);
    add(m_col_playcount);
    add(m_col_title);
  }

  Gtk::TreeModelColumn<Glib::ustring> m_col_thumbnail;
  Gtk::TreeModelColumn<Glib::ustring> m_col_title;
  Gtk::TreeModelColumn<unsigned int> m_col_playcount;
};

class BrowseModel : public Gtk::ListStore {
protected:
  BrowseModel();
};

}

#endif /* BROWSE_MODEL_H_ */
