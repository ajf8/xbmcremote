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

#ifndef XBMCREMOTE_HPP_
#define XBMCREMOTE_HPP_

#include <gdlmm.h>
#include <gtkmm.h>
#include <boost/system/error_code.hpp>

#include "client.h"
#include "xvfs-cache.h"
#include "browse-model.h"

namespace XbmcRemote
{

class MainWindow : public Gtk::Window
{

public:
  MainWindow();
  ~MainWindow();
protected:
  /* signal handlers */
  void on_playlist_add_clicked();
  void on_settings_button_clicked();
  void on_generic_request_click(const std::string &method);
  void on_generic_player_request_click(const std::string &method);
  void dispatch_request(Json::Value request);
  void on_breadcrumb_added(BrowseModel &model, BrowseBreadcrumb &crumb);
  void on_breadcrumb_pop(BrowseModel &model);
  void on_client_play(Client &client, uint playlistid);
  void on_client_stop(Client &client, Client::JsonPtr json_ptr);
  void on_client_pause(Client &client, Client::JsonPtr json_ptr);
  void on_client_seek(Client &client, long elapsed, long total);
  void on_client_speed_change(Client &client, Client::JsonPtr json_ptr);
  void on_client_remote_error(Client &client, Client::JsonPtr json_ptr);
  void on_client_conn_error(Client &client, const std::string &msg);
  void on_client_disconnect(Client &client);
  void on_client_connect(Client &client);
  bool on_delete_event(GdkEventAny* event);
  void cache_object_state_change(XVFSCacheObject &obj);
  void refresh_player(Client &client);

  /* methods - UI setup. */
  Gtk::Widget* create_styles_item();
  Gtk::RadioButton* create_style_button(Gtk::Box* box,
                                        Gtk::RadioButtonGroup* group,
                                        Gdl::SwitcherStyle style,
                                        const Glib::ustring& style_text);
  void create_items();
  void on_change_name(Gdl::DockItem& item);
  Gtk::Widget* create_playlist();
  Gtk::Widget* create_image();
  Gtk::TreeViewColumn& append_and_get_column(Gtk::TreeView &tv, const Glib::ustring &title, Gtk::CellRenderer &cr);
  void on_browse_click(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
  Gtk::Widget* create_remote();
  Gtk::Widget* create_browser();
  Gtk::Widget* create_player();
  void on_style_button_toggled(Gtk::RadioButton* button,
                               Gdl::SwitcherStyle style);
  void set_playpause_dependent_widgets(XbmcState state);
  void set_connection_dependent_widgets(bool connected);

  /* UI fields - generic + gdl */
  XVFSCache m_xvfs;
  Client m_client;
  Gdl::Dock m_dock;
  Glib::RefPtr<Gdl::DockLayout> m_layout_manager;
  Gdl::DockItem m_imageDockItem;
  Gdl::DockItem m_playerDockItem;
  Gdl::DockItem m_playlistDockItem;
  Gdl::DockItem m_remoteDockItem;
  Gdl::DockItem m_browserDockItem;
  Gdl::DockPlaceholder m_ph1;
  Gdl::DockPlaceholder m_ph2;
  Gdl::DockPlaceholder m_ph3;
  Gdl::DockPlaceholder m_ph4;

  /* UI fields - player */
  Gtk::Box *m_playerBox;
  Gtk::Box *m_breadcrumbsBox;
  Gtk::Window *m_mainWindow;
  Gtk::Button *m_playPauseButton;
  Gtk::Button *m_settingsButton;
  Gtk::Button *m_stopButton;
  Gtk::Button *m_playPreviousButton;
  Gtk::Button *m_playerNextButton;
  Gtk::Image *m_playImage;
  Gtk::Image *m_pauseImage;
  Gtk::Scale *m_progressScale;
  Gtk::Label *m_progressLabel;
  Gtk::Label *m_primaryLabel;

  /* UI fields - playlist. */
  Gtk::Box *m_playlistBox;
  Gtk::TreeView *m_playlistTreeView;
  Gtk::Button *m_plDelButton;
  Gtk::Button *m_plClearButton;
  Gtk::Button *m_plAddButton;

  /* UI fields - image. */
  Gtk::Image *m_playlistImage;
  Gtk::Alignment *m_imageAlignment;

  /* UI fields - remote. */
  Gtk::Grid *m_remoteGrid;
  Gtk::Button *m_inputLeft;
  Gtk::Button *m_inputRight;
  Gtk::Button *m_inputUp;
  Gtk::Button *m_inputDown;
  Gtk::Button *m_inputSelect;
  Gtk::Button *m_inputHome;
  Gtk::Button *m_inputBack;

  /* UI fields - browser. */
  Gtk::Box *m_browserBox;
  Gtk::TreeView *m_browserTreeView;
  Glib::RefPtr<BrowseMoviesModel> m_browserModelMovies;
};

}

#endif /* XBMCREMOTE_HPP_ */
