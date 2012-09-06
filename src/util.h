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

#include <glibmm.h>
#include <gtkmm.h>

#include <jsoncpp/value.h>

#ifndef UTIL_H_
#define UTIL_H_

namespace XbmcRemote
{

class Util
{
private:
  Util();
public:
  static Glib::ustring build_glade_path(const Glib::ustring &file);
  static Glib::RefPtr<Gio::Settings> gsettings_create();
  static long json_time_to_seconds(const Json::Value &time);
  static Glib::ustring seconds_to_short_time(long time);
};

}

#endif /* UTIL_H_ */
