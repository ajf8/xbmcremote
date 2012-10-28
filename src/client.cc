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

#include <math.h>
#include <iostream>

#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <sigc++/sigc++.h>

#include <gdkmm.h>
#include <giomm.h>

#include <jsoncpp/writer.h>
#include <jsoncpp/value.h>
#include <jsoncpp/reader.h>

#include "requests.h"
#include "client.h"
#include "playlist-model.h"
#include "browse-movies-model.h"
#include "util.h"

using boost::asio::ip::tcp;
using XbmcRemote::RequestIdentifier;

namespace XbmcRemote {

class Worker {
public:

  Worker(boost::asio::io_service &io_service) :
      m_io_service(io_service) {
  }

  void operator()() {
    boost::asio::io_service::work work(m_io_service);
    m_io_service.run();
  }

private:
  boost::asio::io_service &m_io_service;
};

Client::Client() :
    m_io_service(), m_stopped(true), m_reader(), m_refSettings(Util::gsettings_create()),
    m_state(STATE_NOPLAYER), m_totalTime(0), m_elapsed(0), m_connected(false),
    m_playlistid(-1), m_playlist_model(new PlaylistModel()), m_movies_model(new BrowseMoviesModel()),
    m_socket(m_io_service), m_json_stream(), m_deadline(m_io_service), m_bracket_counter(0),
    m_heartbeat_timer(m_io_service) {
}

Client::~Client() {
  disconnect();
}

void Client::connect() {
	m_stopped = false;
	tcp::resolver r(m_io_service);
	Glib::ustring portString = Glib::ustring::format(m_refSettings->get_int("rpc-port"));
	connect(r.resolve(tcp::resolver::query(m_refSettings->get_string("hostname"), portString)));
}

void Client::start() {
  connect();
  m_deadline.async_wait(boost::bind(&Client::check_deadline, this));

  Worker worker(m_io_service);
  boost::thread workerThread(worker);
}

void Client::disconnect() {
  m_stopped = true;
  m_state = STATE_NOPLAYER;
  m_connected = false;
  m_playlistid = -1;
  m_autoSeekConnection.disconnect();

  boost::system::error_code ignored_ec;
  m_socket.close(ignored_ec);
  m_deadline.cancel();
  m_heartbeat_timer.cancel();

  Glib::signal_idle().connect_once(
      sigc::mem_fun(*this, &Client::disconnect_in_main_loop));
}

void Client::fire_signal_conn_error(const std::string& msg) {
  m_signal_conn_error(*this, msg);
}

void Client::fire_signal_conn_error_idle(const std::string& msg) {
  Glib::signal_idle().connect_once(
      sigc::bind(sigc::mem_fun(*this, &Client::fire_signal_conn_error), msg));
}

void Client::disconnect_in_main_loop() {
  m_playlist_model->clear();
  m_signal_disconnect(*this);
}

void Client::start_ping() {
  if (!m_stopped)
    write(Requests::ping());
}

Glib::RefPtr<BrowseMoviesModel> Client::get_movies_model()
{
  return m_movies_model;
}

void Client::connect(tcp::resolver::iterator endpoint_iter) {
  if (endpoint_iter != tcp::resolver::iterator()) {
    std::cout << "Trying " << endpoint_iter->endpoint() << "...\n";
    m_deadline.expires_from_now(boost::posix_time::seconds(CONNECT_TIMEOUT));
    m_socket.async_connect(endpoint_iter->endpoint(),
        boost::bind(&Client::handle_connect, this, _1, endpoint_iter));
  } else {
    disconnect();
  }
}

void Client::handle_connect(const boost::system::error_code& ec,
    tcp::resolver::iterator endpoint_iter) {
  if (m_stopped)
    return;

  if (!m_socket.is_open()) {
    std::cerr << "Connect timed out\n";
    connect(++endpoint_iter);
    //fire_signal_conn_error_idle("Timeout");
  } else if (ec) {
    std::cerr << "Connect error: " << ec.message() << "\n";
    m_socket.close();
    //fire_signal_conn_error_idle(ec.message());
    connect(++endpoint_iter);
  } else {
    std::cout << "Connected to " << endpoint_iter->endpoint() << "\n";
    start_read();
    write(Requests::get_active_players());
    //start_read();
    write(Requests::get_sources("video"));
    start_ping();
  }
}

void Client::start_read() {
  m_deadline.expires_from_now(boost::posix_time::seconds(PING_TIMEOUT));

  boost::asio::async_read_until(m_socket, m_input_buffer, '}',
      boost::bind(&Client::handle_read, this, _1));
}

void Client::handle_on_pause(JsonPtr root_ptr)
{
  m_state = STATE_PAUSE;
  m_autoSeekConnection.disconnect();
  m_signal_pause(*this, root_ptr);
}

void Client::handle_on_stop(JsonPtr root_ptr)
{
  m_state = STATE_STOP;
  m_autoSeekConnection.disconnect();
  m_signal_stop(*this, root_ptr);
}

void Client::handle_on_play(JsonPtr root_ptr)
{
  m_state = STATE_PLAY;
  write(Requests::get_items(ID_ONPLAY_GET_ITEMS));
}

void Client::handle_json_idle(JsonPtr root_ptr) {
  const Json::Value &root = *root_ptr;

  if (root.isMember("error")) {
    m_signal_remote_error(*this, root_ptr);
  } else if (root.isMember("id")) {
    /* Response has an ID, so it was sent by us and use that ID to figure
     * out what the response type is. */
    switch (root["id"].asInt()) {
    case ID_PING:
      handle_ping();
      break;
    case ID_GET_ACTIVE_PLAYERS:
      handle_get_active_players(root_ptr);
      break;
    case ID_CONNECT_GET_ITEMS:
      handle_connect_get_items(root_ptr);
      break;
    case ID_ONPLAY_GET_PROPERTIES:
      handle_get_properties(root_ptr);
      m_signal_play(*this, m_playlistid);
      break;
    case ID_ONPLAY_GET_ITEMS:
      handle_get_items(root_ptr);
      write(Requests::get_properties(ID_ONPLAY_GET_PROPERTIES));
      break;
    case ID_CONNECT_GET_PROPERTIES:
      handle_get_properties(root_ptr);
      m_connected = true;
      m_signal_connect(*this);
      break;
    case ID_GET_SOURCES:
      m_movies_model->update(root_ptr);
    	break;
    }
  } else if (root.isMember("method")) {
    const Json::Value &method = root["method"];
    if (method == "Player.OnPlay") {
      handle_on_play(root_ptr);
    } else if (method == "Player.OnPause") {
      handle_on_pause(root_ptr);
    } else if (method == "Player.OnStop") {
      handle_on_stop(root_ptr);
    }
  }
}

void Client::handle_json(JsonPtr root_ptr) {
  const Json::Value &root = *root_ptr;
  std::cout << root.toStyledString();

  /* Queue handle_json_idle into the GTK main loop, check response there. */

  Glib::signal_idle().connect_once(
      sigc::bind(sigc::mem_fun(*this, &Client::handle_json_idle), root_ptr));
}

void Client::handle_ping() {
  m_heartbeat_timer.expires_from_now(boost::posix_time::seconds(PING_INTERVAL));
  m_heartbeat_timer.async_wait(boost::bind(&Client::start_ping, this));
}

long Client::get_elapsed()
{
  return m_elapsed;
}

long Client::get_total_time()
{
  return m_totalTime;
}

bool Client::auto_seek_timeout_handler()
{
  if (m_state != STATE_PLAY)
    return false;

  m_elapsed += 1;

  m_signal_seek(*this, m_elapsed, m_totalTime);

  return true;
}

void Client::handle_get_properties(JsonPtr root_ptr) {
  const Json::Value &result = (*root_ptr)["result"];

  if (result.isMember("playlistid")) {
    if (result["speed"].asDouble() > 0) {
      m_state = STATE_PLAY;
      m_autoSeekConnection.disconnect();
      m_autoSeekConnection = Glib::signal_timeout().connect(sigc::mem_fun(*this,
          &Client::auto_seek_timeout_handler), 1000, Glib::PRIORITY_HIGH);
    } else {
      m_state = STATE_PAUSE;
    }

    m_playlistid = result["playlistid"].asInt();
  }

  m_elapsed = Util::json_time_to_seconds(result["time"]);
  m_totalTime = Util::json_time_to_seconds(result["totaltime"]);
}

void Client::handle_get_items(JsonPtr root_ptr)
{
  const Json::Value &result = (*root_ptr)["result"];

  if (result.isMember("items")) {
    const Json::Value &items = result["items"];
    m_playlist_model->populate(items);
  }
}

void Client::handle_connect_get_items(JsonPtr root_ptr) {
  handle_get_items(root_ptr);

  if (m_state != STATE_NOPLAYER) {
    write(Requests::get_properties(ID_CONNECT_GET_PROPERTIES));
  } else {
    m_connected = true;
    m_signal_connect(*this);
  }
}

int Client::get_playlistid() {
  return m_playlistid;
}

XbmcState Client::get_state() {
  return m_state;
}

bool Client::is_player_active() {
  return m_state != STATE_NOPLAYER;
}

bool Client::is_connected() {
  return m_connected;
}

bool Client::is_item_active() {
  return m_playlistid >= 0;
}

Glib::RefPtr<PlaylistModel>& Client::playlist_model() {
  return m_playlist_model;
}

Gtk::TreeIter Client::get_active_iter() {
  return m_playlist_model->children()[m_playlistid - 1];
}

void Client::handle_get_active_players(JsonPtr root_ptr) {
  const Json::Value &result = (*root_ptr)["result"];

  if (result.size() > 0)
    m_state = STATE_STOP;

  write(Requests::get_items(ID_CONNECT_GET_ITEMS));
}

void Client::handle_read(const boost::system::error_code& ec) {
  if (m_stopped)
    return;

  /* It would be easier if XBMC sent a newline at the end of responses.
   * Read up to every closing bracket as an async read, buffering into a
   * stringbuffer until the open bracket counter decrements to zero.
   */

  if (!ec) {
    char ic[512];
    std::istream is(&m_input_buffer);

    int len = is.readsome(ic, sizeof(ic));
    int i;

    for (i = 0; i < len; i++) {
      m_json_stream << ic[i];

      if (ic[i] == '{') {
        m_bracket_counter++;
      } else if (ic[i] == '}' && --m_bracket_counter == 0) {
        JsonPtr root(new Json::Value());

        if (m_reader.parse(m_json_stream, *root, false))
          handle_json(root);

        m_json_stream.clear();
      }
    }
    start_read();
  } else {
    std::cout << "Error on receive: " << ec.message() << "\n";
    disconnect();
    fire_signal_conn_error_idle(ec.message());
  }
}

void Client::write(const Json::Value& value) {
  if (m_stopped)
    return;

  Json::FastWriter writer;
  std::string json = writer.write(value);

  std::cout << "writing: " << json << std::endl;

  boost::asio::async_write(m_socket, boost::asio::buffer(json, json.length()),
      boost::bind(&Client::handle_write, this, _1));
}

void Client::handle_write(const boost::system::error_code& ec) {
  if (m_stopped)
    return;

  if (ec) {
    std::cout << "Error on heartbeat: " << ec.message() << "\n";
    disconnect();
  }
}

void Client::check_deadline() {
  if (m_stopped)
    return;

  if (m_deadline.expires_at() <= deadline_timer::traits_type::now()) {
    m_socket.close();
    m_deadline.expires_at(boost::posix_time::pos_infin);
  }

  m_deadline.async_wait(boost::bind(&Client::check_deadline, this));
}

Client::PlaySignal Client::signal_play() {
  return m_signal_play;
}

Client::JsonResponseSignal Client::signal_stop() {
  return m_signal_stop;
}

Client::JsonResponseSignal Client::signal_pause() {
  return m_signal_pause;
}

Client::SeekSignal Client::signal_seek() {
  return m_signal_seek;
}

Client::JsonResponseSignal Client::signal_speed_change() {
  return m_signal_speed_change;
}

Client::JsonResponseSignal Client::signal_remote_error() {
  return m_signal_remote_error;
}

Client::ConnectionErrorSignal Client::signal_conn_error() {
  return m_signal_conn_error;
}

Client::ConnectionChangeSignal Client::signal_connect() {
  return m_signal_connect;
}

Client::ConnectionChangeSignal Client::signal_disconnect() {
  return m_signal_disconnect;
}

}
