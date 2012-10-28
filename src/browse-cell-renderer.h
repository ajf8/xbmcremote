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

#ifndef BROWSE_CELL_RENDERER_H_
#define BROWSE_CELL_RENDERER_H_

#include <glibmm.h>
#include <gdkmm.h>

namespace XbmcRemote
{

class BrowseCellRenderer : public Gtk::CellRenderer
{
public:
  Glib::PropertyProxy<Glib::ustring> property_thumbnail();
  Glib::PropertyProxy<Glib::ustring> property_label();
  Glib::PropertyProxy<Glib::ustring> property_path();
protected:
  BrowseCellRenderer();
  Glib::Property< Glib::ustring > m_property_thumbnail;
  Glib::Property< Glib::ustring > m_property_label;
  Glib::Property< Glib::ustring > m_property_path;
  Gtk::CellRendererText m_titleTextRenderer;
  Gtk::CellRendererPixbuf m_pixbufRenderer;
};

}

#endif /* BROWSE_CELL_RENDERER_H_ */
