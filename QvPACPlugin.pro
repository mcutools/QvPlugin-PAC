TEMPLATE = lib
QT += network widgets gui
CONFIG += qt plugin c++11

include(interface/QvPluginInterface.pri)

HEADERS += \
    $${PWD}/3rdparty/qhttpserver/http-parser/http_parser.h\
    $${PWD}/3rdparty/qhttpserver/src/*.h \
    $${PWD}/PACPlugin.hpp \
    $${PWD}/core/HTTPRequestHelper.hpp \
    $${PWD}/core/QvPACHandler.hpp \
    $${PWD}/ui/QvPACPluginSettingsWidget.hpp \
    core/PACPluginProcessor.hpp

DEFINES += QHTTPSERVER_EXPORT
    
INCLUDEPATH += \
    $${PWD}/3rdparty/qhttpserver/ \
    $${PWD}/3rdparty/qhttpserver/http-parser \
    $${PWD}/core/

SOURCES += \
    $${PWD}/3rdparty/qhttpserver/http-parser/http_parser.c\
    $${PWD}/3rdparty/qhttpserver/src/*.cpp \
    $${PWD}/PACPlugin.cpp \
    $${PWD}/ui/QvPACPluginSettingsWidget.cpp\
    $${PWD}/core/HTTPRequestHelper.cpp \
    $${PWD}/core/QvGFWPACConverter.cpp\
    $${PWD}/core/QvPACHandler.cpp \
    core/PACPluginProcessor.cpp

FORMS += ui/QvPACPluginSettingsWidget.ui

RESOURCES += resx.qrc
