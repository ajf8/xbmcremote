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

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <boost/shared_ptr.hpp>
#include <gtkmm.h>

#include "util.h"
#include "xvfs-cache.h"
#include "xvfs-cache-object.h"

namespace XbmcRemote {

XVFSCache::XVFSCache() :
    m_threadPool(3),
    m_refSettings(Util::gsettings_create())
{
}

boost::shared_ptr<XVFSCacheObject> XVFSCache::get(const Glib::ustring &vfspath)
{
  boost::shared_ptr<XVFSCacheObject> obj_ptr(new XVFSCacheObject(vfspath));
  std::map<const Glib::ustring, boost::shared_ptr<XVFSCacheObject> >::iterator iter = find(vfspath);
  if (iter != end())
    return iter->second;
  else
    return (*this)[vfspath] = obj_ptr;
}

size_t ImageCallback(std::stringstream &contentstream, char* ptr, size_t size,
    size_t nmemb) {
  gsize realsize = size * nmemb;
  contentstream.write(ptr, realsize);
  return realsize;
}

void XVFSCache::fetch_threadfunc(boost::shared_ptr<XVFSCacheObject> obj)
{
  if (obj->get_state() != CS_FETCHING)
    return;

  try {
    std::stringstream contentstream;

    curlpp::Cleanup cleaner;
    curlpp::Easy request;

    curlpp::types::WriteFunctionFunctor functor(
        utilspp::BindFirst(utilspp::make_functor(&ImageCallback), contentstream));
    curlpp::options::WriteFunction *test = new curlpp::options::WriteFunction(
        functor);
    request.setOpt(test);

    request.setOpt(new curlpp::options::Url(build_vfs_url(obj->get_path())));
    request.setOpt(new curlpp::options::Verbose(true));
    request.perform();

    Glib::RefPtr<Gdk::PixbufLoader> loader = Gdk::PixbufLoader::create("jpeg",
        false);
    const std::string &contentstream_str = contentstream.str();
    const char *contentstream_cs = contentstream_str.c_str();
    loader->write(reinterpret_cast<const guint8*>(contentstream_cs),
        contentstream_str.length());
    obj->set_unscaled_pixbuf(loader->get_pixbuf());
    loader->close();
    obj->set_state(CS_CACHED);
  } catch (const std::exception &ex) {
    obj->set_state(CS_ERROR);
  }
}

void XVFSCache::fetch_async(boost::shared_ptr<XVFSCacheObject> obj)
{
  obj->set_state(CS_FETCHING);
  m_threadPool.push(sigc::bind(sigc::mem_fun(*this, &XVFSCache::fetch_threadfunc), obj));
}

Glib::ustring XVFSCache::build_vfs_url(const Glib::ustring &vfslocation)
{
  return Glib::ustring::format("http://", m_refSettings->get_string("hostname"),
      ":", m_refSettings->get_int("web-port"), "/vfs/", vfslocation);
}

}
