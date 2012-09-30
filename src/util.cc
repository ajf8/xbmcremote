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

#include <string>

#include <boost/format.hpp>
#include <glibmm.h>
#include <giomm.h>
#include "util.h"

namespace XbmcRemote
{

Glib::ustring Util::build_glade_path(const Glib::ustring & file)
{
  return Glib::build_filename(XBMCR_PKGDATADIR, "glade", file);
}

Glib::RefPtr<Gio::Settings> Util::gsettings_create()
{
  return Gio::Settings::create("uk.org.eth0.xbmcremote");
}

long Util::json_time_to_seconds(const Json::Value &time)
{
  return (time["hours"].asUInt()*60*60) + (time["minutes"].asUInt()*60) + time["seconds"].asUInt();
}

std::string Util::seconds_to_short_time(long seconds)
{
  int hours = seconds / 3600;
  int minutes = (seconds % 3600) / 60;
  int secondsPart = (seconds % 3600) % 60;

  return (boost::format("%02d:%02d:%02d") % hours % minutes % secondsPart).str();
}

}
