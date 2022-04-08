# Synnefo (gr. cloud) README
![](http://www.oyranos.org/images/oyranos-config-synnefo.png)

Synnefo is a Qt color management front-end.  It uses the Oyranos CMS to provide the ability to set and examine device profiles, as well as to change system-wide color settings.

### Links
* [About](http://www.oyranos.org/synnefo)
* [Support](https://gitlab.com/oyranos/Synnefo/issues)
* [Copyright](COPYING.md) - BSD
* [Authors](AUTHORS.md)


### Dependencies

This build is currently using the latest Oyranos build through "git". It is required that Oyranos be up-to-date using this.

* [Oyranos](http://www.oyranos.org) - for the Color Management System
* [Qt](http://www.qt.io) - for Qt4 or Qt5 GUI


### Building

Type the following in Synnefo's root directory:

    $ mkdir build
    $ cd build

    $ cmake ..
    $ make
    $ make install

If you wish to uninstall Synnefo, simply type:

    $ make uninstall

#### Build Flags
... are typical cmake flags like CMAKE\_C\_FLAGS to tune compilation.

* CMAKE\_INSTALL\_PREFIX to install into paths and so on. Use on the command 
  line through -DCMAKE\_INSTALL\_PREFIX=/my/path .
* LIB\_SUFFIX - allows to append a architecture specific suffix like 
  LIB\_SUFFIX=64 for 64bit non debian style Linux systems.
* CMAKE\_BUILD\_TYPE - debug or release or what you like
* USE\_Qt4 - use Qt4 even if Qt5 is installed, default is to detect Qt5
* ENABLE\_SHARED\_LIBS - default is ON
* ENABLE\_STATIC\_LIBS - default is ON
* ENABLE\_QT\_DBUS - default is ON
* ENABLE\_APP - default is ON
