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

#include "xvfs-cache-object.h"

namespace XbmcRemote {

XVFSCacheObject::XVFSCacheObject(const Glib::ustring &path) :
    m_path(path),
    m_state(CS_NEW),
    m_last_state(CS_NEW),
    m_scaled_to(-1)
{
}

XVFSCacheObject::CacheObjectStateChangeSignal XVFSCacheObject::signal_state_change()
{
  return m_signal_state_change;
}

void XVFSCacheObject::set_state_main_loop()
{
  m_signal_state_change(*this);
}

void XVFSCacheObject::set_state(CacheObjectState state)
{
  m_last_state = m_state;
  m_state = state;
  Glib::signal_idle().connect_once(sigc::mem_fun(*this, &XVFSCacheObject::set_state_main_loop));
}

CacheObjectState XVFSCacheObject::get_state()
{
  return m_state;
}

Glib::RefPtr<Gdk::Pixbuf> XVFSCacheObject::get_unscaled_pixbuf()
{
  return m_unscaledPixbuf;
}

void XVFSCacheObject::set_unscaled_pixbuf(Glib::RefPtr<Gdk::Pixbuf> pixbuf)
{
  m_unscaledPixbuf = pixbuf;
}

Glib::ustring XVFSCacheObject::get_path()
{
  return m_path;
}

Glib::RefPtr<Gdk::Pixbuf> XVFSCacheObject::scale(double scale)
{
  if (scale == m_scaled_to) {
    return m_scaledPixbuf;
  } else {
    double max_img_size = std::max(m_unscaledPixbuf->get_width(), m_unscaledPixbuf->get_height());
    int scaled_width = int(scale * (m_unscaledPixbuf->get_width() / max_img_size));
    int scaled_height = int(scale * (m_unscaledPixbuf->get_height() / max_img_size));

    m_scaledPixbuf = m_unscaledPixbuf->scale_simple(scaled_width, scaled_height, Gdk::INTERP_BILINEAR);
    m_scaled_to = scale;

    return m_scaledPixbuf;
  }
}

}
