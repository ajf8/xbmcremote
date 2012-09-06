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
#include <gtkmm.h>

#include "browse-cell-renderer.h"
#include "browse-movies-cell-renderer.h"

namespace XbmcRemote {

BrowseMoviesCellRenderer::BrowseMoviesCellRenderer() :
    Glib::ObjectBase( typeid(BrowseMoviesCellRenderer) )
{
}

void BrowseMoviesCellRenderer::render_vfunc(const Cairo::RefPtr<Cairo::Context>& cr,
    Gtk::Widget& widget, const Gdk::Rectangle& background_area,
    const Gdk::Rectangle& cell_area, Gtk::CellRendererState flags)
{
  Gdk::Rectangle title_area, icon_area, fill_area;
  title_area = icon_area = fill_area = background_area;

  int xpad, ypad;
  get_padding(xpad, ypad);

  //Gdk::Rectangle pixbuf_area = background_area;

  title_area.set_x(xpad);
  title_area.set_y(ypad);
  title_area.set_height(title_area.get_height()+ypad);
  title_area.set_width(title_area.get_width()+xpad);

  m_titleTextRenderer.property_text() = property_title().get_value();
  m_titleTextRenderer.render(cr, widget, title_area, title_area, flags);
}

}
