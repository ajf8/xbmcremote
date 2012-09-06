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

#include <exception>

#include <gtkmm/main.h>
#include <jsoncpp/value.h>
#include <jsoncpp/writer.h>

#include "playlist-model.h"

namespace XbmcRemote {

  PlaylistModel::PlaylistModel() : m_cols()
  {
    set_column_types(m_cols);
  }

  PlaylistModelColumns& PlaylistModel::columns()
  {
    return m_cols;
  }

  void PlaylistModel::populate(const Json::Value &items)
  {
    clear();

    for(unsigned int i = 0; i < items.size(); i++)
    {
      const Json::Value &item = items[i];
      Gtk::TreeModel::Row row = *append();
      row[m_cols.m_col_label] = item["label"].asString();
      row[m_cols.m_col_thumbnail] = item["thumbnail"].asString();
      row[m_cols.m_col_type] = item["type"].asString();
    }
  }
}

