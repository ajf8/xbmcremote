AC_PREREQ(2.63)

AC_INIT(xbmcremote, 0.1, alan@eth0.org.uk)
AC_CONFIG_SRCDIR(src)
AC_CONFIG_AUX_DIR([.])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)

AC_PROG_CXX

AC_PROG_INSTALL
AC_PROG_LIBTOOL

PKG_CHECK_MODULES([gtkmm], [gtkmm-3.0])
PKG_CHECK_MODULES([giomm], [giomm-2.4 >= 2.26])
PKG_CHECK_MODULES([gdlmm], [gdlmm-3.0])
dnl PKG_CHECK_MODULES([avahiclient], [avahi-client >= 0.6])
PKG_CHECK_MODULES([curlpp], [curlpp >= 0.7])

BOOST_REQUIRE()
BOOST_ASIO()
BOOST_SYSTEM()
BOOST_THREADS()

GLIB_GSETTINGS

AC_ARG_ENABLE(debug, [AS_HELP_STRING(--enable-debug,
                                     [enable debugging])])
if test x$enable_debug = xyes; then
    AC_DEFINE([DEBUG], [],  [enable debugging])
fi

AC_OUTPUT([Makefile
	   src/Makefile
	   ui/Makefile
	   third-party/Makefile
	   third-party/jsoncpp/Makefile
	   third-party/jsoncpp/src/Makefile])
