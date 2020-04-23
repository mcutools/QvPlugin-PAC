#pragma once
#include "QJsonStruct.hpp"

#include <QJsonObject>
#include <QString>

class PACSettings
{
  public:
    bool enablePAC;
    bool useSocksProxy;
    QString listenIP;
    int listenPort;
    QString proxyServerIP;
    bool setSystemProxy;
    PACSettings()
        : enablePAC(true),            //
          useSocksProxy(true),        //
          listenIP("127.0.0.1"),      //
          listenPort(8989),           //
          proxyServerIP("127.0.0.1"), //
          setSystemProxy(true){};     //
    JSONSTRUCT_REGISTER(PACSettings, F(setSystemProxy, enablePAC, useSocksProxy, listenIP, listenPort, proxyServerIP))
};
