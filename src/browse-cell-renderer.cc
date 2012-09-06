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

#include <gdkmm.h>
#include <gtkmm.h>

#include "hig.h"
#include "browse-cell-renderer.h"

namespace XbmcRemote {

BrowseCellRenderer::BrowseCellRenderer() :
    Glib::ObjectBase( typeid(BrowseCellRenderer) ),
    m_property_thumbnail( *this, "thumbnail", "" ),
    m_property_title( *this, "title", "" ),
    m_titleTextRenderer(), m_pixbufRenderer()
{
  set_padding(GUI_PAD_SMALL, GUI_PAD_SMALL);
}

Glib::PropertyProxy<Glib::ustring> BrowseCellRenderer::property_thumbnail()
{
  return m_property_thumbnail.get_proxy();
}

Glib::PropertyProxy<Glib::ustring> BrowseCellRenderer::property_title()
{
  return m_property_title.get_proxy();
}

}
