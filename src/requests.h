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

#include <boost/shared_ptr.hpp>
#include <jsoncpp/value.h>

#include "client.h"

#ifndef REQUESTS_H_
#define REQUESTS_H_

namespace XbmcRemote {

enum RequestIdentifier
{
  ID_GET_ACTIVE_PLAYERS,
  ID_ONPLAY_GET_PROPERTIES,
  ID_CONNECT_GET_PROPERTIES,
  ID_ONPLAY_GET_ITEMS,
  ID_CONNECT_GET_ITEMS,
  ID_PING,
  ID_PLAY_PAUSE,
  ID_STOP,
  ID_PLAYLIST_CLEAR,
  ID_DO_NOTHING
};

class Requests
{
public:
  static Json::Value ping();
  static Json::Value get_active_players();
  static Json::Value get_items(RequestIdentifier id, uint playlistid = DEFAULT_PLAYER_ID);
  static Json::Value get_properties(RequestIdentifier id, uint playerid = DEFAULT_PLAYER_ID);
  static Json::Value play_pause(uint playerid = DEFAULT_PLAYER_ID);
  static Json::Value stop(uint playerid = DEFAULT_PLAYER_ID);
  static Json::Value playlist_clear(uint playlistid = DEFAULT_PLAYLIST_ID);
  static Json::Value generic(const std::string &method, RequestIdentifier id = ID_DO_NOTHING);
  static Json::Value generic_player(const std::string &method, RequestIdentifier id = ID_DO_NOTHING, uint playerid = DEFAULT_PLAYER_ID);
};

}

#endif /* REQUESTS_H_ */
