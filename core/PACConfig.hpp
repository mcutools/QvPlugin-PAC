#pragma once
#include "QJsonStruct.hpp"

#include <QJsonObject>
#include <QString>

struct PACSettings
{
    bool enablePAC;
    bool useSocksProxy;
    QString listenIP;
    int listenPort;
    QString proxyServerIP;
    bool setSystemProxy;
    JSONSTRUCT_REGISTER(PACSettings, F(setSystemProxy, enablePAC, useSocksProxy, listenIP, listenPort, proxyServerIP))
};
