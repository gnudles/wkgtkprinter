TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c
unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += gtk+-3.0 gtkmm-3.0 glibmm-2.4 webkit2gtk-4.0
}
