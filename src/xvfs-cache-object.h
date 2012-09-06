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

#ifndef XVFS_CACHE_OBJECT_H_
#define XVFS_CACHE_OBJECT_H_

#include <glibmm.h>
#include <gdkmm.h>
#include <boost/shared_ptr.hpp>

namespace XbmcRemote {

enum CacheObjectState
{
  CS_NEW,
  CS_FETCHING,
  CS_CACHED,
  CS_ERROR
};

class XVFSCacheObject
{
public:
  XVFSCacheObject(const Glib::ustring &path);
  void set_state_main_loop();
  void set_state(CacheObjectState state);
  CacheObjectState get_state();
  Glib::RefPtr<Gdk::Pixbuf> get_unscaled_pixbuf();
  void set_unscaled_pixbuf(Glib::RefPtr<Gdk::Pixbuf> pixbuf);
  Glib::ustring get_path();
  Glib::RefPtr<Gdk::Pixbuf> scale(double scale);

  typedef sigc::signal<void, XVFSCacheObject&> CacheObjectStateChangeSignal;
  XVFSCacheObject::CacheObjectStateChangeSignal signal_state_change();
protected:
  Glib::RefPtr<Gdk::Pixbuf> m_unscaledPixbuf;
  Glib::RefPtr<Gdk::Pixbuf> m_scaledPixbuf;
  Glib::ustring m_path;
  CacheObjectState m_state;
  CacheObjectState m_last_state;
  double m_scaled_to;
  CacheObjectStateChangeSignal m_signal_state_change;
};

}

#endif /* XVFS_CACHE_OBJECT_H_ */
