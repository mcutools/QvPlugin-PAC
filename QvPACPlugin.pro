TEMPLATE = lib
QT += network widgets gui
CONFIG += qt plugin

HEADERS += \
    $${PWD}/3rdparty/qhttpserver/http-parser/http_parser.h\
    $${PWD}/3rdparty/qhttpserver/src/*.h \
    $${PWD}/PACPlugin.hpp \
    $${PWD}/common/HTTPRequestHelper.hpp \
    $${PWD}/ui/QvPACPluginSettingsWidget.hpp \
    $${PWD}/interface/QvPluginInterface.hpp \
    $${PWD}/pac-core/QvPACHandler.hpp

INCLUDEPATH += \
    $${PWD}/3rdparty/qhttpserver/ \
    $${PWD}/3rdparty/qhttpserver/http-parser

SOURCES += \
    $${PWD}/3rdparty/qhttpserver/http-parser/http_parser.c\
    $${PWD}/3rdparty/qhttpserver/src/*.cpp \
    $${PWD}/PACPlugin.cpp\
    $${PWD}/common/HTTPRequestHelper.cpp\
    $${PWD}/ui/QvPACPluginSettingsWidget.cpp\
    $${PWD}/pac-core/QvGFWPACConverter.cpp\
    $${PWD}/pac-core/QvPACHandler.cpp

FORMS += ui/QvPACPluginSettingsWidget.ui

RESOURCES += resx.qrc
