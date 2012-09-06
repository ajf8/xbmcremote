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

#include <gtkmm/main.h>

#include <glibmm.h>
#include <iomanip>
#include <iostream>

#include "main-window.h"

namespace XbmcRemote {

class ConnectOptionGroup: public Glib::OptionGroup {
public:
  ConnectOptionGroup();

  /*virtual bool
   on_pre_parse(Glib::OptionContext& context, Glib::OptionGroup& group);
   virtual bool
   on_post_parse(Glib::OptionContext& context, Glib::OptionGroup& group);*/
  virtual void
  on_error(Glib::OptionContext& context, Glib::OptionGroup& group);

  //These int instances should live as long as the OptionGroup to which they are added,
  //and as long as the OptionContext to which those OptionGroups are added.
  int m_arg_port;
  Glib::ustring m_arg_host;
  bool m_arg_discover;
};

ConnectOptionGroup::ConnectOptionGroup() :
    Glib::OptionGroup("connect_group", "connect", "connection settings"), m_arg_port(
        8080), m_arg_host(""), m_arg_discover(false) {
  Glib::OptionEntry portEntry;
  portEntry.set_long_name("port");
  portEntry.set_short_name('p');
  portEntry.set_description("Port");
  add_entry(portEntry, m_arg_port);

  Glib::OptionEntry discoverEntry;
  discoverEntry.set_long_name("discover");
  discoverEntry.set_short_name('d');
  discoverEntry.set_description("Discover xbmcrpc using avahi");
  add_entry(discoverEntry, m_arg_discover);

  Glib::OptionEntry hostEntry;
  hostEntry.set_long_name("host");
  hostEntry.set_short_name('h');
  hostEntry.set_description("Hostname");
  add_entry(hostEntry, m_arg_host);
}

/*bool ConnectOptionGroup::on_pre_parse(Glib::OptionContext& context,
 Glib::OptionGroup& group) {
 //This is called before the m_arg_* instances are given their values.
 // You do not need to override this method. This is just here to show you how,
 // in case you want to do any extra processing.
 return Glib::OptionGroup::on_pre_parse(context, group);
 }

 bool ConnectOptionGroup::on_post_parse(Glib::OptionContext& context,
 Glib::OptionGroup& group) {
 //This is called after the m_arg_* instances are given their values.
 // You do not need to override this method. This is just here to show you how,
 // in case you want to do any extra processing.
 return Glib::OptionGroup::on_post_parse(context, group);
 }*/

void ConnectOptionGroup::on_error(Glib::OptionContext& context,
    Glib::OptionGroup& group) {
  Glib::OptionGroup::on_error(context, group);
}

}

int main(int argc, char** argv) {
  using namespace XbmcRemote;

  Glib::init();
  Gtk::Main kit(argc, argv);
  Gdl::init();
  //std::ios_base::sync_with_stdio();

  Glib::OptionContext context;

  ConnectOptionGroup group;
  context.set_main_group(group);

  try {
    context.parse(argc, argv);
  } catch (const Glib::Error& ex) {
    std::cout << "Exception: " << ex.what() << std::endl;
  }

  std::cout << "parsed values: " << std::endl << "  host = " << group.m_arg_host
      << std::endl << "  port = " << group.m_arg_port << std::endl
      << "  discover = " << (group.m_arg_discover ? "enabled" : "disabled")
      << std::endl;

  //This one shows the results of multiple instance of the same option, such as --list=1 --list=a --list=b
  /*std::cout << "  list = ";
   for (Glib::OptionGroup::vecustrings::const_iterator iter =
   group.m_arg_list.begin(); iter != group.m_arg_list.end(); ++iter) {
   std::cout << *iter << ", ";
   }

   std::cout << std::endl;*/

  try {
    MainWindow mainWin;
    mainWin.show_all();
    Gtk::Main::run(mainWin);
  } catch (const Glib::FileError& ex) {
    std::cerr << "FileError: " << ex.what() << std::endl;
    return 1;
  } catch (const Gtk::BuilderError& ex) {
    std::cerr << "BuilderError: " << ex.what() << std::endl;
    return 1;
  }

  return 0;
}
