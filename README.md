# Synnefo (gr. cloud)

Synnefo is a Qt color management front-end.  It uses the Oyranos CMS to provide the ability to set and examine device profiles, as well as to change system-wide color settings.

###Links
* [About](http://www.oyranos.org/synnefo)
* [Support](http://www.oyranos.org/support)
* [Copyright](COPYING.md) - BSD
* [Authors](AUTHORS.md)


###Dependencies

This build is currently using the latest Oyranos build through "git". It is required that Oyranos be up-to-date using this.

* [Oyranos](http://www.oyranos.org) - for the Color Management System
* [Qt](http://www.qt.io) - for a nice observer utility Qt4 or Qt5


###Building

Type the following in Synnefo's root directory:

    $ mkdir build
    $ cd build

    $ cmake ..
    $ make
    $ make install

If you wish to uninstall Synnefo, simply type:

    $ make uninstall

####Build Flags
... are typical cmake flags like CMAKE_C_FLAGS to tune compilation.

* CMAKE_INSTALL_PREFIX to install into paths and so on. Use on the command 
  line through -DCMAKE_INSTALL_PREFIX=/my/path .
* LIB_SUFFIX - allows to append a architecture specific suffix like 
  LIB_SUFFIX=64 for 64bit non debian style Linux systems.
* CMAKE_BUILD_TYPE - debug or release or what you like
* USE_Qt4 - use Qt4 even if Qt5 is installed, default is to detect Qt5

