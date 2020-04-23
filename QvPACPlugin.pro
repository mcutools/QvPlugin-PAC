TEMPLATE = lib
QT += network widgets gui
CONFIG += qt plugin c++14

include(interface/QvPluginInterface.pri)

HEADERS += \
    $${PWD}/3rdparty/qhttpserver/http-parser/http_parser.h\
    $${PWD}/3rdparty/qhttpserver/src/qhttprequest.h \
    $${PWD}/3rdparty/qhttpserver/src/qhttpconnection.h \
    $${PWD}/3rdparty/qhttpserver/src/qhttpresponse.h \
    $${PWD}/3rdparty/qhttpserver/src/qhttpserver.h \
    $${PWD}/PACPlugin.hpp \
    $${PWD}/core/HTTPRequestHelper.hpp \
    $${PWD}/core/QvPACHandler.hpp \
    $${PWD}/ui/QvPACPluginSettingsWidget.hpp \
    $${PWD}/core/PACPluginProcessor.hpp \
    $${PWD}/core/PACSystemProxyConfigurator.hpp \
    $${PWD}/core/PACConfig.hpp \
    $${PWD}/lib/QJsonStruct/QJsonStruct.hpp


DEFINES += QHTTPSERVER_EXPORT UNICODE _UNICODE
    
INCLUDEPATH += \
    $${PWD}/3rdparty/qhttpserver/ \
    $${PWD}/3rdparty/qhttpserver/http-parser \
    $${PWD}/core/ \
    $${PWD}/lib/QJsonStruct

SOURCES += \
    $${PWD}/3rdparty/qhttpserver/http-parser/http_parser.c\
    $${PWD}/3rdparty/qhttpserver/src/qhttprequest.cpp \
    $${PWD}/3rdparty/qhttpserver/src/qhttpconnection.cpp \
    $${PWD}/3rdparty/qhttpserver/src/qhttpresponse.cpp \
    $${PWD}/3rdparty/qhttpserver/src/qhttpserver.cpp \
    $${PWD}/PACPlugin.cpp \
    $${PWD}/ui/QvPACPluginSettingsWidget.cpp\
    $${PWD}/core/HTTPRequestHelper.cpp \
    $${PWD}/core/QvGFWPACConverter.cpp\
    $${PWD}/core/QvPACHandler.cpp \
    $${PWD}/core/PACPluginProcessor.cpp \
    $${PWD}/core/PACSystemProxyConfigurator.cpp

FORMS += ui/QvPACPluginSettingsWidget.ui

RESOURCES += resx.qrc

QMAKE_CXXFLAGS += -fPIC

win32 {
    LIBS += -lwininet
}

macx {
    QMAKE_RPATHDIR += @executable_path/../Frameworks
}

