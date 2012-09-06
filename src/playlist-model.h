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

#ifndef PLAYLIST_MODEL_H_
#define PLAYLIST_MODEL_H_

#include <gtkmm/treemodel.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treerowreference.h>
#include <jsoncpp/value.h>

namespace XbmcRemote {

class PlaylistModelColumns : public Gtk::TreeModel::ColumnRecord
{
public:
  PlaylistModelColumns() {
    add(m_col_label);
    add(m_col_type);
    add(m_col_thumbnail);
  }

  Gtk::TreeModelColumn<Glib::ustring> m_col_label;
  Gtk::TreeModelColumn<Glib::ustring> m_col_type;
  Gtk::TreeModelColumn<Glib::ustring> m_col_thumbnail;
};

class PlaylistModel : public Gtk::ListStore
{
public:
  PlaylistModel();
  PlaylistModelColumns& columns();
  void populate(const Json::Value &items);
private:
  PlaylistModelColumns m_cols;
};

}

#endif /* PLAYLIST_MODEL_H_ */
