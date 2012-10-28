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

#include <jsoncpp/value.h>

#include "requests.h"

namespace XbmcRemote {

Json::Value Requests::get_active_players()
{
  Json::Value value;
  value["jsonrpc"] = "2.0";
  value["method"] = "Player.GetActivePlayers";
  value["id"] = ID_GET_ACTIVE_PLAYERS;
  return value;
}

Json::Value Requests::ping()
{
  Json::Value value;

  value["jsonrpc"] = "2.0";
  value["method"] = "JSONRPC.Ping";
  value["id"] = ID_PING;

  return value;
}

Json::Value Requests::play_pause(uint playerid)
{
  return generic_player("Player.PlayPause", ID_PLAY_PAUSE, playerid);
}

Json::Value Requests::playlist_clear(uint playlistid)
{
  Json::Value value;
  Json::Value params;

  params["playlistid"] = playlistid;

  value["jsonrpc"] = "2.0";
  value["method"] = "Playlist.Clear";
  value["params"] = params;
  value["id"] = ID_PLAYLIST_CLEAR;

  return value;
}

Json::Value Requests::generic(const std::string &method, RequestIdentifier id)
{
  Json::Value value;
  Json::Value params;

  value["jsonrpc"] = "2.0";
  value["method"] = method;
  value["id"] = id;

  return value;
}

Json::Value Requests::generic_player(const std::string &method, RequestIdentifier id, uint playerid)
{
  Json::Value value;
  Json::Value params;

  params["playerid"] = playerid;

  value["jsonrpc"] = "2.0";
  value["method"] = method;
  value["params"] = params;
  value["id"] = id;

  return value;
}

Json::Value Requests::stop(uint playerid)
{
  return generic_player("Player.Stop", ID_STOP, playerid);
}

Json::Value Requests::get_items(RequestIdentifier id, uint playlistid)
{
  Json::Value value;
  Json::Value params;
  Json::Value properties;

  value["jsonrpc"] = "2.0";
  value["method"] = "Playlist.GetItems";

  properties.append("title");
  properties.append("season");
  properties.append("episode");
  properties.append("plot");
  properties.append("runtime");
  properties.append("showtitle");
  properties.append("thumbnail");

  params["properties"] = properties;
  params["playlistid"] = playlistid;

  value["id"] = id;
  value["params"] = params;

  return value;
}

/*
{"jsonrpc": "2.0", "method": "Player.GetProperties", "params": {
"playerid": 1, "properties": [ "playlistid", "speed", "position", "totaltime",
"time" ] }, "id": 1} */

Json::Value Requests::get_properties(RequestIdentifier id, uint playerid)
{
  Json::Value value;
  Json::Value params;
  Json::Value properties;

  value["jsonrpc"] = "2.0";
  value["method"] = "Player.GetProperties";

  properties.append("playlistid");
  properties.append("speed");
  properties.append("position");
  properties.append("totaltime");
  properties.append("time");

  params["properties"] = properties;
  params["playerid"] = playerid;

  value["id"] = id;
  value["params"] = params;

  return value;
}

Json::Value Requests::get_sources(const std::string &media)
{
	  Json::Value value;
	  Json::Value params;

	  value["jsonrpc"] = "2.0";
	  value["method"] = "Files.GetSources";
	  value["id"] = ID_GET_SOURCES;

	  params["media"] = media;
	  value["params"] = params;

	  return value;
}

}
