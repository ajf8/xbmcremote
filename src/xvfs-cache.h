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

#ifndef XVFS_CACHE_H_
#define XVFS_CACHE_H_

#include <map>
#include <boost/function.hpp>
#include <gdkmm.h>
#include <glibmm.h>
#include <boost/shared_ptr.hpp>

#include "xvfs-cache-object.h"

namespace XbmcRemote {

class XVFSCache : public std::map<Glib::ustring, boost::shared_ptr<XVFSCacheObject> > {
public:
  XVFSCache();
  boost::shared_ptr<XVFSCacheObject> get(const Glib::ustring &vfspath);
  void fetch_async(boost::shared_ptr<XVFSCacheObject> obj);
protected:
  Glib::ThreadPool m_threadPool;
  Glib::RefPtr<Gio::Settings> m_refSettings;
  Glib::ustring build_vfs_url(const Glib::ustring &vfslocation);
private:
  void fetch_threadfunc(boost::shared_ptr<XVFSCacheObject> obj);
};

}

#endif /* XVFS_CACHE_H_ */
