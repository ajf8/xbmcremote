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

#include "browse-model.h"

#ifndef BROWSE_MOVIES_MODEL_H_
#define BROWSE_MOVIES_MODEL_H_

namespace XbmcRemote {

class BrowseMoviesModelColumns : public BrowseModelColumns
{
public:
  BrowseMoviesModelColumns() {
    add(m_col_rating);
  }

  Gtk::TreeModelColumn<double> m_col_rating;
};

class BrowseMoviesModel : public BrowseModel {
public:
  BrowseMoviesModel();
  BrowseMoviesModelColumns& columns();
protected:
  BrowseMoviesModelColumns m_cols;
};

}

#endif /* BROWSE_TV_MODEL_H_ */
