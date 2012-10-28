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
#include <gtkmm/stock.h>
#include <gdlmm.h>
#include <jsoncpp/value.h>
#include <jsoncpp/writer.h>

#include "client.h"
#include "requests.h"
#include "util.h"
#include "browse-model.h"
#include "browse-movies-model.h"
#include "browse-cell-renderer.h"
#include "browse-movies-cell-renderer.h"
#include "preferences-dialog.h"
#include "xvfs-cache.h"
#include "main-window.h"

namespace XbmcRemote {

bool MainWindow::on_delete_event(GdkEventAny* event) {
  //m_layout_manager->save_layout("xbmcremote");
  return false;
}

MainWindow::MainWindow() :
    m_xvfs(),
    m_client(), m_layout_manager(Gdl::DockLayout::create(m_dock)),
    m_imageDockItem("image", "Thumbnail", Gtk::Stock::FIND, Gdl::DOCK_ITEM_BEH_NORMAL),
    m_playerDockItem("player", "Player", Gdl::DOCK_ITEM_BEH_CANT_CLOSE | Gdl::DOCK_ITEM_BEH_NEVER_FLOATING | Gdl::DOCK_ITEM_BEH_LOCKED | Gdl::DOCK_ITEM_BEH_NO_GRIP),
    m_playlistDockItem("playlist", "Playlist", Gtk::Stock::INDEX, Gdl::DOCK_ITEM_BEH_NORMAL),
    m_remoteDockItem("remote", "Remote", Gtk::Stock::NETWORK, Gdl::DOCK_ITEM_BEH_NORMAL),
    m_browserDockItem("browser", "Browser", Gtk::Stock::FILE, Gdl::DOCK_ITEM_BEH_NORMAL),
    m_ph1("ph1", m_dock, Gdl::DOCK_TOP, false),
    m_ph2("ph2", m_dock, Gdl::DOCK_BOTTOM, false),
    m_ph3("ph3", m_dock, Gdl::DOCK_LEFT, false),
    m_ph4("ph4", m_dock, Gdl::DOCK_RIGHT, false)
{
  signal_delete_event().connect(
      sigc::mem_fun(*this, &MainWindow::on_delete_event));
  set_default_icon_name(Gtk::Stock::NETWORK.id);

  m_layout_manager->load_from_file("xbmcremote-layout.xml");

  Gdl::DockBar* dockbar = new Gdl::DockBar(m_dock);
  dockbar->set_style(Gdl::DOCK_BAR_BOTH);

  Gtk::Box* box = new Gtk::HBox(false, 5);
  box->set_border_width(10);
  box->pack_start(*Gtk::manage(dockbar), false, false);
  box->pack_end(m_dock);

  create_items();

  set_title("xbmcremote");
  set_default_size(550, 250);
  add(*Gtk::manage(box));
  show_all_children();

  m_client.start();
}

void MainWindow::create_items() {
  m_dock.add_item(m_playerDockItem, Gdl::DOCK_TOP);
  m_dock.add_item(m_imageDockItem, Gdl::DOCK_RIGHT);
  m_dock.add_item(m_remoteDockItem, Gdl::DOCK_BOTTOM);
  //m_dock.add_item(m_browserDockItem, Gdl::DOCK_BOTTOM);
  m_dock.add_item(m_playlistDockItem, Gdl::DOCK_BOTTOM);

  m_playlistDockItem.dock_to(m_playerDockItem, Gdl::DOCK_BOTTOM);
  m_imageDockItem.dock_to(m_playlistDockItem, Gdl::DOCK_RIGHT);
  m_remoteDockItem.dock_to(m_playerDockItem, Gdl::DOCK_RIGHT);

  m_playerDockItem.add(*create_player());
  m_playerDockItem.set_vexpand(false);
  m_remoteDockItem.add(*create_remote());
  m_remoteDockItem.set_vexpand(false);
  m_playlistDockItem.add(*create_playlist());
  m_playlistDockItem.set_vexpand(true);
  m_imageDockItem.add(*create_image());
  m_imageDockItem.set_vexpand((false));
  m_browserDockItem.add(*create_browser());
}

Gtk::Widget* MainWindow::create_image() {
  Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file(
      Util::build_glade_path("image.glade"));

  builder->get_widget("imageAlignment", m_imageAlignment);
  builder->get_widget("playlistImage", m_playlistImage);

  return m_imageAlignment;
}

void MainWindow::on_generic_player_request_click(const std::string &method) {
  m_client.write(Requests::generic_player(method));
}

void MainWindow::on_generic_request_click(const std::string &method) {
  m_client.write(Requests::generic(method));
}

void MainWindow::dispatch_request(Json::Value request)
{
  m_client.write(request);
}

Gtk::Widget* MainWindow::create_browser()
{
  Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file(
      Util::build_glade_path("browser.glade"));
  BrowseMoviesCellRenderer *bcr = Gtk::manage(new BrowseMoviesCellRenderer());
  Glib::RefPtr<BrowseMoviesModel> model = m_client.get_movies_model();
  BrowseMoviesModelColumns& columns = model->columns();

  builder->get_widget("browserBox", m_browserBox);
  builder->get_widget("browserTreeView", m_browserTreeView);

  /*Gtk::TreeModel::Row row = *(m_browserModelMovies->append());
  row[columns.m_col_thumbnail] = "special://masterprofile/Thumbnails/Video/5/5d807579.tbn";
  row[columns.m_col_label] = "Title";*/

  m_browserTreeView->set_model(model);

  m_browserTreeView->append_column("Item", *bcr);
  Gtk::TreeViewColumn *viewCol = m_browserTreeView->get_column(0);
  viewCol->add_attribute(bcr->property_thumbnail(), columns.m_col_thumbnail);
  viewCol->add_attribute(bcr->property_label(), columns.m_col_label);

  m_browserTreeView->signal_row_activated().connect(sigc::mem_fun(*this, &MainWindow::on_browse_click));

  return m_browserBox;
}

void MainWindow::on_browse_click(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
{

}

Gtk::Widget* MainWindow::create_remote() {
  Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file(
      Util::build_glade_path("remote.glade"));

  builder->get_widget("remoteGrid", m_remoteGrid);
  builder->get_widget("upButton", m_inputUp);
  builder->get_widget("downButton", m_inputDown);
  builder->get_widget("leftButton", m_inputLeft);
  builder->get_widget("rightButton", m_inputRight);
  builder->get_widget("selectButton", m_inputSelect);
  builder->get_widget("backButton", m_inputBack);
  builder->get_widget("homeButton", m_inputHome);

  m_inputBack->signal_clicked().connect(
      sigc::bind(sigc::mem_fun(*this, &MainWindow::on_generic_request_click),
          "Input.Back"));
  m_inputHome->signal_clicked().connect(
      sigc::bind(sigc::mem_fun(*this, &MainWindow::on_generic_request_click),
          "Input.Home"));
  m_inputUp->signal_clicked().connect(
      sigc::bind(sigc::mem_fun(*this, &MainWindow::on_generic_request_click),
          "Input.Up"));
  m_inputDown->signal_clicked().connect(
      sigc::bind(sigc::mem_fun(*this, &MainWindow::on_generic_request_click),
          "Input.Down"));
  m_inputLeft->signal_clicked().connect(
      sigc::bind(sigc::mem_fun(*this, &MainWindow::on_generic_request_click),
          "Input.Left"));
  m_inputRight->signal_clicked().connect(
      sigc::bind(sigc::mem_fun(*this, &MainWindow::on_generic_request_click),
          "Input.Right"));
  m_inputSelect->signal_clicked().connect(
      sigc::bind(sigc::mem_fun(*this, &MainWindow::on_generic_request_click),
          "Input.Select"));

  return m_remoteGrid;
}

void MainWindow::on_playlist_add_clicked() {
  m_browserDockItem.dock_to(m_playlistDockItem, Gdl::DOCK_TOP);
}

void MainWindow::on_settings_button_clicked() {
  PreferencesDialog pd(m_client);
  pd.run();
}

Gtk::Widget* MainWindow::create_playlist() {
  Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file(
      Util::build_glade_path("playlist.glade"));

  builder->get_widget("playlistBox", m_playlistBox);
  builder->get_widget("playlistTreeView", m_playlistTreeView);
  builder->get_widget("addButton", m_plAddButton);
  builder->get_widget("delButton", m_plDelButton);
  builder->get_widget("clearButton", m_plClearButton);

  m_playlistTreeView->set_model(m_client.playlist_model());
  m_playlistTreeView->append_column("label",
      m_client.playlist_model()->columns().m_col_label);

  m_plClearButton->signal_clicked().connect(
      sigc::bind(
          sigc::mem_fun(*this, &MainWindow::dispatch_request),
          Requests::playlist_clear()));
  m_plAddButton->signal_clicked().connect(
          sigc::mem_fun(*this, &MainWindow::on_playlist_add_clicked));


  return m_playlistBox;
}

Gtk::Widget* MainWindow::create_player() {
  Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file(
      Util::build_glade_path("player.glade"));

  builder->get_widget("progressScale", m_progressScale);
  builder->get_widget("progressLabel", m_progressLabel);
  builder->get_widget("pauseImage", m_pauseImage);
  builder->get_widget("playImage", m_playImage);
  builder->get_widget("playerBox", m_playerBox);
  builder->get_widget("backButton", m_playPreviousButton);
  builder->get_widget("forwardButton", m_playerNextButton);
  builder->get_widget("playPauseButton", m_playPauseButton);
  builder->get_widget("settingsButton", m_settingsButton);
  builder->get_widget("stopButton", m_stopButton);
  builder->get_widget("primaryLabel", m_primaryLabel);

  m_playPreviousButton->signal_clicked().connect(
      sigc::bind(
          sigc::mem_fun(*this, &MainWindow::on_generic_player_request_click),
          "Player.GoPrevious"));
  m_playerNextButton->signal_clicked().connect(
      sigc::bind(
          sigc::mem_fun(*this, &MainWindow::on_generic_player_request_click),
          "Player.GoNext"));
  m_playPauseButton->signal_clicked().connect(
      sigc::bind(sigc::mem_fun(*this, &MainWindow::dispatch_request), Requests::play_pause()));
  m_stopButton->signal_clicked().connect(
      sigc::bind(sigc::mem_fun(*this, &MainWindow::dispatch_request), Requests::stop()));

  m_settingsButton->signal_clicked().connect(
      sigc::mem_fun(*this, &MainWindow::on_settings_button_clicked));

  m_client.signal_play().connect(
      sigc::mem_fun(*this, &MainWindow::on_client_play));
  m_client.signal_pause().connect(
      sigc::mem_fun(*this, &MainWindow::on_client_pause));
  m_client.signal_stop().connect(
      sigc::mem_fun(*this, &MainWindow::on_client_stop));
  m_client.signal_seek().connect(
      sigc::mem_fun(*this, &MainWindow::on_client_seek));
  m_client.signal_speed_change().connect(
      sigc::mem_fun(*this, &MainWindow::on_client_speed_change));
  m_client.signal_remote_error().connect(
      sigc::mem_fun(*this, &MainWindow::on_client_remote_error));
  m_client.signal_conn_error().connect(
      sigc::mem_fun(*this, &MainWindow::on_client_conn_error));
  m_client.signal_connect().connect(
      sigc::mem_fun(*this, &MainWindow::on_client_connect));
  m_client.signal_disconnect().connect(
      sigc::mem_fun(*this, &MainWindow::on_client_disconnect));

  return m_playerBox;
}

MainWindow::~MainWindow() {
}

void MainWindow::on_client_play(Client &client, uint playlistid) {
  refresh_player(client);
}

void MainWindow::on_client_stop(Client &client, Client::JsonPtr json_ptr) {
  m_primaryLabel->set_text("Stopped.");
  set_playpause_dependent_widgets(client.get_state());
  m_playlistImage->clear();
}

void MainWindow::on_client_pause(Client &client, Client::JsonPtr json_ptr) {
  m_playPauseButton->set_image(*m_playImage);
  set_playpause_dependent_widgets(client.get_state());
}

void MainWindow::on_client_seek(Client &client, long elapsed, long total) {
  m_progressScale->set_range(0, total);
  m_progressScale->set_value(elapsed);
  m_progressLabel->set_text(Glib::ustring::format(
      Util::seconds_to_short_time(elapsed), " / ",
      Util::seconds_to_short_time(total)));
}

void MainWindow::on_client_speed_change(Client &client,
    Client::JsonPtr json_ptr) {
}

void MainWindow::on_client_remote_error(Client &client,
    Client::JsonPtr json_ptr) {
  const Json::Value &root = *json_ptr;
  const Json::Value &error = root["error"];
  Gtk::MessageDialog box(error["message"].asString(), false,
      Gtk::MESSAGE_ERROR);
  box.run();
}

void MainWindow::on_client_conn_error(Client &client, const std::string &msg) {
  Gtk::MessageDialog box(msg, false, Gtk::MESSAGE_ERROR);
  box.run();
}

void MainWindow::cache_object_state_change(XVFSCacheObject &obj)
{
  if (obj.get_state() == CS_CACHED)
    m_playlistImage->set(obj.scale(160));
}

void MainWindow::refresh_player(Client &client) {
  XbmcState state = client.get_state();
  set_playpause_dependent_widgets(state);

  if (client.is_item_active()) {
    Gtk::TreeIter iter = m_client.get_active_iter();
    PlaylistModelColumns &columns = m_client.playlist_model()->columns();
    Glib::ustring label = (*iter)[columns.m_col_label];

    try {
      boost::shared_ptr<XVFSCacheObject> cacheObject = m_xvfs.get((*iter)[columns.m_col_thumbnail]);
      cacheObject->signal_state_change().connect(sigc::mem_fun(*this, &MainWindow::cache_object_state_change));
      m_xvfs.fetch_async(cacheObject);
    } catch (const Gdk::PixbufError &e) {
      std::cerr << "unable to get pixbuf" << std::endl;
    } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
    }

    if (label.length() > 43) {
      label = label.substr(0, 40);
      label.append(" ..");
    }

    m_primaryLabel->set_text(label);
    m_progressLabel->set_text(Glib::ustring::format(
        Util::seconds_to_short_time(m_client.get_elapsed()), " / ",
        Util::seconds_to_short_time(m_client.get_total_time())));
    m_progressScale->set_range(0, m_client.get_total_time());
    m_progressScale->set_value(m_client.get_elapsed());
  } else {
    m_primaryLabel->set_text("Connected. Nothing playing.");
  }
}

void MainWindow::set_playpause_dependent_widgets(XbmcState state)
{
  bool playPause = (state == STATE_PLAY || state == STATE_PAUSE);
  m_playPauseButton->set_sensitive(playPause);
  m_playerNextButton->set_sensitive(playPause);
  m_playPreviousButton->set_sensitive(playPause);
  m_stopButton->set_sensitive(playPause);
  m_progressScale->set_sensitive(playPause);

  if (!playPause) {
    m_progressLabel->set_text("");
    m_playlistImage->clear();
  }

  m_playPauseButton->set_image(
      state == STATE_PLAY ? *m_pauseImage : *m_playImage);
}

void MainWindow::set_connection_dependent_widgets(bool connected)
{
  m_remoteGrid->set_sensitive(connected);
  m_playlistBox->set_sensitive(connected);
}

void MainWindow::on_client_connect(Client &client) {
  set_connection_dependent_widgets(true);
  refresh_player(client);
  //m_imageDockItem.dock_to(m_playerDockItem, Gdl::DOCK_RIGHT);
}

void MainWindow::on_client_disconnect(Client &client)
{
  m_primaryLabel->set_label("Disconnected.");
  set_connection_dependent_widgets(false);
  set_playpause_dependent_widgets(client.get_state());
}

}
