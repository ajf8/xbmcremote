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

#include <iostream>
#include <jsoncpp/value.h>

#include "client.h"
#include "browse-model.h"
#include "browse-movies-model.h"

namespace XbmcRemote {

BrowseMoviesModel::BrowseMoviesModel() :
    m_cols() {
  set_column_types(m_cols);
}

BrowseMoviesModelColumns& BrowseMoviesModel::columns() {
  return m_cols;
}

void BrowseMoviesModel::update(JsonPtr json) {
// result -> sources -> file
  const Json::Value &result = (*json)["result"];
  std::string itemKey("");

  clear();

  if (m_breadcrumbs.size() >= 2) {
    Gtk::TreeModel::Row parentRow = *append();
    parentRow[m_cols.m_col_label] = "..";
    parentRow[m_cols.m_col_type] = TYPE_PARENT;
    parentRow[m_cols.m_col_path] = m_breadcrumbs[m_breadcrumbs.size() - 2].path;
  }

  if (result.isMember("sources")) {
    itemKey = "sources";
  } else if (result.isMember("files")) {
    itemKey = "files";
  } else {
    return;
  }

  const Json::Value &items = result[itemKey];

  for (unsigned int i = 0; i < items.size(); i++) {
    const Json::Value &item = items[i];
    Gtk::TreeModel::Row row = *append();
    row[m_cols.m_col_label] = item["label"].asString();
    row[m_cols.m_col_thumbnail] = item["thumbnail"].asString();
    row[m_cols.m_col_path] = item["file"].asString();
    if (itemKey == "files") {
      Glib::ustring fileType = item["filetype"].asString();
      if (fileType == "directory") {
        row[m_cols.m_col_type] = TYPE_DIRECTORY;
      } else if (fileType == "file") {
        row[m_cols.m_col_type] = TYPE_FILE;
      }
    } else if (itemKey == "sources") {
      row[m_cols.m_col_type] = TYPE_SOURCE;
    }
  }
}

}
