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

#ifndef CLIENT_H_
#define CLIENT_H_

#include <sstream>
#include <string>
#include <exception>

#include <gdkmm.h>
#include <giomm.h>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>
#include <jsoncpp/value.h>
#include <jsoncpp/reader.h>

#include "playlist-model.h"
#include "browse-movies-model.h"

#define DEFAULT_PLAYER_ID     1
#define DEFAULT_PLAYLIST_ID   1
#define CONNECT_TIMEOUT       10
#define PING_INTERVAL         25
#define PING_TIMEOUT          30

using boost::asio::deadline_timer;
using boost::asio::ip::tcp;

namespace XbmcRemote {

enum XbmcState
{
  STATE_PLAY,
  STATE_PAUSE,
  STATE_STOP,
  STATE_NOPLAYER
};

class Client {
public:
  Client();
  ~Client();

  /* methods - control. */
  void start();
  void connect();
  void disconnect();
  void write(const Json::Value& value);
  Glib::RefPtr<Gdk::Pixbuf> get_pixbuf(const Glib::ustring &vfslocation,
      double scale);

  /* methods - remote state. */
  int get_playlistid();
  XbmcState get_state();
  bool is_player_active();
  bool is_connected();
  Gtk::TreeIter get_active_iter();
  bool is_item_active();
  Glib::RefPtr<PlaylistModel>& playlist_model();
  long get_elapsed();
  long get_total_time();

  /* signal types and accessors */
  typedef boost::shared_ptr<Json::Value> JsonPtr;
  typedef sigc::signal<void, Client&, JsonPtr> JsonResponseSignal;
  typedef sigc::signal<void, Client&> ConnectionChangeSignal;
  typedef sigc::signal<void, Client&, const std::string &> ConnectionErrorSignal;
  typedef sigc::signal<void, Client&, uint> PlaySignal;
  typedef sigc::signal<void, Client&, long, long> SeekSignal;

  PlaySignal signal_play();
  JsonResponseSignal signal_stop();
  JsonResponseSignal signal_pause();
  SeekSignal signal_seek();
  JsonResponseSignal signal_speed_change();
  JsonResponseSignal signal_remote_error();
  ConnectionErrorSignal signal_conn_error();
  ConnectionChangeSignal signal_connect();
  ConnectionChangeSignal signal_disconnect();
protected:
  /* fields */
  boost::asio::io_service m_io_service;
  bool m_stopped;
  Json::Reader m_reader;

  Glib::RefPtr<Gio::Settings> m_refSettings;

  /* fields - remote state. */
  XbmcState m_state;
  long m_totalTime;
  long m_elapsed;
  bool m_connected;
  int m_playlistid;
  Glib::RefPtr<PlaylistModel> m_playlist_model;
  sigc::connection m_autoSeekConnection;
  Glib::RefPtr<BrowseMoviesModel> m_movies_model;

  tcp::socket m_socket;
  boost::asio::streambuf m_input_buffer;
  std::stringstream m_json_stream;
  deadline_timer m_deadline;
  int m_bracket_counter;
  deadline_timer m_heartbeat_timer;

  /* methods - I/O framework. */
  void connect(tcp::resolver::iterator endpoint_iter);
  void handle_connect(const boost::system::error_code& ec,
      tcp::resolver::iterator endpoint_iter);
  void start_read();
  void handle_read(const boost::system::error_code& ec);
  void start_write();
  void handle_write(const boost::system::error_code& ec);
  void handle_json_idle(JsonPtr root_ptr);
  void handle_json(JsonPtr root_ptr);
  void start_ping();
  void check_deadline();
  void io_thread(boost::asio::io_service &io_service);

  /* signal triggers */
  void disconnect_in_main_loop();
  void fire_signal_conn_error(const std::string& msg);
  void fire_signal_conn_error_idle(const std::string& msg);

  /* methods - response handlers. */
  void handle_on_play(JsonPtr root_ptr);
  void handle_on_pause(JsonPtr root_ptr);
  void handle_on_stop(JsonPtr root_ptr);
  void handle_get_active_players(JsonPtr root_ptr);
  void handle_get_items(JsonPtr root_ptr);
  void handle_connect_get_items(JsonPtr root_ptr);
  bool auto_seek_timeout_handler();
  void handle_get_properties(JsonPtr root_ptr);
  void handle_ping();

  /* signals */
  PlaySignal m_signal_play;
  JsonResponseSignal m_signal_pause;
  JsonResponseSignal m_signal_stop;
  SeekSignal m_signal_seek;
  JsonResponseSignal m_signal_speed_change;
  JsonResponseSignal m_signal_remote_error;
  ConnectionChangeSignal m_signal_connect;
  ConnectionChangeSignal m_signal_disconnect;
  ConnectionErrorSignal m_signal_conn_error;
};

}

#endif /* CLIENT_H_ */
