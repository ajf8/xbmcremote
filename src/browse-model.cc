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

namespace XbmcRemote {

BrowseModel::BrowseModel() : m_breadcrumbs()
{

}

void BrowseModel::add_breadcrumb(BrowseBreadcrumb& crumb)
{
  m_breadcrumbs.push_back(crumb);
}

void BrowseModel::pop_breadcrumbs(unsigned int n)
{
  for (unsigned int i = 0; i < n; i++)
    m_breadcrumbs.pop_back();
}

}
