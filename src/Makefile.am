INCLUDES=-I../third-party/jsoncpp/include

xbmcremote_defines = \
	       -DXBMCR_DATADIR=\""$(datadir)"\" \
	       -DXBMCR_LOCALEDIR=\""$(localedir)"\" \
	       -DXBMCR_PKGDATADIR=\""$(pkgdatadir)"\"

bin_PROGRAMS = xbmcremote

xbmcremote_CPPFLAGS = $(BOOST_CPPFLAGS) --pedantic -Wall $(gdlmm_CFLAGS) $(gtkmm_CFLAGS) $(giomm_CFLAGS) $(avahiclient_CFLAGS) $(curlpp_CFLAGS) $(xbmcremote_defines)
xbmcremote_LDFLAGS = $(BOOST_THREAD_LIBS) $(BOOST_ASIO_LIBS) $(BOOST_SYSTEM_LIBS) $(gtkmm_LIBS) $(giomm_LIBS) $(avahiclient_LIBS) $(gdlmm_LIBS) $(curlpp_LIBS) $(BOOST_ASIO_LDFLAGS) $(BOOST_SYSTEM_LDFLAGS) $(BOOST_THREAD_LDFLAGS)
xbmcremote_SOURCES = main-window.cc \
					 main.cc \
					 client.cc \
					 requests.cc \
					 playlist-model.cc \
					 util.cc \
					 browse-cell-renderer.cc \
					 browse-movies-cell-renderer.cc \
					 browse-model.cc \
					 browse-movies-model.cc \
					 preferences-dialog.cc \
					 xvfs-cache.cc \
					 xvfs-cache-object.cc
					 
xbmcremote_LDADD = ../third-party/jsoncpp/src/libjsoncpp.la

gsettings_SCHEMAS = uk.org.eth0.xbmcremote.gschema.xml
@GSETTINGS_RULES@
