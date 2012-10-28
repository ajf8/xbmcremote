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

BrowseMoviesModel::BrowseMoviesModel() : m_cols()
{
  set_column_types(m_cols);
}

BrowseMoviesModelColumns& BrowseMoviesModel::columns()
{
  return m_cols;
}

void BrowseMoviesModel::update(JsonPtr json) {
// result -> sources -> file
  const Json::Value &result = (*json)["result"];
  const Json::Value &sources = result["sources"];

  for (unsigned int i = 0; i < sources.size(); i++) {
    const Json::Value &item = sources[i];
    Gtk::TreeModel::Row row = *append();
    row[m_cols.m_col_label] = item["label"].asString();
    std::cout << "adding " << item["label"].asString() << std::endl;
    row[m_cols.m_col_path] = item["path"].asString();
  }
}

}
